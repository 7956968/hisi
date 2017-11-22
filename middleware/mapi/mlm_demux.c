#include "string.h"
#include "mapi_inner.h"

#include "hi_debug.h"
#include "hi_unf_demux.h"

#if 0
#define MLDMX_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLDMX_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLDMX_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLDMX_DEBUG(fmt, args...) {}
#endif

#define MLMF_FLT_REGION_NUM 3
#define MLMF_FLT_NUM_PERREGION 32
#define MLMF_MAX_CHN_NUM DMX_TOTALCHAN_CNT
#define MLMF_MAX_MSG_NUM 128

#define MLMF_FLT_LOCK() \
    do {                                                    \
        (void)MLMF_Mutex_Lock((g_FltMutex));           \
    } while (0)

#define MLMF_FLT_UNLOCK() \
    do {                                                    \
        (void)MLMF_Mutex_UnLock((g_FltMutex));         \
    } while (0)

#define IsFltInit() \
    if (g_bFltInit == HI_FALSE) { \
        MLDMX_ERR("filter is not exist!\n"); \
        return HI_FAILURE; \
    } \

#define CheckFltID(id) \
    if ((id) >= DMX_TOTALCHAN_CNT) { \
        MLDMX_ERR("invalid fltid:%d!\n", (id)); \
        return HI_FAILURE; \
    } \


#define DMX_GET_DEV_REGION(dmxid, region) \
    if ((dmxid) == 0) { \
        (region) = 0; \
    } \
    else if ((dmxid) == 4) { \
        (region) = 2; \
    } \
    else {\
        (region) = 1; \
    } \

#define FLT_REVERSE_MASK(mask) (HI_U8)(~(mask))

typedef enum hiFILTER_CALLBACK_TYPE_E
{
    HI_FILTER_CALLBACK_TYPE_DATA_COME,       /**< ���ݵ����ڲ�buffer    */
    HI_FILTER_CALLBACK_TYPE_DATA_IN_BUFFER,  /**< �����ѿ������ⲿbuffer   */
    HI_FILTER_CALLBACK_TYPE_TIMEOUT,         /**< �ȴ����ݳ�ʱ */
    HI_FILTER_CALLBACK_TYPE_BUTT
} HI_FILTER_CALLBACK_TYPE_E;


typedef HI_S32 (*HI_FILTER_CALLBACK)(HI_S32 s32Filterid, HI_FILTER_CALLBACK_TYPE_E enCallbackType,
                                     HI_UNF_DMX_DATA_TYPE_E eDataType, HI_U8 *pu8Buffer, HI_U32 u32BufferLength);

typedef struct  hiFILTER_ATTR_S
{
    HI_U32  u32DMXID;              /* DMX ID ,0-4*/
    HI_U32  u32PID;                 /* TS PID */
    HI_U8  *pu8BufAddr;             /*Transparent transmission mode set to NULL,otherwise you need to get a buffer and record here*//*CNcomment:�ڷ�͸��ģʽ�£�����������ݵ�buffer��ַ����Ҫ������һ���ڴ棬Ȼ����ָ��,͸��ģʽ����ΪNULL */
    HI_U32  u32BufSize;             /* buffer size,Transparent transmission mode set to 0 */

    HI_U32 u32DirTransFlag;        /* if Transparent transmission mode��0 -Not Transparent transmission mode 1 - Transparent transmission mode */
    HI_U32 u32FilterType;          /* section type 0 - SECTION  1- PES */
    HI_U32 u32CrcFlag;             /* crc check flag,0 - close crc check 1-force crc check 2-crc check by sytax*/
    HI_U32 u32TimeOutMs;           /*time out in ms,0 stand for not timeout,otherwise when timeout,the user can receive the envent*//*CNcomment: ��ʱʱ��(ms)��������ʱ��filter��Ȼû�����ݣ�������¼��ص� ���Ϊ0���ʾ����Ҫ��ʱ�¼�*/

    HI_U32 u32FilterDepth;          /*Filter Depth*/
    HI_U8  u8Match[DMX_FILTER_MAX_DEPTH];
    HI_U8  u8Mask[DMX_FILTER_MAX_DEPTH];
    HI_U8  u8Negate[DMX_FILTER_MAX_DEPTH];

    HI_FILTER_CALLBACK funCallback;         /* data comes callback */
} HI_FILTER_ATTR_S;

typedef struct
{
    HI_U32 u32UseFlag;             /*use flag :0- available,1-busy*//*CNcomment:ʹ�ñ��:0 - δʹ�ã�1 - ʹ��*/
    HI_U32 u32EnableFlag;          /*enable flag :0- enable,1-disable*//*CNcomment:ʹ�ܱ��:0 - δʹ�ܣ�1 - ʹ��*/

    HI_HANDLE hChannel;            /*corresponding  channel of filter*//*CNcomment:filter��Ӧ��ͨ����*/
    HI_HANDLE hFilter;             /*corresponding  hander of filter*//*CNcomment:filter��Ӧ�ľ��*/

    HI_U32 u32TimerCount;          /*current value in timer of (ms)filter *//*CNcomment:  ��ǰfilter��ʱ������ֵ,ms*/

    HI_FILTER_ATTR_S stFltAttr;
} FILTER_ATTR_S;

static FILTER_ATTR_S g_FltAttrArr[DMX_TOTALFILTER_CNT];
static HI_BOOL g_bFltInit = HI_FALSE;
static HI_BOOL g_bFltRunFlag = HI_TRUE;
static MET_Mutex_T g_FltMutex;
static MET_Task_T g_FltThreadID;

static MMT_STB_ErrorCode_E (*mf_dmx_pGetPtiDataCall)(MET_PTI_PID_T stCurPid,MBT_U8 *pbuff, MBT_U32 u32Len,MET_PTI_FilterHandle_T  stFileterHanel);

///DMX input configuration
static HI_VOID Flt_DataCallBack(HI_HANDLE Chan, HI_UNF_DMX_DATA_TYPE_E eDataType, HI_U8 *pData, HI_U32 u32SecLen )
{
    HI_U32 i = 0, k = 0;
    HI_FILTER_ATTR_S *pFilter;
    FILTER_ATTR_S    *pFltArrItem;

    if (HI_NULL == pData)
    {
        return;
    }

    for (i = 0; i < DMX_TOTALFILTER_CNT; i++)
    {
        pFilter = &(g_FltAttrArr[i].stFltAttr);
        pFltArrItem = &(g_FltAttrArr[i]);

        if ((pFltArrItem->hChannel != Chan) || (0 == pFltArrItem->u32UseFlag) || (0 == pFltArrItem->u32EnableFlag))
        {
            continue;
        }

        if (0 == pFilter->u32FilterType && pFilter->u32FilterDepth != 0) /*section data,filter data by software*//*CNcomment:section ����,�����ݽ��������*/
        {
            if (pFilter->u8Negate[0])
            {
                if (((pFilter->u8Match[0]) & FLT_REVERSE_MASK(pFilter->u8Mask[0])) ==
                    ((*pData) & FLT_REVERSE_MASK(pFilter->u8Mask[0])))
                {
                    continue;
                }
            }
            else
            {
                if (((pFilter->u8Match[0]) & FLT_REVERSE_MASK(pFilter->u8Mask[0])) !=
                    ((*pData) & FLT_REVERSE_MASK(pFilter->u8Mask[0])))
                {
                    continue;
                }
            }

            for (k = 1; k < pFilter->u32FilterDepth; k++)
            {
                if (pFilter->u8Negate[k])
                {
                    if (((pFilter->u8Match[k]) & FLT_REVERSE_MASK(pFilter->u8Mask[k])) ==
                        ((*(pData + k + 2)) & FLT_REVERSE_MASK(pFilter->u8Mask[k])))
                    {
                        break;
                    }
                }
                else
                {
                    if (((pFilter->u8Match[k]) & FLT_REVERSE_MASK(pFilter->u8Mask[k])) !=
                        ((*(pData + k + 2)) & FLT_REVERSE_MASK(pFilter->u8Mask[k])))
                    {
                        break;
                    }
                }
            }
        }
        else
        {
            k = pFilter->u32FilterDepth; /*do not filter pes*//*CNcomment:pesû��filter����*/
        }

        if (k == pFilter->u32FilterDepth) /*get matched filter condition *//*CNcomment: �õ�ƥ��Ĺ������� */
        {
            if (pFilter->funCallback)
            {
                if (HI_FALSE == pFltArrItem->u32EnableFlag)
                {
                    continue;
                }

                pFltArrItem->u32TimerCount = 0;
                if ((pFilter->u32DirTransFlag == 0) && (pFilter->pu8BufAddr != HI_NULL)
                    && (pFilter->u32BufSize >= u32SecLen))
                {
                    HI_S32 ret;
                    ret = pFilter->funCallback(i, HI_FILTER_CALLBACK_TYPE_DATA_COME, eDataType, (void *)pData,
                                               u32SecLen);
                    if (0 == ret)
                    {
                        memcpy(pFilter->pu8BufAddr, pData, u32SecLen); /*copy data and return to caller,newer data will cover older data *//*CNcomment:�������ݲ����ظ��ϲ�,�����������ݻḲ��ǰ�������*/
                        pFilter->funCallback(i, HI_FILTER_CALLBACK_TYPE_DATA_IN_BUFFER, eDataType, pFilter->pu8BufAddr,
                                             u32SecLen);
                    }
                }
                else
                {
                    /*under transfer mode,data will give caller directly*//*CNcomment:͸��ģʽ�£�����ֱ�Ӹ��ϲ� */
                    pFilter->funCallback(i, HI_FILTER_CALLBACK_TYPE_DATA_COME, eDataType, (void *)pData, u32SecLen);
                }
            }
            else
            {
                MLDMX_DEBUG("filter have no callback\n");
            }

            return; /*if return once find data that need to think over *//*CNcomment:�Ƿ��ҵ����return����Ҫ��*/
        }
    }

    return;
}

static HI_VOID* flt_task( HI_VOID *param)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 u32MsgNum = 0;
    HI_U32 u32BufLen = 0;
    HI_U8  *pTmpBuff = 0;
    HI_U32 u32HandleNum = MLMF_MAX_CHN_NUM;
    HI_HANDLE u32ChHandle[MLMF_MAX_CHN_NUM];
    HI_UNF_DMX_DATA_S sRcvBuf[MLMF_MAX_MSG_NUM];
    HI_UNF_DMX_CHAN_STATUS_S channelstatus;
    HI_UNF_DMX_CHAN_ATTR_S pstChAttr;
    HI_UNF_DMX_DATA_TYPE_E enDataType;
    HI_U32 i = 0, j = 0;

    while (g_bFltRunFlag)
    {
        memset((void *)u32ChHandle, 0, sizeof(HI_HANDLE) * MLMF_MAX_CHN_NUM);

        u32HandleNum = MLMF_MAX_CHN_NUM;
        ret = HI_UNF_DMX_GetDataHandle(u32ChHandle, &u32HandleNum, 1000);  /* timeout 1000ms */
        if ((HI_SUCCESS != ret) || (u32HandleNum == 0))
        {
            continue;
        }

        for (i = 0; i < u32HandleNum; i++) /*amount of channel which has new data*//*CNcomment:�������ݵ�ͨ����*/
        {
            ret = HI_UNF_DMX_GetChannelStatus(u32ChHandle[i], &channelstatus);
            if ((HI_SUCCESS != ret) || (HI_UNF_DMX_CHAN_CLOSE == channelstatus.enChanStatus))
            {
                continue;
            }

            u32MsgNum = 0;
            ret = HI_UNF_DMX_AcquireBuf(u32ChHandle[i], MLMF_MAX_MSG_NUM, &u32MsgNum, sRcvBuf, 1000);
            if (HI_SUCCESS != ret)
            {
                MLDMX_DEBUG("HI_UNF_DMX_AcquireBuf error:%x\n", ret);
                continue;
            }

            ret = HI_UNF_DMX_GetChannelAttr(u32ChHandle[i], &pstChAttr);
            if (HI_SUCCESS != ret)
            {
                MLDMX_DEBUG("HI_UNF_DMX_GetChannelAttr error:%x\n", ret);
                continue;
            }

            for (j = 0; j < u32MsgNum; j++) /*process data package gradually*//*CNcomment:����������ݰ�*/
            {
                MLMF_FLT_LOCK();
                ret = HI_UNF_DMX_GetChannelStatus(u32ChHandle[i], &channelstatus);
                if ((HI_SUCCESS != ret) || ((HI_UNF_DMX_CHAN_CLOSE == channelstatus.enChanStatus) && (HI_SUCCESS == ret)))
                {
                    MLMF_FLT_UNLOCK(); /*avoid reference sRcvBuf after closing channel *//*CNcomment:����ر�ͨ�����ַ���sRcvBuf*/
                    break;
                }

                pTmpBuff   = sRcvBuf[j].pu8Data;
                u32BufLen  = sRcvBuf[j].u32Size;
                enDataType = sRcvBuf[j].enDataType;

                if (HI_UNF_DMX_CHAN_TYPE_PES == pstChAttr.enChannelType)
                {
                    Flt_DataCallBack(u32ChHandle[i], enDataType, pTmpBuff, u32BufLen);
                }
                else if (HI_UNF_DMX_CHAN_TYPE_SEC == pstChAttr.enChannelType)
                {
                    Flt_DataCallBack(u32ChHandle[i], HI_UNF_DMX_DATA_TYPE_WHOLE, pTmpBuff, u32BufLen);
                }

                MLMF_FLT_UNLOCK();
            }

            /*release message*//*CNcomment: �ͷ���Ϣ */
            MLMF_FLT_LOCK();
            ret = HI_UNF_DMX_GetChannelStatus(u32ChHandle[i], &channelstatus);
            if ((HI_SUCCESS != ret) || (HI_UNF_DMX_CHAN_CLOSE == channelstatus.enChanStatus))
            {
                MLMF_FLT_UNLOCK();
                continue;
            }
            else
            {
                ret = HI_UNF_DMX_ReleaseBuf(u32ChHandle[i], u32MsgNum, sRcvBuf);
                if (HI_SUCCESS != ret)
                {
                    MLDMX_DEBUG("HI_UNF_DMX_ReleaseBuf error:%x\n", ret);
                }

                MLMF_FLT_UNLOCK();
            }
        }
    }

    return NULL;
}

static HI_S32 CheckFltAttr(HI_FILTER_ATTR_S *pstFilterAttr)
{
    if (!pstFilterAttr)
    {
        return -1;
    }

    if ((pstFilterAttr->u32DirTransFlag == 0) && (!pstFilterAttr->pu8BufAddr
                                                  || (pstFilterAttr->u32BufSize == 0))) /*did not send available buffer under un-transfer mode *//*CNcomment:��͸��ģʽû�д���Ϸ���buffer */
    {
        MLDMX_DEBUG("error buffer\n");
        return -1;
    }

    if (pstFilterAttr->u32DMXID >= DMX_CNT)
    {
        MLDMX_DEBUG("error dmx id:%x(0~4)\n", pstFilterAttr->u32DMXID);
        return -1;
    }

    if ((pstFilterAttr->u32FilterType != 0) && (pstFilterAttr->u32FilterType != 1))
    {
        MLDMX_DEBUG("error filter type:%x!\n", pstFilterAttr->u32FilterType);
        return -1;
    }

    if (pstFilterAttr->u32CrcFlag > 2)
    {
        MLDMX_DEBUG("error u32CrcFlag flag:%x\n", pstFilterAttr->u32CrcFlag);
        return -1;
    }

    if (pstFilterAttr->u32FilterDepth > DMX_FILTER_MAX_DEPTH)
    {
        MLDMX_DEBUG("error filter depth:%x\n", pstFilterAttr->u32FilterDepth);
        return -1;
    }

    return 0;
}

/*pu32FltId's value scope is 0-95 *//*CNcomment:pu32FltId��ȡ���ķ�ΧΪ0-95*/
static HI_S32 GetFreeFltId(HI_U32 u32DmxID, HI_U32 *pu32FltId)
{
    HI_U32 i;
    HI_U32 u32RegionNum;
    FILTER_ATTR_S *pFilter;

    DMX_GET_DEV_REGION(u32DmxID, u32RegionNum);
    for (i = 0; i < MLMF_FLT_NUM_PERREGION; i++)
    {
        pFilter = &(g_FltAttrArr[u32RegionNum * MLMF_FLT_NUM_PERREGION + i]);
        if (pFilter->u32UseFlag == 0)
        {
            *pu32FltId = u32RegionNum * MLMF_FLT_NUM_PERREGION + i;
            return 0;
        }
    }

    return -1;
}

static HI_U32 GetChnFltNum(HI_S32 u32DmxID, HI_HANDLE Chan)
{
    HI_U32 i = 0;
    HI_U32 u32RegionNum;
    HI_U32 u32FltNum = 0;
    FILTER_ATTR_S *pFilter;

    DMX_GET_DEV_REGION(u32DmxID, u32RegionNum);
    for (i = 0; i < MLMF_FLT_NUM_PERREGION; i++)
    {
        pFilter = &(g_FltAttrArr[u32RegionNum * MLMF_FLT_NUM_PERREGION + i]);
        if (!pFilter->u32UseFlag)
        {
            continue;
        }

        if (pFilter->hChannel == Chan)
        {
            u32FltNum++;
        }
    }

    return u32FltNum;
}

/*get filter amount which channel is enable *//*CNcomment:��ȡͨ��ʹ�ܵ�filter��Ŀ*/
static HI_U32 GetChnEnFltNum(HI_S32 u32DmxID, HI_HANDLE Chan)
{
    HI_U32 i = 0;
    HI_U32 u32RegionNum;
    HI_U32 u32FltNum = 0;
    FILTER_ATTR_S *pFilter;

    DMX_GET_DEV_REGION(u32DmxID, u32RegionNum);
    for (i = 0; i < MLMF_FLT_NUM_PERREGION; i++)
    {
        pFilter = &(g_FltAttrArr[u32RegionNum * MLMF_FLT_NUM_PERREGION + i]);
        if (!pFilter->u32UseFlag || !pFilter->u32EnableFlag)
        {
            continue;
        }

        if (pFilter->hChannel == Chan)
        {
            u32FltNum++;
        }
    }

    return u32FltNum;
}


static HI_S32 _MLMF_FILTER_Creat(HI_FILTER_ATTR_S *pstFilterAttr, HI_S32 *ps32FilterID)
{
    HI_U32 u32FltID;
    HI_UNF_DMX_FILTER_ATTR_S sFilterAttr;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr;
    HI_HANDLE hFilter;
    HI_HANDLE hChannel;
    FILTER_ATTR_S* ps_FltAttr;
    HI_S32 ret;

    IsFltInit();
    if (CheckFltAttr(pstFilterAttr) != 0)
    {
        MLDMX_DEBUG("invalid param!\n");
        return HI_FAILURE;
    }

    MLMF_FLT_LOCK();
    if (GetFreeFltId(pstFilterAttr->u32DMXID, &u32FltID) != 0)
    {
        MLDMX_DEBUG("no free filter!\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    memset(&sFilterAttr, 0, sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    sFilterAttr.u32FilterDepth = pstFilterAttr->u32FilterDepth;
    memcpy(sFilterAttr.au8Mask, pstFilterAttr->u8Mask, pstFilterAttr->u32FilterDepth);
    memcpy(sFilterAttr.au8Match, pstFilterAttr->u8Match, pstFilterAttr->u32FilterDepth);
    memcpy(sFilterAttr.au8Negate, pstFilterAttr->u8Negate, pstFilterAttr->u32FilterDepth);
    ret = HI_UNF_DMX_CreateFilter(pstFilterAttr->u32DMXID, &sFilterAttr, &hFilter);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_CreateFilter error:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    ret = HI_UNF_DMX_GetChannelHandle(pstFilterAttr->u32DMXID, pstFilterAttr->u32PID, &hChannel);
    if ((HI_SUCCESS != ret) && (HI_ERR_DMX_UNMATCH_CHAN != ret))
    {
        MLDMX_DEBUG("HI_UNF_DMX_GetChannelHandle error:%x !\n", ret);
        HI_UNF_DMX_DestroyFilter(hFilter);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if ((HI_ERR_DMX_UNMATCH_CHAN == ret) || (HI_INVALID_HANDLE == hChannel))
    {
        /*do have channel for this PID,it need to create new channel *//*CNcomment:û�и�pid��ͨ������Ҫ�����µ�ͨ�� */
        ret = HI_UNF_DMX_GetChannelDefaultAttr(&stChAttr);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_GetChannelDefaultAttr error:%x !\n", ret);
            HI_UNF_DMX_DestroyFilter(hFilter);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }

        if (pstFilterAttr->u32CrcFlag == 1)
        {
            stChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD;
        }
        else if (pstFilterAttr->u32CrcFlag == 2)
        {
            stChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
        }
        else
        {
            stChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        }

        if (pstFilterAttr->u32FilterType == 1)
        {
            stChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_PES;
        }
        else
        {
            stChAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
        }

        stChAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;

        stChAttr.u32BufSize = 512 * 1024; //not used,don't care

        ret = HI_UNF_DMX_CreateChannel(pstFilterAttr->u32DMXID, &stChAttr, &hChannel);
        if (HI_SUCCESS != ret)
        {
            HI_UNF_DMX_DestroyFilter(hFilter);
            MLDMX_DEBUG("HI_UNF_DMX_CreateChannel error:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }

        ret = HI_UNF_DMX_SetChannelPID(hChannel, pstFilterAttr->u32PID);
        if (HI_SUCCESS != ret)
        {
            HI_UNF_DMX_DestroyFilter(hFilter);
            HI_UNF_DMX_DestroyChannel(hChannel);
            MLDMX_DEBUG("HI_UNF_DMX_SetChannelPID error:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }
    }

    ps_FltAttr = &g_FltAttrArr[u32FltID];
    memset(ps_FltAttr, 0, sizeof(FILTER_ATTR_S));
    ps_FltAttr->u32EnableFlag = 0;
    ps_FltAttr->u32TimerCount = 0;
    ps_FltAttr->hFilter  = hFilter;
    ps_FltAttr->hChannel = hChannel;
    memcpy(&ps_FltAttr->stFltAttr, pstFilterAttr, sizeof(HI_FILTER_ATTR_S));
    ps_FltAttr->u32UseFlag = 1;
    *ps32FilterID = u32FltID;

    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}

static HI_S32 _MLMF_FILTER_Destroy(HI_S32 s32FilterID)
{
    FILTER_ATTR_S* ps_FltAttr;
    HI_S32 ret;

    IsFltInit();
    CheckFltID(s32FilterID);
    MLMF_FLT_LOCK();
    ps_FltAttr = &g_FltAttrArr[s32FilterID];
    if ((HI_INVALID_HANDLE == ps_FltAttr->hFilter) || (HI_INVALID_HANDLE == ps_FltAttr->hChannel))
    {
        MLDMX_DEBUG("filter handle error !\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if (ps_FltAttr->u32EnableFlag)
    {
        ret = HI_UNF_DMX_DetachFilter(ps_FltAttr->hFilter, ps_FltAttr->hChannel);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_DetachFilter error:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }

        ps_FltAttr->u32EnableFlag = 0;
    }

    ret = HI_UNF_DMX_DestroyFilter(ps_FltAttr->hFilter);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_DestroyFilter error:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    ps_FltAttr->u32UseFlag = 0;
    if (!GetChnFltNum(ps_FltAttr->stFltAttr.u32DMXID, ps_FltAttr->hChannel)) /*if the channel did not attach filter,destroy the channel*//*CNcomment:ͨ����û�а�filter��������ͨ��*/
    {
        ret = HI_UNF_DMX_DestroyChannel(ps_FltAttr->hChannel);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_DestroyChannel error:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }
    }

    ps_FltAttr->hChannel = HI_INVALID_HANDLE;
    ps_FltAttr->hFilter = HI_INVALID_HANDLE;
    ps_FltAttr->u32TimerCount = 0;

    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}

static HI_S32 _MLMF_FILTER_SetAttr(HI_S32 s32FilterID, HI_FILTER_ATTR_S *pstFilterAttr)
{
    FILTER_ATTR_S* ps_FltAttr;
    HI_UNF_DMX_FILTER_ATTR_S sFilterAttr;
    HI_UNF_DMX_CHAN_ATTR_S sChanAttr;
    HI_S32 ret;

    IsFltInit();
    CheckFltID(s32FilterID);
    if (CheckFltAttr(pstFilterAttr) != 0)
    {
        MLDMX_DEBUG("invalid param!\n");
        return HI_FAILURE;
    }

    MLMF_FLT_LOCK();
    ps_FltAttr = &g_FltAttrArr[s32FilterID];
    if (pstFilterAttr->u32FilterType != ps_FltAttr->stFltAttr.u32FilterType)
    {
        MLDMX_DEBUG("can not change the channel type!\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if ((HI_INVALID_HANDLE == ps_FltAttr->hFilter) || (HI_INVALID_HANDLE == ps_FltAttr->hChannel))
    {
        MLDMX_DEBUG("filter handle error !\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    memset(&sFilterAttr, 0, sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    sFilterAttr.u32FilterDepth = pstFilterAttr->u32FilterDepth;
    memcpy(sFilterAttr.au8Mask, pstFilterAttr->u8Mask, pstFilterAttr->u32FilterDepth);
    memcpy(sFilterAttr.au8Match, pstFilterAttr->u8Match, pstFilterAttr->u32FilterDepth);
    memcpy(sFilterAttr.au8Negate, pstFilterAttr->u8Negate, pstFilterAttr->u32FilterDepth);
    ret = HI_UNF_DMX_SetFilterAttr(ps_FltAttr->hFilter, &sFilterAttr);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_SetFilterAttr error:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if (ps_FltAttr->stFltAttr.u32CrcFlag != pstFilterAttr->u32CrcFlag)
    {
        ret = HI_UNF_DMX_GetChannelAttr(ps_FltAttr->hChannel, &sChanAttr);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_GetChannelAttr error:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }

        if (sChanAttr.enChannelType == HI_UNF_DMX_CHAN_TYPE_SEC)
        {
            if ((ps_FltAttr->stFltAttr.u32CrcFlag == 1)
                && (sChanAttr.enCRCMode != HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD))
            {
                sChanAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD;
                ret  = HI_UNF_DMX_CloseChannel(ps_FltAttr->hChannel);
                ret |= HI_UNF_DMX_SetChannelAttr(ps_FltAttr->hChannel, &sChanAttr);
                ret |= HI_UNF_DMX_OpenChannel(ps_FltAttr->hChannel);
            }
            else if ((ps_FltAttr->stFltAttr.u32CrcFlag == 2) && (sChanAttr.enCRCMode != HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD))
            {
                sChanAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
                ret  = HI_UNF_DMX_CloseChannel(ps_FltAttr->hChannel);
                ret |= HI_UNF_DMX_SetChannelAttr(ps_FltAttr->hChannel, &sChanAttr);
                ret |= HI_UNF_DMX_OpenChannel(ps_FltAttr->hChannel);
            }
            else if ((ps_FltAttr->stFltAttr.u32CrcFlag == 0) && (sChanAttr.enCRCMode != HI_UNF_DMX_CHAN_CRC_MODE_FORBID))
            {
                sChanAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
                ret  = HI_UNF_DMX_CloseChannel(ps_FltAttr->hChannel);
                ret |= HI_UNF_DMX_SetChannelAttr(ps_FltAttr->hChannel, &sChanAttr);
                ret |= HI_UNF_DMX_OpenChannel(ps_FltAttr->hChannel);
            }

            if (HI_SUCCESS != ret)
            {
                MLDMX_DEBUG("set crc mode error:%x !\n", ret);
                MLMF_FLT_UNLOCK();
                return HI_FAILURE;
            }
        }
    }

    memcpy(&ps_FltAttr->stFltAttr, pstFilterAttr, sizeof(HI_FILTER_ATTR_S));
    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}


static HI_S32 _MLMF_FILTER_GetAttr (HI_S32 s32FilterID, HI_FILTER_ATTR_S *pstFilterAttr)
{
    FILTER_ATTR_S* ps_FltAttr;

    IsFltInit();
    CheckFltID(s32FilterID);
    if (!pstFilterAttr)
    {
        HI_ERR_FLT("null pointer!\n");
        return HI_FAILURE;
    }

    MLMF_FLT_LOCK();
    ps_FltAttr = &g_FltAttrArr[s32FilterID];
    memcpy(pstFilterAttr, &ps_FltAttr->stFltAttr, sizeof(HI_FILTER_ATTR_S));
    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}

static HI_S32 _MLMF_FILTER_Start(HI_S32 s32FilterID)
{
    FILTER_ATTR_S* ps_FltAttr;
    HI_UNF_DMX_CHAN_STATUS_S stStatus;
    HI_S32 ret;

    IsFltInit();
    CheckFltID(s32FilterID);
    MLMF_FLT_LOCK();
    ps_FltAttr = &g_FltAttrArr[s32FilterID];
    if ((HI_INVALID_HANDLE == ps_FltAttr->hFilter) || (HI_INVALID_HANDLE == ps_FltAttr->hChannel))
    {
        MLDMX_DEBUG("filter handle error !\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if (ps_FltAttr->u32EnableFlag)
    {
        MLDMX_DEBUG("filter already started!\n");
        MLMF_FLT_UNLOCK();
        return HI_SUCCESS;
    }

    ret = HI_UNF_DMX_AttachFilter(ps_FltAttr->hFilter, ps_FltAttr->hChannel);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_AttachFilter error:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    ps_FltAttr->u32EnableFlag = 1;
    ps_FltAttr->u32TimerCount = 0;
    ret = HI_UNF_DMX_GetChannelStatus(ps_FltAttr->hChannel, &stStatus);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_GetChannelStatus failed:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if (HI_UNF_DMX_CHAN_CLOSE == stStatus.enChanStatus)
    {
        ret = HI_UNF_DMX_OpenChannel(ps_FltAttr->hChannel);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_OpenChannel failed:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }
    }

    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}

static HI_S32 _MLMF_FILTER_Stop(HI_S32 s32FilterID)
{
    FILTER_ATTR_S* ps_FltAttr;
    HI_UNF_DMX_CHAN_STATUS_S stStatus;
    HI_S32 ret;

    IsFltInit();
    CheckFltID(s32FilterID);
    MLMF_FLT_LOCK();
    ps_FltAttr = &g_FltAttrArr[s32FilterID];
    if ((HI_INVALID_HANDLE == ps_FltAttr->hFilter) || (HI_INVALID_HANDLE == ps_FltAttr->hChannel))
    {
        MLDMX_DEBUG("filter handle error !\n");
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    if (!ps_FltAttr->u32EnableFlag)
    {
        MLDMX_DEBUG("filter already stoped!\n");
        MLMF_FLT_UNLOCK();
        return HI_SUCCESS;
    }

    ret = HI_UNF_DMX_DetachFilter(ps_FltAttr->hFilter, ps_FltAttr->hChannel);
    if (HI_SUCCESS != ret)
    {
        MLDMX_DEBUG("HI_UNF_DMX_DetachFilter error:%x !\n", ret);
        MLMF_FLT_UNLOCK();
        return HI_FAILURE;
    }

    ps_FltAttr->u32EnableFlag = 0;
    ps_FltAttr->u32TimerCount = 0;

    if (!GetChnEnFltNum(ps_FltAttr->stFltAttr.u32DMXID, ps_FltAttr->hChannel)) /*if the channel did not attach filter,close the channel*//*CNcomment:ͨ����û�а�filter����ر�ͨ��*/
    {
        ret = HI_UNF_DMX_GetChannelStatus(ps_FltAttr->hChannel, &stStatus);
        if (HI_SUCCESS != ret)
        {
            MLDMX_DEBUG("HI_UNF_DMX_GetChannelStatus failed:%x !\n", ret);
            MLMF_FLT_UNLOCK();
            return HI_FAILURE;
        }

        if (HI_UNF_DMX_CHAN_CLOSE != stStatus.enChanStatus)
        {
            ret = HI_UNF_DMX_CloseChannel(ps_FltAttr->hChannel);
            if (HI_SUCCESS != ret)
            {
                MLDMX_DEBUG("HI_UNF_DMX_CloseChannel error:%x !\n", ret);
                MLMF_FLT_UNLOCK();
                return HI_FAILURE;
            }
        }
    }

    MLMF_FLT_UNLOCK();
    return HI_SUCCESS;
}


static void _MLMF_FILTER_CallBack(HI_S32 s32Filterid, HI_FILTER_CALLBACK_TYPE_E enCallbackType,
                                     HI_UNF_DMX_DATA_TYPE_E eDataType, HI_U8 *pu8Buffer, HI_U32 u32BufferLength)
{
	FILTER_ATTR_S* ps_FltAttr;
	HI_S32 ret;

	IsFltInit();
	CheckFltID(s32Filterid);
//	MLMF_FLT_LOCK();
	ps_FltAttr = &g_FltAttrArr[s32Filterid];
	
	if (NULL != mf_dmx_pGetPtiDataCall)
	{
		mf_dmx_pGetPtiDataCall(ps_FltAttr->stFltAttr.u32PID,pu8Buffer,u32BufferLength,s32Filterid);
	}
}


/*
*˵��������һ���ص��������ݵ���ʱͨ�����ô˻ص�������֪ͨ�ϲ㡣
*������������.���úù���������,ÿ������������,����ûص�����,
*������������.��Ϊ���ݵĽ��պܿ죬�������Ӵ�Ϊ���������Ч�ʣ�
*����һ�����ݿ����Ĺ��̣�����涨�ɵײ�������pbuff���ڴ棬����pDataCallbk������
*pbuff���ϲ�ӹܣ��ײ㲻�ͷ�pbuff���ϲ���������MLMF_PTI_FreeSectionBuffer�ͷš�
*���������
*	pDataCallbk�����յ������Ժ���Ҫ���õĻص��������˻ص��������ڼ��̵�ʱ����
*                                �ڵ��ô˻ص�����ʱ������ش���˵���ϲ�û�нӹܴ����ݣ�
*                               �ײ���Ҫ�����ͷ�buffer���ڴ�
*ִ���꣬��������ײ��������
*�������:
*			�ޡ�
*		����  
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_SetNotify (MMT_STB_ErrorCode_E (*pDataCallbk)(MET_PTI_PID_T stCurPid,MBT_U8 *pbuff, MBT_U32 u32Len,MET_PTI_FilterHandle_T  stFileterHanel))
{
	mf_dmx_pGetPtiDataCall = pDataCallbk;
	return MM_NO_ERROR;
}

/*
*˵������ͨ���ϴ����������ˣ����������ӱ�
*���������
*		cHandle��ͨ�������
*�������:
*			pFilter�������ɹ��󷵻ص�filter����������ɹ��򷵻�MBT_NULL��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_CreateFilter(MET_PTI_PID_T pid,MBT_U32 u32BufSize,MET_PTI_FilterHandle_T *pFilter,MMT_PTI_DataSrc_E eDataType)
{
	HI_S32 ret;
	HI_FILTER_ATTR_S _stFilterAttr;
	HI_S32 _32FilterID;

	*pFilter = MM_INVALID_HANDLE;

	_stFilterAttr.u32DMXID = 0;
	_stFilterAttr.u32FilterType = 0;//Section
	_stFilterAttr.funCallback = _MLMF_FILTER_CallBack;
	_stFilterAttr.u32DirTransFlag = 1;//Transparent transmission mode 
	_stFilterAttr.u32PID = pid;
	_stFilterAttr.u32CrcFlag = 0;
	_stFilterAttr.u32TimeOutMs = 2000;
	memset(_stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
	memset(_stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
	memset(_stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
		
	ret = _MLMF_FILTER_Creat(&_stFilterAttr,&_32FilterID);

	if (HI_SUCCESS != ret)
	{
		return MM_ERROR_UNKNOW;
	}

	*pFilter = _32FilterID;	
	return MM_NO_ERROR;
}


/*
*˵��������һ��filter��
*���������
*		cHandle��ͨ�������
*			vFilter�����������Ҫ���ٵ�filter�����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_DestroyFilter(MET_PTI_FilterHandle_T vFilter)
{
	HI_S32 ret;
	
	ret = _MLMF_FILTER_Destroy(vFilter);

	if (HI_SUCCESS != ret)
	{
		return MM_ERROR_UNKNOW;
	}
	
	return MM_NO_ERROR;
}


/*
*˵��������һ��filter�Ĺ��˲�����ͬʱע�����ݽ��յĻص�����.
*������������.���úù���������,ÿ������������,����ûص�����,
*������������.��Ϊ���ݵĽ��պܿ죬�������Ӵ�
*Ϊ���������Ч�ʣ�����һ�����ݿ����Ĺ��̣�
*����涨�ɵײ�������pbuff���ڴ棬����pDataCallbk������
*pbuff���ϲ�ӹܣ��ײ㲻�ͷ�pbuff���ϲ�������ͷš�
*���������
*			vFilter����Ҫ���õ�filter�����
*			MaskData�����˲����ṹ��
*pDataCallbk�����յ������Ժ���Ҫ���õĻص��������˻ص��������ڼ��̵�ʱ����ִ���꣬��������ײ��������
*�������:
*			�ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_SetFilter(MET_PTI_FilterHandle_T vFilter,MST_PTI_MaskData_T *MaskData)
{
	HI_S32 ret;
	HI_FILTER_ATTR_S _stFilterAttr;

	if (NULL == MaskData)
	{
		return MM_ERROR_BAD_PARAMETER;
	}

	_MLMF_FILTER_GetAttr(vFilter,&_stFilterAttr);

	memset(_stFilterAttr.u8Mask,0,DMX_FILTER_MAX_DEPTH);
	memset(_stFilterAttr.u8Match,0,DMX_FILTER_MAX_DEPTH);
	memset(_stFilterAttr.u8Negate,0,DMX_FILTER_MAX_DEPTH);
	//_stFilterAttr.u8Match[0] = PAT_TABLEID;
	memcpy(_stFilterAttr.u8Match,MaskData->filter,MaskData->filter_len);
	memcpy(_stFilterAttr.u8Mask,MaskData->mask,MaskData->filter_len);

#if 0
	_stFilterAttr.u8Match[0] = MaskData->filter[0];
	_stFilterAttr.u8Mask[0] = MaskData->mask[0];
	if(MaskData->filter_len > 1)
	{
		_stFilterAttr.u32FilterDepth = MaskData->filter_len + 2;
		memcpy(_stFilterAttr.u8Match+3, MaskData->filter+1,MaskData->filter_len-1);
		memcpy(_stFilterAttr.u8Mask+3,MaskData->mask+1,MaskData->filter_len-1);
	}
	else
	{
		_stFilterAttr.u32FilterDepth = 1;
	}
	
#endif	
	_stFilterAttr.u32FilterDepth = MaskData->filter_len;
	
	ret = _MLMF_FILTER_SetAttr(vFilter,&_stFilterAttr);

	if (HI_SUCCESS != ret)
	{
		return MM_ERROR_UNKNOW;
	}
	
	return MM_NO_ERROR;
}


/*
*˵��������һ��filter�������ݡ�
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_EnableFilter(MET_PTI_FilterHandle_T vFilter)
{
	HI_S32 ret;
	
	ret = _MLMF_FILTER_Start(vFilter);

	if (HI_SUCCESS != ret)
	{
		return MM_ERROR_UNKNOW;
	}

	return MM_NO_ERROR;
}



/*
*˵����ֹͣһ���������ݵ�filter��
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_DisableFilter(MET_PTI_FilterHandle_T vFilter)
{
	HI_S32 ret;
	
	ret = _MLMF_FILTER_Stop(vFilter);

	if (HI_SUCCESS != ret)
	{
		return MM_ERROR_UNKNOW;
	}
	
	return MM_NO_ERROR;
}


/*
*˵�����ͷ���pDataCallbk�ص�������������pbuff�ڴ档
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*pbuff��Section���ݵ��ڴ�ָ�롣
*�������:
*			�ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
MBT_VOID MLMF_PTI_FreeSectionBuffer (MET_PTI_FilterHandle_T vFilter,MBT_U8 *pbuff)
{
    //MLDMX_DEBUG("MW NOT Create mem,return");
    return;
}


/*
*˵��:����һ����������������ָ���Ľ���ģʽ������
*���������
*	dsmHandle�������������
*	Encryptmode������ģʽ��
*�������:
*			�ޡ�
*	���أ�
*		MM_NO_ERROR:�ɹ���
*		����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerAllocate(MET_DescramblerHandle *dsmHandle, MMT_PTI_DsmMode_E encryptmode)
{
	
}


/*
*˵�����ͷ�ָ������Ľ�����ͨ����
*���������
*	dsmHandle�������������
*�������:
*			�ޡ�
*	����
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerFree(MET_DescramblerHandle dsmHandle)
{
	
}



/*
*˵��:Ϊָ������Ľ�����ͨ����һ��pid��
*���������
*dsmHandle�������������
*pid�������ŵ�����pid��
*�������:
*			�ޡ�
*���أ�
*	MM_NO_ERROR:�ɹ���
*����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetPid(MET_DescramblerHandle dsmHandle, MET_PTI_PID_T pid)
{
	
}


/*
*˵��:Ϊָ������Ľ�����ͨ��,����������֡�
*���������
*dsmHandle�������������
*key�������֡�
*keyLen�������ֳ��ȡ�
*�������:
*			�ޡ�
*���أ�
*MM_NO_ERROR���ɹ���
*����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetOddKey(MET_DescramblerHandle dsmHandle,MBT_U8 *key, MBT_S32 keyLen)
{
	
}


/*
*˵����Ϊָ������Ľ�����ͨ��,����ż�����֡�
*���������
*	dsmHandle�������������
*	key�������֡�
*	keyLen�������ֳ��ȡ�
*�������:
*			�ޡ�
*	���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetEvenKey(MET_DescramblerHandle dsmHandle,MBT_U8 *key, MBT_S32 keyLen)
{
	
}


MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedDescramblerSetKeys(MET_DescramblerHandle dsmHandle,MBT_U8 *OddKey,MBT_U8 *EvenKey,MBT_S32 keyLen)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSelectRootKey(MBT_U8 u8RootKeyID)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetContentKey(MBT_U32 u32KeyLen,MBT_U8 *pu8ContentKey)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetCommonKey(MBT_U16 u16Algorithm,MBT_U8 *pu8CommonKey)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetTempSessionKey(MBT_U16 u16Algorithm,MBT_U8 *pu8EncrypedTSK)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetContentKey_extern(MBT_U32 u32KeyLen,MBT_U8 *pu8ContentKey)
{    
    return MM_NO_ERROR;
}


MMT_STB_ErrorCode_E MLF_DmxInit(MBT_VOID)
{
    HI_S32 ret;

    if (g_bFltInit == HI_TRUE)
    {
        return MM_NO_ERROR;
    }

    ret = HI_UNF_DMX_Init();

    HI_UNF_DMX_AttachTSPort(0,HI_UNF_DMX_PORT_0_TUNER);
	
    if (HI_SUCCESS != ret)
    {
        return MM_ERROR_UNKNOW;
    }

    memset(g_FltAttrArr, 0, sizeof(FILTER_ATTR_S) * DMX_TOTALFILTER_CNT);
    //pthread_mutex_init(&g_FltMutex, NULL);
    MLMF_Mutex_Create(&g_FltMutex);
    g_bFltRunFlag = HI_TRUE;

    //ret  = pthread_create(&g_FltThreadID, NULL, flt_task, NULL);
    ret = MLMF_Task_Create(flt_task,
                                            NULL,
                                            1024,
                                            7,
						  &g_FltThreadID,
                                            "DMX_FLT_TASK");	

    if (MM_NO_ERROR != ret)
    {
        return MM_ERROR_UNKNOW;
    }

    g_bFltInit = HI_TRUE;

    return MM_NO_ERROR;
}


MMT_STB_ErrorCode_E MLF_DmxTerm(MBT_VOID)
{
	
}



MBT_VOID MLF_ConnectVideo2DSM(MBT_U16 u32VduPID)
{
	
}


MBT_VOID MLF_ConnectAudio2DSM(MBT_U16 u32AudPID)
{
	
}


//-------------------- test code ----------------------//

