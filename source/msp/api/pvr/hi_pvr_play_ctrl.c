/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_play_ctrl.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/14
  Description   : PLAY module
  History       :
  1.Date        : 2008/04/14
    Author      : q46153
    Modification: Created file

******************************************************************************/

#include <malloc.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>

#include "hi_type.h"

#include "hi_mpi_pvr.h"
#include "hi_mpi_avplay.h"

#include "pvr_debug.h"
#include "hi_pvr_play_ctrl.h"
#include "hi_pvr_index.h"
#include "hi_pvr_rec_ctrl.h"
#include "hi_pvr_intf.h"
#include "hi_pvr_priv.h"
#include "hi_pvr_smooth_ctrl.h"
#include "hi_mpi_demux.h"
#include "hi_drv_pvr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

//#define DUMP_DMX_DATA

#define TIMESHIFT_INVALID_CHN           0XFF
#define PVR_PLAY_MAX_SEND_BUF_SIZE      (256*1024)

#define PVR_DMX_TS_BUFFER_GAP           0x100   /*DMX_TS_BUFFER_GAP*/

extern HI_S32   g_s32PvrFd;      /*PVR module file description */
extern char api_pathname_pvr[];

/* initial flag for play module                                             */
STATIC PVR_PLAY_COMM_S g_stPlayInit;

/* all information of play channel                                          */
STATIC PVR_PLAY_CHN_S g_stPvrPlayChns[PVR_PLAY_MAX_CHN_NUM];
#ifdef DMUP_FILE_DATA
static FILE *g_pvrfpSend = NULL; /* handle of file */
#endif
static HI_BOOL g_bPlayTimerInitFlag = HI_FALSE;
static timer_t g_stPlayTimer;

/*play direction whether it is forward or not: current or before pause the state is play, fast forward, slow forward or step forward */
#define PVR_IS_PLAY_FORWARD(StateNow, StateLast)\
         (  (HI_UNF_PVR_PLAY_STATE_PLAY == StateNow) \
         || (HI_UNF_PVR_PLAY_STATE_FF == StateNow) \
         || (HI_UNF_PVR_PLAY_STATE_SF == StateNow) \
         || (HI_UNF_PVR_PLAY_STATE_STEPF == StateNow)\
         || ((HI_UNF_PVR_PLAY_STATE_PAUSE == StateNow) \
             && ( (HI_UNF_PVR_PLAY_STATE_PLAY == StateLast) \
               || (HI_UNF_PVR_PLAY_STATE_FF == StateLast) \
               || (HI_UNF_PVR_PLAY_STATE_SF == StateLast) \
               || (HI_UNF_PVR_PLAY_STATE_STEPF == StateLast) \
          )))

/*play direction whether it is backward or not: current or before puase the state is fast rewind or step rewind */
#define PVR_IS_PLAY_BACKWARD(StateNow, StateLast)\
         (  (HI_UNF_PVR_PLAY_STATE_FB == StateNow) \
         || (HI_UNF_PVR_PLAY_STATE_STEPB == StateNow)\
         || ((HI_UNF_PVR_PLAY_STATE_PAUSE == StateNow) \
             && ( (HI_UNF_PVR_PLAY_STATE_FB == StateLast) \
               || (HI_UNF_PVR_PLAY_STATE_STEPB == StateLast) \
          )))

static void PVRPlayCalcTime(union sigval unSig)
{
    HI_U32 i = 0;
    HI_S32 s32SeedRatio = 0;
    PVR_PLAY_CHN_S *pChnAttr;
    HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S stAvplayPrivInfo;
    
    for(i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        pChnAttr = &g_stPvrPlayChns[i];

        if ((HI_UNF_PVR_PLAY_STATE_PAUSE == pChnAttr->enState) ||
            (HI_UNF_PVR_PLAY_STATE_STEPF == pChnAttr->enState) ||
            (pChnAttr->bEndOfFile) ||
            (pChnAttr->bStartOfFile))
        {
            usleep(1000);
            continue;
        }
            
        if (HI_TRUE == pChnAttr->bInterPauseForTimeshift)
        {
            if (HI_TRUE == pChnAttr->bRecordedVideoExist)
            {
                memset(&stAvplayPrivInfo, 0 ,sizeof(HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S));
                if (HI_SUCCESS == HI_UNF_AVPLAY_Invoke(pChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO, (void *)&stAvplayPrivInfo))
                {
                    if (-1 != stAvplayPrivInfo.u32LastPlayTime)
                    {
                        pChnAttr->u32CurPlayTimeMs = stAvplayPrivInfo.u32LastPlayTime;
                    }
                }
            }
            continue;
        }

        if (!(HI_UNF_PVR_PLAY_STATE_STOP == pChnAttr->enState
          || HI_UNF_PVR_PLAY_STATE_INVALID == pChnAttr->enState))
        {
            /* normal or fast mode */
            if (abs(pChnAttr->enSpeed) >= HI_UNF_PVR_PLAY_SPEED_NORMAL)
            {
                s32SeedRatio = pChnAttr->enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL;
                pChnAttr->u32CurPlayTimeMs += (s32SeedRatio*((HI_S32)PVR_TIME_CTRL_TIMEBASE_NS))/1000000;
            }
            else/* slow mode */
            {
                s32SeedRatio = (HI_S32)HI_UNF_PVR_PLAY_SPEED_NORMAL/pChnAttr->enSpeed;
                pChnAttr->u32CurPlayTimeMs += (((HI_S32)PVR_TIME_CTRL_TIMEBASE_NS)/s32SeedRatio)/1000000;
            }
        }

        if((HI_S32)pChnAttr->u32CurPlayTimeMs < 0)
        {
            pChnAttr->u32CurPlayTimeMs = 0;
        }
    }
}

#ifdef PVR_PROC_SUPPORT
static HI_PROC_ENTRY_S g_stPvrPlayProcEntry;
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
static HI_S32 PVRTplayShowProc(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData, PVR_PLAY_CHN_S *pChnAttr)
{
    HI_PVR_SMOOTH_PARA_S   *pstFBAttr = &pChnAttr->stFBAttr;
    HI_PVR_FAST_BACKWARD_S *pstBKInfo = &pstFBAttr->stBKInfo;
    HI_PVR_FETCH_RESULT_S  *pstPvrFetchRes = pstFBAttr->pstPvrFetchRes;
    
    HI_U32 u32SpeedRate = abs(pChnAttr->enSpeed)/HI_UNF_PVR_PLAY_SPEED_NORMAL;
    HI_CHAR FFTillEnd[32]   = "FF Till End";
    HI_CHAR FBTillStart[32] = "FB Till Start";
    HI_CHAR StatusSuccess[32] = "Success";

    if ((HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState) || 
        (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState))
    {
        HI_PROC_Printf(pstBuf, "\n");
        if (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)
        {
            HI_PROC_Printf(pstBuf, "\t-------- FB control -----------\n");
        }
        else
        {
            HI_PROC_Printf(pstBuf, "\t-------- FF control ------------\n");
        }

        if ( pstFBAttr->s32FFFBStatus == HI_ERR_PVR_FILE_TILL_START)
        {
            HI_PROC_Printf(pstBuf, "\tStatus                    :%s\n", FBTillStart);
        }
        else if ( pstFBAttr->s32FFFBStatus == HI_ERR_PVR_FILE_TILL_END )
        {
            HI_PROC_Printf(pstBuf, "\tStatus                    :%s\n", FFTillEnd);
        }
        else if(pstFBAttr->s32FFFBStatus == HI_SUCCESS)
        {
            HI_PROC_Printf(pstBuf, "\tStatus                    :%s\n", StatusSuccess);
        }
        else
        {
            HI_PROC_Printf(pstBuf, "\tStatus                    :0x%x\n", pstFBAttr->s32FFFBStatus);
        }
            
        HI_PROC_Printf(pstBuf, "\tDecodec ability           :%d\n", pstFBAttr->u32DecodeAblity);/*оƬ�������� (1S���ܽ������֡ȫ��������)*/
        HI_PROC_Printf(pstBuf, "\tWidth                     :%d\n", pstFBAttr->u32Width);
        HI_PROC_Printf(pstBuf, "\tHeight                    :%d\n", pstFBAttr->u32Heigth);
        HI_PROC_Printf(pstBuf, "\tDisp Frame buffer         :%d\n", pstFBAttr->u32DispFrmBufNum); 
        HI_PROC_Printf(pstBuf, "\tVO Frame buffer           :%d\n", pstFBAttr->u32VoUsedFrmBufNum); 
        HI_PROC_Printf(pstBuf, "\tOri frame rate            :%d\n", pstFBAttr->u32SourceFrameRate);/*����ԭʼ֡��*/
        HI_PROC_Printf(pstBuf, "\tVO In FrmRate(Int:dec)    :%d:%d\n", pstFBAttr->u32VoRate/1000,pstFBAttr->u32VoRate%1000);/*����������������ø�VO������֡�ʵ�������С��ֵ*/        
        HI_PROC_Printf(pstBuf, "\tMax Dec Frm Num 1s        :%d\n", pstFBAttr->u32MaxDecFrmNum1S);/*��Ա�������1S �������ܽ�������֡��*/
        HI_PROC_Printf(pstBuf, "\tIDR flag                  :%d\n", pstFBAttr->stVdecCtrlInfo.u32IDRFlag);
        HI_PROC_Printf(pstBuf, "\tB frame ref flag          :%d\n", pstFBAttr->stVdecCtrlInfo.u32BFrmRefFlag);
        HI_PROC_Printf(pstBuf, "\tContinuous flag           :%d\n", pstFBAttr->stVdecCtrlInfo.u32ContinuousFlag);
        HI_PROC_Printf(pstBuf, "\tDispOptimize flag         :%d\n", pstFBAttr->stVdecCtrlInfo.u32DispOptimizeFlag);
        HI_PROC_Printf(pstBuf, "\tField flag                :%d\n", pstFBAttr->bFieldFlg);

        if (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)
        {
            HI_PROC_Printf(pstBuf, "\tSupported max gop size    :%d\n", pstBKInfo->u32MaxGopSize);
            if (pChnAttr->enSpeed == HI_UNF_PVR_PLAY_SPEED_2X_FAST_BACKWARD)
            {
                HI_PROC_Printf(pstBuf, "\n");
                HI_PROC_Printf(pstBuf, "\t----FB 2X Optimize info--\n");
                HI_PROC_Printf(pstBuf, "\tFBOptimize flag           :%d\n", pstBKInfo->bBK2xOptimize);
                if (pstBKInfo->bBK2xOptimize)
                {
                    HI_PROC_Printf(pstBuf, "\tDisplay distance          :%d\n", pstBKInfo->u32DispDistance);                
                    HI_PROC_Printf(pstBuf, "\tFrmCnt not disp From Last :%d\n", pstBKInfo->u32FrmRemainsBeforeThisGop);  
                    /*����������ϢΪ0��ʱ��-2* ���˻ử��û����ʾ���� ��֡*/
                    HI_PROC_Printf(pstBuf, "\tDisplay enable(this gop)  :%d\n", pstBKInfo->stFrmTag.u32DispEnableFlag);
                    HI_PROC_Printf(pstBuf, "\tVdec FBOptimizeFlag(Should be 1)  :%d\n", pstFBAttr->stVdecCtrlInfo.u32BackwardOptimizeFlag);
                }
            }  
        }
        HI_PROC_Printf(pstBuf, "\n");
        HI_PROC_Printf(pstBuf, "\t---PVR Extract Frm info---\n");
        HI_PROC_Printf(pstBuf, "\tExtract or not            :%d\n", pstFBAttr->bPVRExtract);/*PVR �Ƿ������֡*/
        if (pstFBAttr->bPVRExtract)
        {
            HI_PROC_Printf(pstBuf, "\tEntry frame num           :%d\n", pstFBAttr->u32EntryFrameNum);/*��һ�ν���Smooth��֡ */
            HI_PROC_Printf(pstBuf, "\tTry frame num             :%d\n", pstFBAttr->u32TryFrameNum);/*������֡��Χ*/
            HI_PROC_Printf(pstBuf, "\tStart frame num           :%d\n", pstFBAttr->u32StartFrmNum);/*������֡��Χ����ʼ֡��*/
            HI_PROC_Printf(pstBuf, "\tEnd frame num             :%d\n", pstFBAttr->u32EndFrmNum);/*������֡��Χ�Ľ���֡��*/
            HI_PROC_Printf(pstBuf, "\tframe cnt to send         :%d\n", (pstPvrFetchRes->u32TotalFrameNum + u32SpeedRate - 1)/u32SpeedRate);/*����Ӧ���������ͽ����֡��*/
            
            {
                HI_PROC_Printf(pstBuf, "\tTotal frame num           :%d\n", pstPvrFetchRes->u32TotalFrameNum);
                HI_PROC_Printf(pstBuf, "\tTotal GOP num             :%d\n", pstPvrFetchRes->u32GopNum);
                HI_PROC_Printf(pstBuf, "\tTotal I frame num         :%d\n", pstPvrFetchRes->u32IFrameNum);
                HI_PROC_Printf(pstBuf, "\tTotal P frame num         :%d\n", pstPvrFetchRes->u32PFrameNum);
                HI_PROC_Printf(pstBuf, "\tTotal B frame num         :%d\n", pstPvrFetchRes->u32BFrameNum);
            }
            if (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)
            {
                HI_PROC_Printf(pstBuf, "\tNeed drop I Frm           :%d\n", pstBKInfo->bNeedDropI);
            }
            
        }
        
        HI_PROC_Printf(pstBuf, "\t-----------------------------\n");
    }
    return HI_SUCCESS;
}
#endif
static HI_S32 PVRPlayShowProc(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_VOID *pPrivData)
{    
    HI_U32 i=0;
    HI_U32 u32VidType=0;
    PVR_PLAY_CHN_S *pChnAttr = NULL;
    PVR_INDEX_INFO_S* pstIdxInfo = NULL;
    
    HI_S8 pStreamType[][32] = {"MPEG2", "MPEG4 DIVX4 DIVX5", "AVS", "H263", "H264",
                             "REAL8", "REAL9", "VC-1", "VP6", "VP6F", "VP6A", "MJPEG",
                             "SORENSON SPARK", "DIVX3", "RAW", "JPEG", "VP8", "MSMPEG4V1",
                             "MSMPEG4V2", "MSVIDEO1", "WMV1", "WMV2", "RV10", "RV20",
                             "SVQ1", "SVQ3", "H261", "VP3", "VP5", "CINEPAK", "INDEO2",
                             "INDEO3", "INDEO4", "INDEO5", "MJPEGB", "MVC", "HEVC", "DV", "INVALID"};
    HI_S8 pPlayStats[][16] = { "INVALID", "INIT", "PLAY", "PAUSE", "FF", "FB", "SF", "STEPF",
                             "STEPB", "STOP", "BUTT"};
    
    HI_PROC_Printf(pstBuf, "\n---------Hisilicon PVR Playing channel Info---------\n");
    
    for(i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        pChnAttr = &g_stPvrPlayChns[i];
        
        if (pChnAttr->IndexHandle == NULL)
        {
            HI_WARN_PVR("pChnAttr %d index is null\n", i);
            break;
        }

        pstIdxInfo = &pChnAttr->IndexHandle->stRecIdxInfo.stIdxInfo;
        
        if ((pChnAttr->enState != HI_UNF_PVR_PLAY_STATE_INVALID) &&
            (pChnAttr->enState != HI_UNF_PVR_PLAY_STATE_STOP) &&
            (pChnAttr->enState != HI_UNF_PVR_PLAY_STATE_BUTT))
        {

            u32VidType = PVR_Index_GetVtype(pChnAttr->IndexHandle)-100;
            u32VidType = (u32VidType > HI_UNF_VCODEC_TYPE_BUTT) ? HI_UNF_VCODEC_TYPE_BUTT : u32VidType;
        
            HI_PROC_Printf(pstBuf, "chan %d infomation\n", i);
            
            HI_PROC_Printf(pstBuf, "\tPlay filename     \t:%s\n", pChnAttr->stUserCfg.szFileName);
            HI_PROC_Printf(pstBuf, "\tStream type       \t:%s\n", pStreamType[u32VidType]);
            HI_PROC_Printf(pstBuf, "\ttimeshift bufferMs\t:%ums\n", pChnAttr->u32BufferTimeForTimeshift);
            HI_PROC_Printf(pstBuf, "\tDemuxID           \t:%d\n", pChnAttr->u32chnID);
            HI_PROC_Printf(pstBuf, "\tTsBuffer handle   \t:%#x\n", pChnAttr->hTsBuffer);
            HI_PROC_Printf(pstBuf, "\tAvplay handle     \t:%#x\n", pChnAttr->hAvplay);
            HI_PROC_Printf(pstBuf, "\tCipher handle     \t:%#x\n", pChnAttr->hCipher);
            HI_PROC_Printf(pstBuf, "\tPlay State        \t:%s\n", pPlayStats[pChnAttr->enState]);
            HI_PROC_Printf(pstBuf, "\tPlay Speed        \t:%d\n", pChnAttr->enSpeed);
            HI_PROC_Printf(pstBuf, "\tStream Read Pos   \t:%#llx\n", pChnAttr->u64CurReadPos);
            HI_PROC_Printf(pstBuf, "\tIndex Start       \t:%d\n", pChnAttr->IndexHandle->stCycMgr.u32StartFrame);
            HI_PROC_Printf(pstBuf, "\tIndex End         \t:%d\n", pChnAttr->IndexHandle->stCycMgr.u32EndFrame);
            HI_PROC_Printf(pstBuf, "\tIndex Last        \t:%d\n", pChnAttr->IndexHandle->stCycMgr.u32LastFrame);
           
            HI_PROC_Printf(pstBuf, "\tStart Frm&GOP num \t:%d %d\n", (pChnAttr->u32GopNumOfStart)>>16, (pChnAttr->u32GopNumOfStart)&0xffff);
            HI_PROC_Printf(pstBuf, "\tTotal GOP num     \t:%d\n", pstIdxInfo->u32GopTotalNum);
            HI_PROC_Printf(pstBuf, "\tMax GOP size      \t:%d\n", pstIdxInfo->u32MaxGopSize);
 
            if (0 != pstIdxInfo->u32GopTotalNum)
            {
                HI_PROC_Printf(pstBuf, "\tAverage GOP size  \t:%d\n", 
                               (pstIdxInfo->u32FrameTotalNum)/(pstIdxInfo->u32GopTotalNum));  
            }
            HI_PROC_Printf(pstBuf, "\tIndex GOP distr   \t:%d %d %d %d %d %d %d %d %d %d %d %d %d\n", 
                                   pstIdxInfo->u32GopSizeInfo[0],
                                   pstIdxInfo->u32GopSizeInfo[1],
                                   pstIdxInfo->u32GopSizeInfo[2],
                                   pstIdxInfo->u32GopSizeInfo[3],
                                   pstIdxInfo->u32GopSizeInfo[4],
                                   pstIdxInfo->u32GopSizeInfo[5],
                                   pstIdxInfo->u32GopSizeInfo[6],
                                   pstIdxInfo->u32GopSizeInfo[7],
                                   pstIdxInfo->u32GopSizeInfo[8],
                                   pstIdxInfo->u32GopSizeInfo[9],
                                   pstIdxInfo->u32GopSizeInfo[10],
                                   pstIdxInfo->u32GopSizeInfo[11],
                                   pstIdxInfo->u32GopSizeInfo[12]);
            HI_PROC_Printf(pstBuf, "\tIndex Read Now    \t:%d\n", pChnAttr->IndexHandle->u32ReadFrame);
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
            (HI_VOID)PVRTplayShowProc(pstBuf, pPrivData,pChnAttr);
#endif
        }
    }
    return HI_SUCCESS;
}

HI_S32 PVRPlaySetProc(HI_PROC_SHOW_BUFFER_S * pstBuf, HI_U32 u32Argc, HI_U8 *pu8Argv[], HI_VOID *pPrivData)
{
#ifndef HI_PVR_SOFT_INDEX_SUPPORT   
    HI_U32 u32PrintFlg = 0;
#endif
    HI_U32 u32ChnNum = 0;
    PVR_PLAY_CHN_S *pChnAttr = g_stPvrPlayChns;
    
    if (2 != u32Argc)
    {
        HI_ERR_PVR("echo pvr_play argc is incorrect.\n");
        return HI_FAILURE;
    }

    u32ChnNum = strtoul((HI_CHAR *)pu8Argv[0], HI_NULL, 10);

    if (u32ChnNum >= PVR_PLAY_MAX_CHN_NUM)
    {
        HI_ERR_PVR("invalid channel number %d\n.", u32ChnNum);
        return HI_FAILURE;
    }

    if ((pChnAttr[u32ChnNum].enState == HI_UNF_PVR_PLAY_STATE_INVALID) ||
            (pChnAttr[u32ChnNum].enState == HI_UNF_PVR_PLAY_STATE_STOP) ||
            (pChnAttr[u32ChnNum].enState == HI_UNF_PVR_PLAY_STATE_BUTT))
    {
        HI_ERR_PVR("channel status is invalid.\n");
        return HI_FAILURE;
    }
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
    if ((HI_UNF_PVR_PLAY_STATE_FF == pChnAttr[u32ChnNum].enState) ||  
        (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr[u32ChnNum].enState))
    {
        u32PrintFlg = strtoul((HI_CHAR *)pu8Argv[1], HI_NULL, 10);

        if(1 == u32PrintFlg)
        {
            g_stPvrPlayChns[u32ChnNum].stFBAttr.bPrintFlg = HI_TRUE;
        }
        else if (0 == u32PrintFlg)
        {
            g_stPvrPlayChns[u32ChnNum].stFBAttr.bPrintFlg = HI_FALSE;
        }
    }
#endif    
    return HI_SUCCESS;
}

#endif
static INLINE HI_S32 PVRPlayReadDataFromFile(PVR_PLAY_CHN_S *pstChn, 
                                             HI_VOID *pBuf, 
                                             HI_U32 u32BytesSend, 
                                             HI_U64 u64Offset)
{
    HI_U64 u64Offset1 = 0;
    HI_U64 u64Offset2 = 0;
    HI_U32 u32Size1 = 0;
    HI_U32 u32Size2 = 0;
    HI_U32 u32TotalRead = 0;
    HI_CHAR *pTempBuf = HI_NULL;

    
    if(pstChn->IndexHandle->stCycMgr.u64MaxCycSize > 0)
    {
        if (PVR_INDEX_IS_REWIND(pstChn->IndexHandle))
        {
            u64Offset1 = u64Offset % PVR_INDEX_MAX_FILE_SIZE(pstChn->IndexHandle);
            if (u64Offset1 + u32BytesSend > PVR_INDEX_MAX_FILE_SIZE(pstChn->IndexHandle))
            {
                u32Size1 = PVR_INDEX_MAX_FILE_SIZE(pstChn->IndexHandle) - u64Offset1;
                u32Size2 = u32BytesSend - u32Size1;
                u64Offset2 = (HI_U64)0;
            }
        }
    }
    if (0 == u32Size1)
    {
        return (HI_S32)PVR_PREAD64(pBuf, u32BytesSend, pstChn->s32DataFile, u64Offset);
    }
    else
    {
        u32TotalRead = (HI_U32)PVR_PREAD64(pBuf, u32Size1, pstChn->s32DataFile, u64Offset1);
        if (u32Size1 == u32TotalRead)
        {
            pTempBuf = (HI_CHAR *)pBuf + u32TotalRead;
            u32TotalRead += (HI_U32)PVR_PREAD64((HI_U8 *)pTempBuf, u32Size2, pstChn->s32DataFile, u64Offset2);
        }
        return u32TotalRead;       
    }
}
STATIC INLINE HI_S32 PVRPlayReadData(PVR_PLAY_CHN_S *pstChn, 
                                      HI_VOID *pBuf, 
                                      HI_U32 u32BytesSend, 
                                      PVR_FILE64 File, 
                                      HI_U64 u64Offset, 
                                      HI_U64 u64GlobalOffset,
                                      HI_U32 u32CipherHead,
                                      HI_U32 u32HeadToAdd)
{
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
    HI_S32 s32Cached = 0;
    HI_U32 u32ReadedCnt = 0;
    HI_U32 u32FileReadSize = 0;
    HI_U64 u64ReadGlobalOffset = 0;

    if (pstChn->bPlayMainThreadStop)
    {
        return 0;
    }

    u64ReadGlobalOffset = u64GlobalOffset - u32HeadToAdd - u32CipherHead;

    s32Cached = PVR_TS_CACHE_BUFFER_ReadFromBuffer(&pstChn->IndexHandle->stTsCacheBufInfo, u64ReadGlobalOffset, u32BytesSend, pBuf);
    if (0 > s32Cached)
    {
        return HI_ERR_PVR_PLAY_INDEX_BEYOND_TS;
    }
    else if ((HI_U32)s32Cached < u32BytesSend)
    {
        u32FileReadSize = u32BytesSend - (HI_U32)s32Cached;
        u32ReadedCnt = PVRPlayReadDataFromFile(pstChn, pBuf, u32FileReadSize, u64Offset);
        if (u32ReadedCnt != u32FileReadSize)
        {
            HI_ERR_PVR("Read file part fail! want=%#x readed=%#x\n", u32FileReadSize, u32ReadedCnt);
            return u32ReadedCnt;
        }
        HI_WARN_PVR("read %u bytes from cache, and %u from the file\n", s32Cached, u32FileReadSize);
    }

    return s32Cached + u32ReadedCnt;
#else
    return PVRPlayReadDataFromFile(pstChn, pBuf, u32BytesSend, u64Offset);
#endif
}

STATIC INLINE HI_S32 PVRPlayDevInit(HI_VOID)
{
    int fd;

    if (g_s32PvrFd == -1)
    {
        fd = open (api_pathname_pvr, O_RDWR , 0);

        if(fd < 0)
        {
            HI_FATAL_PVR("Cannot open '%s'\n", api_pathname_pvr);
            return HI_FAILURE;
        }
        g_s32PvrFd = fd;

    }

    return HI_SUCCESS;
}

STATIC INLINE PVR_PLAY_CHN_S * PVRPlayFindFreeChn(HI_VOID)
{
    PVR_PLAY_CHN_S * pChnAttr = NULL;

    /* find a free play channel */
#if 0 /*not support multi-thread */
    HI_U32 i;
    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        if (g_stPvrPlayChns[i].enState == HI_UNF_PVR_PLAY_STATE_INVALID)
        {
            pChnAttr = &g_stPvrPlayChns[i];
            pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INIT;
            break;
        }
    }
#else /* manage the resources by kernel driver */
    HI_U32 ChanId;
    if (HI_SUCCESS != ioctl(g_s32PvrFd, CMD_PVR_CREATE_PLAY_CHN, (HI_S32)&ChanId))
    {
        HI_FATAL_PVR("pvr play creat channel error\n");
        return HI_NULL;
    }

    HI_ASSERT(g_stPvrPlayChns[ChanId].enState == HI_UNF_PVR_PLAY_STATE_INVALID);
    pChnAttr = &g_stPvrPlayChns[ChanId];

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INIT;
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
#endif

    return pChnAttr;
}


STATIC INLINE HI_S32 PVRPlayCheckUserCfg(const HI_UNF_PVR_PLAY_ATTR_S *pUserCfg, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer)
{
    HI_S32 ret;
    HI_UNF_AVPLAY_ATTR_S         AVPlayAttr;
    HI_UNF_AVPLAY_STATUS_INFO_S  StatusInfo;
    HI_UNF_DMX_TSBUF_STATUS_S    TsBufStatus;
    HI_CHAR szIndexName[PVR_MAX_FILENAME_LEN];
    HI_U32 i;

    if (HI_UNF_PVR_STREAM_TYPE_TS != pUserCfg->enStreamType )
    {
        HI_ERR_PVR("invalid play enStreamType:%d\n", pUserCfg->enStreamType );
        return HI_ERR_PVR_INVALID_PARA;
    }

    PVR_CHECK_CIPHER_CFG(&pUserCfg->stDecryptCfg);

    /*  if play file name ok */
    if (!((pUserCfg->u32FileNameLen > 0)
        && (strlen(pUserCfg->szFileName) == pUserCfg->u32FileNameLen)))
    {
        HI_ERR_PVR("Invalid file name!\n");
        return HI_ERR_PVR_FILE_INVALID_FNAME;
    }

    if (PVR_PLAY_BUFFER_MAX_TIME_TIMESHIFT < pUserCfg->u32TimeshiftBufferMs)
    {
        HI_ERR_PVR("the buffer time for timeshift is more large, max is: %ums!\n", (HI_U32)PVR_PLAY_BUFFER_MAX_TIME_TIMESHIFT);
        return HI_ERR_PVR_INVALID_PARA;
    }

    /* check if stream exist!                                               */
    if (!PVR_CHECK_FILE_EXIST64(pUserCfg->szFileName))
    {
        HI_ERR_PVR("Stream file %s doesn't exist!\n", pUserCfg->szFileName);
        return HI_ERR_PVR_FILE_NOT_EXIST;
    }
    //snprintf(szIndexName, PVR_MAX_FILENAME_LEN,"%s.%s", pUserCfg->szFileName, "idx");
    PVR_Index_GetIdxFileName(szIndexName,  (HI_CHAR *)pUserCfg->szFileName);
    if (!PVR_CHECK_FILE_EXIST(szIndexName))
    {
        HI_ERR_PVR("can NOT find index file for '%s'!\n", pUserCfg->szFileName);
        return HI_ERR_PVR_FILE_NOT_EXIST;
    }

    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        /* check whether demux id is used or not */
        if (HI_UNF_PVR_PLAY_STATE_INVALID != g_stPvrPlayChns[i].enState)
        {
            /* check whether the same file is playing or not*/
            if (0 == strncmp(g_stPvrPlayChns[i].stUserCfg.szFileName, pUserCfg->szFileName,sizeof(pUserCfg->szFileName)))
            {
                HI_ERR_PVR("file %s was exist to be playing.\n", pUserCfg->szFileName);
                return HI_ERR_PVR_FILE_EXIST;
            }

            if (g_stPvrPlayChns[i].hAvplay == hAvplay)
            {
                HI_ERR_PVR("avplay 0x%x already has been used to play.\n", hAvplay);
                return HI_ERR_PVR_ALREADY;
            }
            if (g_stPvrPlayChns[i].hTsBuffer == hTsBuffer)
            {
                HI_ERR_PVR("Ts buffer 0x%x already has been used to play.\n", hTsBuffer);
                return HI_ERR_PVR_ALREADY;
            }
        }

    }

    ret = HI_UNF_AVPLAY_GetStatusInfo(hAvplay, &StatusInfo);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("check hAvplay for PVR failed:%#x\n", ret);
        return HI_FAILURE;
    }
    if (StatusInfo.enRunStatus != HI_UNF_AVPLAY_STATUS_STOP)
    {
        HI_WARN_PVR("the hAvplay is not stopped\n");

        //lint -e655
        ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("can NOT stop hAvplay for pvr replay\n");
            return ret;
        }
        //lint +e655
    }

    ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_STREAM_MODE, &AVPlayAttr);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("check hAvplay attr failed\n");
        return HI_FAILURE;
    }
    if (AVPlayAttr.stStreamAttr.enStreamType != HI_UNF_AVPLAY_STREAM_TYPE_TS)
    {
        HI_ERR_PVR("hAvplay's enStreamType is NOT TS.\n");
        return HI_ERR_PVR_INVALID_PARA;
    }

    ret = HI_UNF_DMX_GetTSBufferStatus(hTsBuffer, &TsBufStatus);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("check hTsBuffer failed.\n");
        return HI_ERR_PVR_INVALID_PARA;
    }

    return HI_SUCCESS;
}

STATIC INLINE HI_S32 PVRPlayPrepareCipher(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 ret;
    HI_UNF_PVR_CIPHER_S *pCipherCfg;
    HI_UNF_CIPHER_CTRL_S ctrl;
    HI_UNF_CIPHER_ATTS_S stCipherAttr;

    pCipherCfg = &(pChnAttr->stUserCfg.stDecryptCfg);
    if (!pCipherCfg->bDoCipher)
    {
        return HI_SUCCESS;
    }

    /* get cipher handle */
    stCipherAttr.enCipherType = HI_UNF_CIPHER_TYPE_NORMAL;
    ret = HI_UNF_CIPHER_CreateHandle(&pChnAttr->hCipher, &stCipherAttr);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_CIPHER_CreateHandle failed:%#x\n", ret);
        return ret;
    }

    ctrl.enAlg = pCipherCfg->enType;
    ctrl.bKeyByCA = HI_FALSE;
    memcpy(ctrl.u32Key, pCipherCfg->au8Key, sizeof(ctrl.u32Key));
    memset(ctrl.u32IV, 0, sizeof(ctrl.u32IV));

    if (HI_UNF_CIPHER_ALG_AES ==  pCipherCfg->enType )
    {
        ctrl.enBitWidth = PVR_CIPHER_AES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_AES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_AES_KEY_LENGTH;
    }
    else if (HI_UNF_CIPHER_ALG_DES ==  pCipherCfg->enType )
    {
        ctrl.enBitWidth = PVR_CIPHER_DES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_DES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_DES_KEY_LENGTH;
    }
    else
    {
        ctrl.enBitWidth = PVR_CIPHER_3DES_BIT_WIDTH;
        ctrl.enWorkMode = PVR_CIPHER_3DES_WORK_MODD;
        ctrl.enKeyLen = PVR_CIPHER_3DES_KEY_LENGTH;
    }

    ret = HI_UNF_CIPHER_ConfigHandle(pChnAttr->hCipher, &ctrl);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_CIPHER_ConfigHandle failed:%#x\n", ret);
        (HI_VOID)HI_UNF_CIPHER_DestroyHandle(pChnAttr->hCipher);
        return ret;
    }

    return HI_SUCCESS;
}

STATIC INLINE HI_S32 PVRPlayReleaseCipher(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 ret = HI_SUCCESS;

    /* free cipher handle */
    if ( (pChnAttr->stUserCfg.stDecryptCfg.bDoCipher) && (pChnAttr->hCipher) )
    {
        ret = HI_UNF_CIPHER_DestroyHandle(pChnAttr->hCipher);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("release Cipher handle failed! erro:%#x\n", ret);
        }
        pChnAttr->hCipher = 0;
    }

    return ret;
}
#ifndef HI_PVR_SOFT_INDEX_SUPPORT   
/*
table 2-3 -- ITU-T Rec. H.222.0 | ISO/IEC 13818 transport packet
        Syntax                    No. of bits        Mnemonic
transport_packet(){
    sync_byte                        8            bslbf
    transport_error_indicator        1            bslbf
    payload_unit_start_indicator     1            bslbf
    transport_priority               1            bslbf
    PID                              13            uimsbf
    transport_scrambling_control     2            bslbf
    *********************************************************
***    adaptation_field_control      2            bslbf ****** the 2bit we want
    *********************************************************
    continuity_counter               4            uimsbf
    if(adaptation_field_control=='10'  || adaptation_field_control=='11'){
        adaptation_field()
    }
    if(adaptation_field_control=='01' || adaptation_field_control=='11') {
        for (i=0;i<N;i++){
            data_byte                8            bslbf
        }
    }
}


"2.4.3.2 Transport Stream packet layer" of ISO 13818-1

adaptation_field_control
    -- This 2 bit field indicates whether this Transport Stream packet header
        is followed by an adaptation field and/or payload.

Table 2-6 -- Adaptation field control values
value    description
00    reserved for future use by ISO/IEC
01    no adaptation_field, payload only
10    adaptation_field only, no payload
11    adaptation_field followed by payload
*/
STATIC INLINE HI_VOID RVRPlaySetAdptFiled(HI_U8 *pTsHead, HI_U32 flag)
{
    HI_U8 byte4 = pTsHead[3];

    HI_ASSERT(flag <= 0x3);

    byte4 = byte4 & 0xcf;/* clear adp field bits */
    byte4 = byte4 | ((flag & 0x3) << 4); /* set adp field bits */

    pTsHead[3] = byte4;
}

STATIC INLINE HI_U8 RVRPlayGetAdptFiled(const HI_U8 *pTsHead)
{
    HI_U8 byte4 = pTsHead[3];

    byte4 = (byte4 >> 4) & 0x3;

    return byte4;
}

/*****************************************************************************
 Prototype       : RVRPlayDisOrderCnt,RVRPlayDisOrderCntEnd
 Description     : disorderly number, prevent from making the demux regard repeatly number  as lost packet
 Input           : pTsHead  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/7/21
    Author       : fd
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_VOID RVRPlayDisOrderCnt(HI_U8 *pTsHead)
{
    HI_U8 oldCnt;
    HI_U8 byte4 = pTsHead[3];

    oldCnt = byte4 & 0xf;

    oldCnt += 5; /* 5 is one random number, make the calu discontinuous */

    byte4 = byte4 & 0xf0;
    byte4 = byte4 | (oldCnt & 0xf);

    pTsHead[3] = byte4;
}

STATIC INLINE HI_VOID RVRPlayDisOrderCntEnd(HI_U8 *pTsHead)
{
    HI_U8 oldCnt;
    HI_U8 byte4 = pTsHead[3];

    oldCnt = byte4 & 0xf;

    oldCnt += 3; /* 3 is one random number, make the calu discontinuous */

    byte4 = byte4 & 0xf0;
    byte4 = byte4 | (oldCnt & 0xf);

    pTsHead[3] = byte4;
}
/* modify adaptation_field_control field, and set that length, pading with 0xff */
STATIC INLINE HI_VOID PVRPlaySetTsHead(HI_U8 *pTsHead, HI_U32 dataStartPos)
{
    HI_U8 adapt_flag;

    if (PVR_TS_HEAD_SIZE == dataStartPos)
    {
        return;
    }

    if (0 != (pTsHead[1] & 0x40))
    {
        return;
    }

    if (dataStartPos >= PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE) /* modify the padding area length */
    {
        adapt_flag = RVRPlayGetAdptFiled(pTsHead);
        /*AI7D02961 the dataStartPos is not equal the size of ts head, it should be both */
        RVRPlaySetAdptFiled(pTsHead, PVR_TS_ADAPT_BOTH);

        pTsHead[PVR_TS_PD_SIZE_POS] = (HI_U8)(dataStartPos - (PVR_TS_HEAD_SIZE + 1));

        if (!PVR_TS_ADAPT_HAVE_ADAPT(adapt_flag))
        {
            pTsHead[PVR_TS_PD_FLAG_POS] = 0;
        }

        memset(pTsHead + PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE, 0xff,
                    dataStartPos - (PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE));
    }
    else /* only 1Byte Adapt_len */
    {
        RVRPlaySetAdptFiled(pTsHead, PVR_TS_ADAPT_BOTH);
        pTsHead[PVR_TS_PD_SIZE_POS] = 0;
    }

    RVRPlayDisOrderCnt(pTsHead);
   
    return;
}
/*
dataEnd: position of valid data end in last TS pkg(Byte)
*                               dataEnd
*                                 |
original:                         V
* -----------------------------------------------------------
*| TS head | pending | valid data |  invalid data     |
*------------------------------------------------------------

0 == PVR_TS_MOVE_TO_END:
* -----------------------------------------------------------
*| TS head | pending | valid data |  0xff 0xff  ...  |
*------------------------------------------------------------

1 == PVR_TS_MOVE_TO_END:
* -----------------------------------------------------------
*| TS head | pending |   0xff 0xff  ... | valid data |
*------------------------------------------------------------
*/
STATIC INLINE HI_VOID PVRPlayAddTsEnd(HI_U8 *pBufAddr, HI_U32 dataEnd,  HI_U32 u32EndToAdd)
{
    /*CNcomment:����������Ҫ�����β*/
    HI_U8 *pLastTsHead;
    HI_U8  adapt_flag;
    HI_U32 dataInLastTs ;

   if (0 == u32EndToAdd)
   {
        return;
   }
#if 0 == PVR_TS_MOVE_TO_END
    memset((HI_U8*)pBufAddr + dataEnd , 0xff, u32EndToAdd);
#else

    pLastTsHead = pBufAddr + dataEnd + u32EndToAdd - PVR_TS_LEN;

    if (0 != (pLastTsHead[1] & 0x40))
    {
        pLastTsHead[1] = 0x1f;
        pLastTsHead[2] = 0xff;
        return;
    }

    adapt_flag = RVRPlayGetAdptFiled(pLastTsHead);

    /* TODO: 2B or 1B adapt-head */
    RVRPlaySetAdptFiled(pLastTsHead,
            ((u32EndToAdd + PVR_TS_HEAD_SIZE) == PVR_TS_LEN)?
            PVR_TS_ADAPT_ADAPT_ONLY : PVR_TS_ADAPT_BOTH);
    RVRPlayDisOrderCntEnd(pLastTsHead);
    /* AI7D04104 event if it should have adaptation, we need to check whether it length is zero or not */
    if (PVR_TS_ADAPT_HAVE_ADAPT(adapt_flag) && 0 != pLastTsHead[PVR_TS_PD_SIZE_POS]) /* existent the padding field */
    {
        if (u32EndToAdd + PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE > PVR_TS_LEN)
        {
            u32EndToAdd = PVR_TS_LEN - (PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE);
        }

        dataInLastTs = PVR_TS_LEN - (u32EndToAdd + PVR_TS_HEAD_SIZE + PVR_TS_MIN_PD_SIZE);

        memmove(pBufAddr + dataEnd  + u32EndToAdd - dataInLastTs,
            pBufAddr + dataEnd  - dataInLastTs,
            dataInLastTs);

        memset(pBufAddr + dataEnd  - dataInLastTs, 0xff, u32EndToAdd);
        pBufAddr[dataEnd  - (dataInLastTs + PVR_TS_MIN_PD_SIZE)] += (HI_U8)(u32EndToAdd);
    }
    else /* nonexistent padding field */
    {
        if (u32EndToAdd + PVR_TS_HEAD_SIZE > PVR_TS_LEN)
        {
            u32EndToAdd = PVR_TS_LEN - (PVR_TS_HEAD_SIZE);
        }

        dataInLastTs = PVR_TS_LEN - (u32EndToAdd + PVR_TS_HEAD_SIZE);

        memmove(pLastTsHead + PVR_TS_HEAD_SIZE + u32EndToAdd,
            pLastTsHead + PVR_TS_HEAD_SIZE,
            dataInLastTs);

        memset(pLastTsHead + PVR_TS_HEAD_SIZE, 0xff, u32EndToAdd);
        pLastTsHead[PVR_TS_PD_SIZE_POS] = (HI_U8)(u32EndToAdd - 1);
        pLastTsHead[PVR_TS_PD_FLAG_POS] = 0;
    }
#endif
    return ;
}
#endif

STATIC HI_S32 PVRPlayProcCipherDecrypt(PVR_PLAY_CHN_S *pChnAttr, HI_U32 u32SrcPhyAddr, HI_U32 u32DestPhyAddr, HI_U32 u32ByteLength)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 Len = 0;
    HI_U32 i = 0;
    HI_U32 TotalLen = u32ByteLength;

    if (!pChnAttr->stUserCfg.stDecryptCfg.bDoCipher)
    {
        return HI_SUCCESS;
    }

    while (TotalLen > 0)
    {
        Len = (TotalLen >= PVR_CIPHER_MAX_LEN)?PVR_CIPHER_MAX_LEN:TotalLen;
        TotalLen -= Len;

        ret = HI_UNF_CIPHER_Decrypt(pChnAttr->hCipher, 
                                    u32SrcPhyAddr + i*PVR_CIPHER_MAX_LEN, 
                                    u32DestPhyAddr+ i*PVR_CIPHER_MAX_LEN, 
                                    Len);
        if (ret != HI_SUCCESS)
        {
            HI_ERR_PVR("HI_UNF_CIPHER_Decrypt failed:%#x!, u32SrcPhyAddr:0x%x,u32DestPhyAddr:0x%x,Len:0x%x\n",ret, u32SrcPhyAddr,u32DestPhyAddr,Len);
            return ret;
        }

        i++;
        if (i > 20)
        {
            HI_ERR_PVR("data len to be decrypt is too large(> 20M), Must be error!\n");
            return HI_FAILURE;
        }
        
    }

    return HI_SUCCESS;
}
STATIC HI_S32 PVRPlayProcExtCallBack(PVR_PLAY_CHN_S *pChnAttr,
                                     HI_UNF_STREAM_BUF_S* pDemuxBuf,
                                     HI_U32 u32PhyAddr,
                                     HI_U64 u64ReadOffset, 
                                     HI_U64 u64GlobalOffset,
                                     HI_U32 u32BytesSend)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32   u32EndFrm;
    HI_UNF_PVR_DATA_ATTR_S stDataAttr;
    PVR_INDEX_ENTRY_S stStartFrame;
    PVR_INDEX_ENTRY_S stEndFrame;

    memset(&stStartFrame, 0, sizeof(PVR_INDEX_ENTRY_S));
    memset(&stEndFrame, 0, sizeof(PVR_INDEX_ENTRY_S));

    s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle,&stStartFrame,pChnAttr->IndexHandle->stCycMgr.u32StartFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get Start Frame failed,ret=%d\n",s32Ret);
        stStartFrame.u64Offset = 0;
    }

    if (pChnAttr->IndexHandle->stCycMgr.u32EndFrame > pChnAttr->IndexHandle->stCycMgr.u32StartFrame)
    {
        u32EndFrm = pChnAttr->IndexHandle->stCycMgr.u32EndFrame - 1;
    }
    else
    {
        u32EndFrm = pChnAttr->IndexHandle->stCycMgr.u32LastFrame - 1;
    }

    s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle,&stEndFrame,u32EndFrm);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get End Frame failed,ret=%d\n",s32Ret);
        stEndFrame.u64Offset = 0;
    }

    stDataAttr.u32ChnID        = pChnAttr->u32chnID;
    stDataAttr.u64FileStartPos = stStartFrame.u64Offset;
    stDataAttr.u64FileEndPos   = stEndFrame.u64Offset;
    stDataAttr.u64GlobalOffset = u64GlobalOffset;
    stDataAttr.u64FileReadOffset = u64ReadOffset;

    memset(stDataAttr.CurFileName, 0, sizeof(stDataAttr.CurFileName));
    PVRFileGetOffsetFName(pChnAttr->s32DataFile, u64ReadOffset, stDataAttr.CurFileName);
    PVR_Index_GetIdxFileName(stDataAttr.IdxFileName, pChnAttr->stUserCfg.szFileName);

    s32Ret = pChnAttr->readCallBack(&stDataAttr, pDemuxBuf->pu8Data, u32PhyAddr, (HI_U32)u64ReadOffset, (HI_U32)u32BytesSend);
    if(HI_SUCCESS!=s32Ret)
    {
        HI_ERR_PVR("readCallBack failed,ret=%d\n",s32Ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : PVRPlaySendData
 Description     : by TS packet align mode, send pointed size data to demux, and the data must be cotinuious and valid
 Input           : pChnAttr     **the attribute of channel
                   offSet       ** the data offset from start in ts
                   bytesToSend  ** the data size
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/23
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
/*
PVR_CIPHER_PKG_LEN aligned, read file from here        PVR_CIPHER_PKG_LEN aligned, read file to here
|                                                                             |
|     188 aligned        offSet(picture header,StartCode)    188 aligned                                        |
|              |             |                                  |             |
V              V             V                                  V             V
-------------------------------------------------------------------------------
| xxx          | TS head |xx |       valid data    |    xxx     |0x47...      |
-------------------------------------------------------------------------------
|<-CipherHead->|<-HeadToAdd->|<---bytesToSend----->|<-EndToAdd->|<-CipherEnd->|
|<---------------------------AlignSize--------------------------------------->|
*/

/* send new command, when break or execute error, return failure, otherwise, return success.
   u32BytesSend must be aligned by 512 byte */
STATIC INLINE HI_S32 PVRPlaySendToTsBuffer(PVR_PLAY_CHN_S *pChnAttr, HI_U64 u64ReadOffset, HI_U64 u64GlobalOffset,
                                           HI_U32 u32BytesSend, HI_U32 u32CipherHead, HI_U32 u32CipherEnd, 
                                           HI_U32 u32HeadToAdd, HI_U32 u32EndToAdd)
{
    HI_S32   ret;
    HI_U32   u32BytesRealSend = u32BytesSend;
    HI_U32   u32StartPos = 0;
    HI_U32   u32PhyAddr = 0;
    ssize_t  u32ReadedBytes = 0;
    HI_UNF_STREAM_BUF_S stDemuxBuf;
    HI_U64 u64GlobalOffsetAlign = (u64GlobalOffset - u32HeadToAdd) - u32CipherHead;
    
    /*find out ts buffer size u32ReadOnce*/
    ret = HI_UNF_DMX_GetTSBufferEx(pChnAttr->hTsBuffer, u32BytesSend, &stDemuxBuf, &u32PhyAddr, 0);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("HI_UNF_DMX_GetTSBufferEx failed, ret = 0x%x\n",ret);
        return HI_ERR_PVR_PLAY_INVALID_TSBUFFER;
    }

    /* read ts to buffer */
    u32ReadedBytes = PVRPlayReadData(pChnAttr, stDemuxBuf.pu8Data, u32BytesSend, 
                                     pChnAttr->s32DataFile, u64ReadOffset, u64GlobalOffset,
                                     u32CipherHead, u32HeadToAdd);
    if (u32ReadedBytes != u32BytesSend)
    {
        if (-1 == u32ReadedBytes)
        {
            HI_ERR_PVR("Read ts file fail, offset=%#llx size=%#x readed=%#x, errno=%#x\n", 
                        u64ReadOffset, u32BytesSend, u32ReadedBytes, errno);
            return HI_FAILURE;
        }

        if (0 == u32ReadedBytes)
        {
            HI_WARN_PVR("read 0,  offset=%#llx size=%#x readed=%#x, errno=%#x\n", 
                        u64ReadOffset, u32BytesSend, u32ReadedBytes, errno);
            return HI_ERR_PVR_FILE_TILL_END;
        }

        if (HI_ERR_PVR_PLAY_INDEX_BEYOND_TS == u32ReadedBytes)
        {
            return HI_ERR_PVR_PLAY_INDEX_BEYOND_TS;
        }

        /*otherwise, maybe reach the rewind file end*/
        HI_WARN_PVR("Reach file end, offset=%#llx size=%#x readed=%#x\n", u64ReadOffset, u32BytesSend, u32ReadedBytes);
    }

    /* if registed external read callback function, decrypt it */
    if ((NULL != pChnAttr->readCallBack) && (HI_UNF_PVR_STREAM_TYPE_ALL_TS != pChnAttr->stUserCfg.enStreamType))
    {
        if(HI_SUCCESS != PVRPlayProcExtCallBack(pChnAttr, &stDemuxBuf,u32PhyAddr, u64ReadOffset, u64GlobalOffsetAlign, u32BytesSend))
        {
            HI_ERR_PVR("Call external read callback function fail!\n");
            return HI_FAILURE;
        }
    }

    /*if need to decrypt, decrypt it */
    if (HI_SUCCESS != PVRPlayProcCipherDecrypt(pChnAttr, u32PhyAddr, u32PhyAddr, u32BytesSend))
    {
        return ret;
    }
    /* none header or end, send u32BytesSend size data to ts buffer */
    if ((HI_UNF_PVR_PLAY_STATE_PLAY != pChnAttr->enState)
       &&  (HI_UNF_PVR_PLAY_STATE_SF != pChnAttr->enState)
       &&  (HI_UNF_PVR_PLAY_STATE_STEPF != pChnAttr->enState))
    {
#ifndef HI_PVR_SOFT_INDEX_SUPPORT           
        PVRPlaySetTsHead(stDemuxBuf.pu8Data + u32CipherHead, u32HeadToAdd);
#endif
        u32BytesRealSend -= u32CipherHead;
        u32StartPos = u32CipherHead;
#ifndef HI_PVR_SOFT_INDEX_SUPPORT           
        PVRPlayAddTsEnd(stDemuxBuf.pu8Data, u32BytesSend - u32CipherEnd - u32EndToAdd, u32EndToAdd);
#endif
        u32BytesRealSend -= u32CipherEnd;

    }
    else /* both normal play and slow play, send all stream,  no longer patch stream, just depart it at PVR_TS_LEN size times */
    {
        u32BytesRealSend -= u32CipherHead;
        u32StartPos = u32CipherHead;

        if (u32EndToAdd != 0)
        {
            u32BytesRealSend -= (u32CipherEnd + PVR_TS_LEN);
        }
        else
        {
            u32BytesRealSend -= u32CipherEnd;
        }
    }

    ret = HI_UNF_DMX_PutTSBufferEx(pChnAttr->hTsBuffer, u32BytesRealSend, u32StartPos);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_DMX_PutTSBufferEx failed:%#x!\n", ret);
        return HI_FAILURE;
    }
    else
    {
        #ifdef DMUP_FILE_DATA
        if (g_pvrfpSend)
        {
            fwrite(stDemuxBuf.pu8Data + u32StartPos, 1, u32BytesRealSend, g_pvrfpSend);
        }
        #endif
    }

    return HI_SUCCESS;
}



STATIC INLINE HI_S32 PVRPlaySendData(PVR_PLAY_CHN_S *pChnAttr, HI_U64 u64OffSet, HI_U32 u32BytesToSend, HI_U64 u64GlobalOffset)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32AlignSize = 0;
    HI_U32 u32HeadToAdd = 0;  /* distance from data start to TS header */
    HI_U32 u32EndToAdd = 0;   /* distance from data end to TS end */
    HI_U32 u32CipherHead = 0; /* distance from start ts header forward to the closest cipher group(meanwhile, imply read position of O_DIRECT, too) */
    HI_U32 u32CipherEnd = 0;  /* distance data end backward to the closest cipher group(meanwhile, imply read position of O_DIRECT, too) */
    HI_U64 u64ReadOffset = 0; /* read offset address of ts file*/
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    u32HeadToAdd = 0;
    u32EndToAdd = 0;
    u32CipherHead = 0;
    u32CipherEnd = 0;    
#else
    u32HeadToAdd = (HI_U32)(u64GlobalOffset % PVR_TS_LEN);
    if (0 != ((u64GlobalOffset + u32BytesToSend) % PVR_TS_LEN))
    {
        u32EndToAdd = PVR_TS_LEN - (HI_U32)((u64GlobalOffset + u32BytesToSend) % PVR_TS_LEN);
    }

    u32CipherHead = (HI_U32)((u64GlobalOffset - u32HeadToAdd)  % PVR_CIPHER_PKG_LEN);
    u32CipherEnd = PVR_CIPHER_PKG_LEN - (HI_U32)((u64GlobalOffset + u32BytesToSend + u32EndToAdd) % PVR_CIPHER_PKG_LEN);
#endif
    u32AlignSize = u32BytesToSend + u32HeadToAdd + u32EndToAdd + u32CipherHead + u32CipherEnd;
    u64ReadOffset = ((u64OffSet - u32HeadToAdd) - u32CipherHead);

    //HI_INFO_PVR("%u = %u + %u + %u + %u + %u\n", u32AlignSize ,u32BytesToSend , u32HeadToAdd, u32EndToAdd , u32CipherHead , u32CipherEnd);
    ret = PVRPlaySendToTsBuffer(pChnAttr, u64ReadOffset, u64GlobalOffset, u32AlignSize, u32CipherHead, u32CipherEnd, u32HeadToAdd, u32EndToAdd);
    return ret;
}

STATIC HI_S32 PVRPlaySendPrivatePacketToTsBuffer(PVR_PLAY_CHN_S *pChnAttr, HI_U32 u32DisPlayTime, HI_U32 u32Pid)
{
    HI_S32 ret;
    HI_U32 u32Bytes2Send = PVR_TS_LEN;
    HI_U32 u32PhyAddr;
    HI_UNF_STREAM_BUF_S DataBuf;
    HI_U8 u8PesPacket[41] = {0x00, 0x00, 0x01, 0xee, 0x00, 0x00, 0x80, 0x00, 0x00, 
                             0x00, 0x00, 0x01, 0x1E, 0x70, 0x76, 0x72, 0x63, 0x75, 0x72, 0x74, 0x6d, 
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    ret = HI_UNF_DMX_GetTSBufferEx(pChnAttr->hTsBuffer, u32Bytes2Send, &DataBuf, &u32PhyAddr, 0);
    if (HI_SUCCESS != ret)
    {
        if ((HI_ERR_DMX_NOAVAILABLE_BUF == ret) || (HI_ERR_DMX_TIMEOUT == ret))
        {
            while (HI_SUCCESS != ret)
            {
                if (pChnAttr->bPlayMainThreadStop)
                {
                    return HI_FAILURE;
                }

                if ((HI_ERR_DMX_NOAVAILABLE_BUF == ret) || (HI_ERR_DMX_TIMEOUT == ret))
                {
                    usleep(40000);
                    if (pChnAttr->bPlayMainThreadStop)
                    {
                        return HI_FAILURE;
                    }

                    ret = HI_UNF_DMX_GetTSBufferEx(pChnAttr->hTsBuffer, u32Bytes2Send, &DataBuf, &u32PhyAddr, 0);
                }
                else
                {
                    HI_ERR_PVR("HI_UNF_DMX_GetTSBufferEx failed:%#x!\n", ret);
                    return HI_FAILURE;
                }
            }
        }
        else
        {
            HI_ERR_PVR("HI_UNF_DMX_GetTSBufferEx failed:%#x!\n", ret);
            return HI_FAILURE;
        }
    }

    memset(DataBuf.pu8Data, 0xff, DataBuf.u32Size);
    DataBuf.pu8Data[0] = 0x47;
    DataBuf.pu8Data[1] = (HI_U8)(((u32Pid & 0x1f00) >> 8) | 0x40);
    DataBuf.pu8Data[2] = (HI_U8)(u32Pid & 0xff);
    DataBuf.pu8Data[3] = 0x10;
    memcpy((void *)((HI_U32)u8PesPacket + 21), &u32DisPlayTime, sizeof(u32DisPlayTime));
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    memset((void *)((HI_U32)u8PesPacket + 25), 0x00, sizeof(PVR_FRAME_TAG_S));
#else
    memcpy((void *)((HI_U32)u8PesPacket + 25), &(pChnAttr->stFBAttr.stBKInfo.stFrmTag), sizeof(PVR_FRAME_TAG_S));
#endif
    memcpy((void *)((HI_U32)DataBuf.pu8Data+4), u8PesPacket, sizeof(u8PesPacket));
    ret = HI_UNF_DMX_PutTSBufferEx(pChnAttr->hTsBuffer, u32Bytes2Send, 0);
    if (ret != HI_SUCCESS)
    {
        HI_ERR_PVR("HI_UNF_DMX_PutTSBufferEx failed:%#x!\n", ret);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/*****************************************************************************
 Prototype       : PVRPlaySendAframe
 Description     : send one frame data to demux
 Input           : pChnAttr     **the attribute of play channel
                   pframe       ** frame index info
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 PVRPlaySendAframe(PVR_PLAY_CHN_S *pChnAttr, const PVR_INDEX_ENTRY_S *pframe)
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32VidPid = 0x1fff;
    HI_U32 u32DmxFreeTsBufSize = 0;
    HI_UNF_DMX_TSBUF_STATUS_S stDmxTsBufState;

    HI_ASSERT_RET(NULL != pframe);
    HI_ASSERT_RET(NULL != pChnAttr);

    pChnAttr->bInterPauseForTimeshift = HI_FALSE;
    
    if (0 == pframe->u32FrameSize) /* the abnormal, index file content is zero, AI7D02622 */
    {
        return HI_SUCCESS;
    }
    
    if (pframe->u32FrameSize > PVR_PLAY_MAX_FRAME_SIZE)
    {
        HI_WARN_PVR("Frame size too large, drop it(Size:%u, offset=%llu).\n",
                    pframe->u32FrameSize, pframe->u64Offset);
        return HI_SUCCESS;
    }

    ret = HI_UNF_DMX_GetTSBufferStatus(pChnAttr->hTsBuffer, &stDmxTsBufState);
    if(HI_SUCCESS != ret)
    {
        HI_ERR_PVR("Get demux ts buffer status fail.ret=%#x\n",ret);
        return HI_ERR_PVR_PLAY_INVALID_TSBUFFER;
    }

    u32DmxFreeTsBufSize = stDmxTsBufState.u32BufSize - stDmxTsBufState.u32UsedSize;

    if (u32DmxFreeTsBufSize < 2*(PVR_CIPHER_PKG_LEN + PVR_TS_LEN) + pframe->u32FrameSize + PVR_TS_LEN)
    {
        pChnAttr->bNotAvailableTsBuff = HI_TRUE;
        return HI_ERR_PVR_PLAY_INVALID_TSBUFFER;
    }

    pChnAttr->bNotAvailableTsBuff = HI_FALSE;

    memcpy(&(pChnAttr->IndexHandle->stCurPlayFrame), pframe, sizeof(PVR_INDEX_ENTRY_S));

    if ((pChnAttr->u32CurPlayTimeMs > pframe->u32DisplayTimeMs) && (pChnAttr->enSpeed > 0))
    {
        pChnAttr->u32CurPlayTimeMs = pframe->u32DisplayTimeMs;
    }

    HI_INFO_PVR("Frame to send: FrmNo/FrmType/Offset/Size/DispTime(ms):%d  %d %llu, %u, %u.\n",
                pChnAttr->IndexHandle->u32ReadFrame,
                ((pframe->u16FrameTypeAndGop >> 14) & 0x3),
                pframe->u64Offset, 
                pframe->u32FrameSize, 
                pframe->u32DisplayTimeMs);
   
    /* At present, only video support disptime TS packet */
    if ( pChnAttr->bRecordedVideoExist == HI_TRUE)
    {
        if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &u32VidPid))
        {
            if ((0x1fff != u32VidPid) && (HI_FALSE == pChnAttr->bNotAvailableTsBuff))
            {
                ret = PVRPlaySendPrivatePacketToTsBuffer(pChnAttr, pframe->u32DisplayTimeMs, u32VidPid);
                if (HI_SUCCESS != ret)  
                {
                    HI_WARN_PVR("Send playing time packet fail. ret=%#x\n",ret);
                    return ret;
                }
            }
        }
        else
        {
            HI_ERR_PVR("Get Stream Vpid fail!\n");
        }
    }
    
    ret = PVRPlaySendData(pChnAttr, pframe->u64Offset, pframe->u32FrameSize, pframe->u64GlobalOffset);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("Send the rewind data fail! ret=%#x\n", ret);
    }

    if (HI_ERR_PVR_PLAY_INDEX_BEYOND_TS == ret)
    {
        pChnAttr->bIndexBeyondTs = HI_TRUE;
    }
    else
    {
        pChnAttr->bIndexBeyondTs = HI_FALSE;
    }

    return ret;
}

/*****************************************************************************
 Prototype       : PVRPlayCheckError
 Description     : Check return value, if not success, trigger callback event
 Input           : pChnAttr  **
                   ret     **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/5/26
    Author       : q46153
    Modification : Created function

*****************************************************************************/
STATIC INLINE HI_VOID PVRPlayCheckError(const PVR_PLAY_CHN_S  *pChnAttr,  HI_S32 ret)
{
    if (HI_SUCCESS == ret)
    {
        return;
    }

    /* after pause, stop play, notice error play event, AI7D02621 */
    if (HI_ERR_DMX_NOAVAILABLE_BUF == ret
        || HI_ERR_DMX_NOAVAILABLE_DATA == ret)
    {
        return;
    }

    HI_INFO_PVR("====callback occured, ret=0x%x\n", ret);
    switch (ret)
    {
        case HI_ERR_PVR_FILE_TILL_END:
            if (pChnAttr->IndexHandle->bIsRec)
            {
                PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_REACH_REC, 0);
                pChnAttr->IndexHandle->u32TimeShiftTillEndCnt = 0;
            }
            else
            {
                PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_EOF, 0);
                pChnAttr->IndexHandle->u32TimeShiftTillEndCnt = 0;
            }
            break;
        case HI_ERR_PVR_FILE_TILL_START:
            //if (!pChnAttr->IndexHandle->bIsRec)
            {
                PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_SOF, 0);
            }
            break;
        default:
            PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_ERROR, ret);
    }

    return;
}

HI_VOID PVRPlaySeektoStartFrame(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_U32 u32ReadFrm;
    HI_S32 s32Ret = HI_SUCCESS;

    if ((HI_TRUE == pChnAttr->stStartSeekInfo.bSeek)
        ||(0 != pChnAttr->IndexHandle->u64PauseOffset)
            &&(PVR_INDEX_PAUSE_INVALID_OFFSET != pChnAttr->IndexHandle->u64PauseOffset))
    {
        if(HI_TRUE == pChnAttr->stStartSeekInfo.bSeek)
        {
            s32Ret = PVR_Index_SeekByTime(pChnAttr->IndexHandle, pChnAttr->stStartSeekInfo.s64Offset,
                                            pChnAttr->stStartSeekInfo.s32Whence,0);
           HI_WARN_PVR("seek to the position(whence, offset:%d, %lld), ret = 0x%08x\n",pChnAttr->stStartSeekInfo.s32Whence, pChnAttr->stStartSeekInfo.s64Offset, s32Ret);
        }
        else
        {
            s32Ret = PVR_Index_SeekToPauseOrStart(pChnAttr->IndexHandle);
        }

        if(HI_SUCCESS == s32Ret)
        {
            u32ReadFrm = PVR_Index_GetCurReadFrameNum(pChnAttr->IndexHandle);
            if (u32ReadFrm == -1)
            {
                HI_ERR_PVR("Get cur read frame invalide\n");
                return;
            }

            s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &(pChnAttr->IndexHandle->stCurPlayFrame), u32ReadFrm);
            if((HI_ERR_PVR_FILE_TILL_END == s32Ret) || 
               (pChnAttr->IndexHandle->stCurPlayFrame.u64GlobalOffset < pChnAttr->IndexHandle->u64FileSizeGlobal))
            {
                u32ReadFrm = PVR_Index_GetPrevFrameNum(u32ReadFrm, &(pChnAttr->IndexHandle->stCycMgr));
                s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &(pChnAttr->IndexHandle->stCurPlayFrame), u32ReadFrm);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_PVR("get the %d entry fail.\n", u32ReadFrm);
                }
                (HI_VOID)PVR_Index_SetCurReadFrameNum(pChnAttr->IndexHandle, u32ReadFrm);
            }
            pChnAttr->bTimeShiftStartFlg = HI_TRUE;
            pChnAttr->u32CurPlayTimeMs = pChnAttr->IndexHandle->stCurPlayFrame.u32DisplayTimeMs;
        }
    }
    else
    {
        if (HI_SUCCESS != PVR_Index_SeekToStart(pChnAttr->IndexHandle))
        {
            HI_ERR_PVR("seek to start frame failed!\n");
        }
    }
}
static HI_U32 PVRPlayGetCurPlayTime(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32CurPlayTime = 0;
    HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S stAvplayPrivInfo;
    
    if (pChnAttr->bRecordedVideoExist == HI_TRUE)
    {
        memset(&stAvplayPrivInfo, 0 ,sizeof(HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S));
        s32Ret = HI_UNF_AVPLAY_Invoke(pChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO, (void *)&stAvplayPrivInfo);
        if ((HI_SUCCESS != s32Ret) || (-1 == stAvplayPrivInfo.u32LastPlayTime))
        {
            u32CurPlayTime = pChnAttr->u32CurPlayTimeMs;   
            HI_WARN_PVR("Can't get current play frame time, use default time 0!  Ret = 0x%x; u32LastPlayTime = %u\n", s32Ret, stAvplayPrivInfo.u32LastPlayTime);
        }
        else
        {
            u32CurPlayTime = stAvplayPrivInfo.u32LastPlayTime;
        }
    }
    else
    {
        u32CurPlayTime = pChnAttr->u32CurPlayTimeMs;
    }

    return u32CurPlayTime;
}

/* seek the read pointer of index to the output one frame, so that, seek or play state can get the correct reference position*/
STATIC INLINE HI_S32 PVRPlaySeekToCurFrame(PVR_INDEX_HANDLE handle, 
                                           PVR_PLAY_CHN_S  *pChnAttr,
                                           HI_UNF_PVR_PLAY_STATE_E enCurState)
{
    HI_S32 ret;
    HI_U32 u32SeekToTime;

    if (HI_UNF_PVR_PLAY_STATE_INVALID == pChnAttr->enState)
    {
        HI_ERR_PVR("Can not seek to current play frame when state is invalid!\n");
        return HI_FAILURE;
    }

    if (HI_UNF_PVR_PLAY_STATE_STEPF == pChnAttr->enState)
    {
        PVRPlaySyncTrickPlayTime(pChnAttr);
    }

    /* if bEndOfFile is set, no need to seek to current frame. */
    if (pChnAttr->bEndOfFile)
    {
        return HI_SUCCESS;
    }

    u32SeekToTime = PVRPlayGetCurPlayTime(pChnAttr);

    if (HI_TRUE == pChnAttr->bTimeShiftStartFlg)
    {
        if (1000 >= abs(pChnAttr->u32CurPlayTimeMs - u32SeekToTime))
        {
            pChnAttr->bTimeShiftStartFlg= HI_FALSE;
        }
        else
        {
            u32SeekToTime = pChnAttr->u32CurPlayTimeMs;
        }
    }

    ret = PVR_Index_SeekToTime(handle, u32SeekToTime);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("PVR_Index_SeekToTime not found the time failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* reset buffer and player, seek ts position to the current play frame, if that frame invalid, mean to reset it already, the decoder no longer reset it */
STATIC INLINE HI_S32 PVRPlayResetToCurFrame(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 ret = HI_SUCCESS;
    PVR_INDEX_ENTRY_S stCurIndex = {0};

    ret = PVRPlaySeekToCurFrame(pChnAttr->IndexHandle, pChnAttr, pChnAttr->enState);
    if (HI_SUCCESS == ret)
    {
        HI_INFO_PVR("to reset buffer and player.\n");
        PVR_Index_ChangePlayMode(pChnAttr->IndexHandle);

        pChnAttr->bNotAvailableTsBuff = HI_FALSE;

        ret = HI_UNF_DMX_ResetTSBuffer(pChnAttr->hTsBuffer);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("ts buffer reset failed! ret=%#x\n", ret);
            return ret;
        }

        ret = HI_UNF_AVPLAY_Reset(pChnAttr->hAvplay, NULL);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("AVPLAY reset failed! ret=%#x\n", ret);
            return ret;
        }

        ret = HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, HI_UNF_VCODEC_MODE_NORMAL);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("AVPLAY set decode mode to normal fail! ret=%#x\n", ret);
            return ret;
        }

        /* DTS2015073107898 */
        if (HI_SUCCESS != PVR_Index_GetCurrentFrame(pChnAttr->IndexHandle, &stCurIndex))
        {
            pChnAttr->u32CurPlayTimeMs = PVRPlayGetCurPlayTime(pChnAttr);
        }
        else
        {
            pChnAttr->u32CurPlayTimeMs = stCurIndex.u32DisplayTimeMs;
        }
    }
    pChnAttr->IndexHandle->u32TimeShiftTillEndCnt = 0;

    return HI_SUCCESS;
}


STATIC INLINE HI_S32 PVRPlayDealWithRecReachPlay(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_S32 ret;

    if (pChnAttr->IndexHandle->bIsRec == HI_FALSE)
    {
        return HI_SUCCESS;
    }

    switch (pChnAttr->enState)
    {
    case HI_UNF_PVR_PLAY_STATE_PLAY:
    case HI_UNF_PVR_PLAY_STATE_FF:
        if (PVR_Index_CheckSetRecReachPlay(pChnAttr->IndexHandle, 0))
        {
            (HI_VOID)PVR_Index_SeekToStart(pChnAttr->IndexHandle);
            ret = HI_UNF_DMX_ResetTSBuffer(pChnAttr->hTsBuffer);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("ts buffer reset failed!\n");
                return ret;
            }

            ret = HI_UNF_AVPLAY_Reset(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("AVPLAY reset failed!\n");
                return ret;
            }
        }
        break;
    case HI_UNF_PVR_PLAY_STATE_FB:
        if (PVR_Index_CheckSetRecReachPlay(pChnAttr->IndexHandle, PVR_TPLAY_MIN_DISTANCE))
        {
            return HI_ERR_PVR_FILE_TILL_START;
        }
        break;
    case HI_UNF_PVR_PLAY_STATE_PAUSE:
    case HI_UNF_PVR_PLAY_STATE_SF:
    case HI_UNF_PVR_PLAY_STATE_STEPF:
    case HI_UNF_PVR_PLAY_STATE_STEPB:
        if (PVR_Index_CheckSetRecReachPlay(pChnAttr->IndexHandle, 0))
        {
            (HI_VOID)PVR_Index_SeekToStart(pChnAttr->IndexHandle);
            ret = HI_UNF_DMX_ResetTSBuffer(pChnAttr->hTsBuffer);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("ts buffer reset failed!\n");
                return ret;
            }

            ret = HI_UNF_AVPLAY_Reset(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("AVPLAY reset failed!\n");
                return ret;
            }

            PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_REC_REACH_PLAY, 0);
        }
        break;
    default:
        HI_ERR_PVR("Invalide state %d\n", pChnAttr->enState);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#if 0 /*obsolete APIs*/
STATIC INLINE HI_S32 PVRPlayIsChnTplay(HI_U32 u32Chn)
{
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_PLAY_CHECK_INIT(&g_stPlayInit);

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);

    if ( pChnAttr->enState > HI_UNF_PVR_PLAY_STATE_PAUSE
        && pChnAttr->enState < HI_UNF_PVR_PLAY_STATE_STOP)
    {
        return HI_SUCCESS;
    }
    else
    {
        return HI_ERR_PVR_PLAY_INVALID_STATE;
    }
}
#endif
/*
check if the frame to play is saved to ts file
*/
STATIC INLINE HI_BOOL PVRPlayIsTsSaved(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pFrameToPlay)
{
    HI_U64 u64CurFramPosition = 0;

    if (pChnAttr->IndexHandle->bIsRec)
    {
        /*judge the read content is avalible or not when time shift, 
          read length need to add some additional length for alignment reading*/
        u64CurFramPosition = ((HI_U64)pFrameToPlay->u64GlobalOffset + 
                             (HI_U64)pFrameToPlay->u32FrameSize + 
                             (HI_U64)(2*(PVR_CIPHER_PKG_LEN + PVR_TS_LEN)+ PVR_TS_LEN));
        if (pChnAttr->IndexHandle->u64FileSizeGlobal >= u64CurFramPosition)
        {
            return HI_TRUE;
        }
#ifdef HI_PVR_EXTRA_BUF_SUPPORT
        else if (0 != PVR_TS_CACHE_BUFFER_CheckCached(&pChnAttr->IndexHandle->stTsCacheBufInfo, u64CurFramPosition))
        {
            return HI_TRUE;
        }
#endif
        else
        {
            HI_WARN_PVR("Play Over Rec when timeshift: readPos=%#llx, indexPos=%#llx filePos=%#llx \n",
                          u64CurFramPosition,
                          pChnAttr->IndexHandle->u64GlobalOffset, 
                          pChnAttr->IndexHandle->u64FileSizeGlobal);
            return HI_FALSE;
        }
    }
    else
    {
        return HI_TRUE;
    }
}

void PVRPlaySyncTrickPlayTime(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_S32 ret = 0;
    HI_U32 u32CurFrmTimeMs = 0;
    HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S stAvplayPrivInfo = {0};

    if ( pChnAttr->bRecordedVideoExist == HI_FALSE)  
    {
        HI_WARN_PVR("Pure Audio stream not support Trick Play\n");
        return;
    }

    ret = HI_UNF_AVPLAY_Invoke(pChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO, &stAvplayPrivInfo);
    if ((HI_SUCCESS != ret) || (-1 == stAvplayPrivInfo.u32LastPlayTime))
    {
        HI_WARN_PVR("Get Avplay private info fail.ret = 0x%x, lastplaytime = %u\n", ret, stAvplayPrivInfo.u32LastPlayTime);
        return;
    }

    if (HI_TRUE == pChnAttr->bTimeShiftStartFlg)
    {
        u32CurFrmTimeMs = pChnAttr->u32CurPlayTimeMs;
    }
    else
    {
        u32CurFrmTimeMs = stAvplayPrivInfo.u32LastPlayTime;
    }
                        
    if(u32CurFrmTimeMs > pChnAttr->u32CurPlayTimeMs)
    {
        if ((u32CurFrmTimeMs - pChnAttr->u32CurPlayTimeMs)>2000)
        {
            pChnAttr->u32CurPlayTimeMs = u32CurFrmTimeMs;
        }
    }
    else if(u32CurFrmTimeMs < pChnAttr->u32CurPlayTimeMs)
    {
        if ((pChnAttr->u32CurPlayTimeMs - u32CurFrmTimeMs)>2000)
        {
            pChnAttr->u32CurPlayTimeMs = u32CurFrmTimeMs;
        }
    }
}
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
static INLINE void PVRPlayAnalysisStream(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_U32 u32VideoType = 0;
    HI_U32 u32PreReadCnt = 0x400;
    HI_U32 u32StartFrm=0, u32EndFrm=0, u32LastFrm=0, u32TotalFrm=0;
    HI_U32 i = 0;
    HI_U32 u32NalHeader;
    PVR_INDEX_ENTRY_S stEntry;

    memset(&stEntry,0 ,sizeof(PVR_INDEX_ENTRY_S));

    u32VideoType = PVR_Index_GetVtype(pChnAttr->IndexHandle);
    u32VideoType -= 100;

    if(HI_UNF_VCODEC_TYPE_H264 != u32VideoType)
    {
        return;
    }

    u32StartFrm = pChnAttr->IndexHandle->stCycMgr.u32StartFrame;
    u32EndFrm = pChnAttr->IndexHandle->stCycMgr.u32EndFrame;
    u32LastFrm = pChnAttr->IndexHandle->stCycMgr.u32LastFrame;

    if(u32StartFrm > u32EndFrm)
    {
    u32TotalFrm = u32EndFrm + u32LastFrm - u32StartFrm;
    }
    else
    {
        u32TotalFrm = u32LastFrm - u32StartFrm;
    }

    if (u32TotalFrm < u32PreReadCnt)
    {
        u32PreReadCnt = u32TotalFrm;
    }

    pChnAttr->stFBAttr.u32GopNumOfStart = (u32PreReadCnt << 16);
    for(i = u32StartFrm; i < (u32StartFrm + u32PreReadCnt); i++)
    {
        u32NalHeader = 0;
        
        if (HI_SUCCESS != PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stEntry, i))
        {
            HI_ERR_PVR("Get IndexEntry fail, Can't Analysis anymore.\n");
            return;
        }

        if (PVR_INDEX_is_Pframe(&stEntry))
        {
            continue;
        }
    
        if (sizeof(u32NalHeader) != PVR_PREAD64((HI_U8 *)&u32NalHeader, sizeof(u32NalHeader), pChnAttr->s32DataFile, stEntry.u64Offset))
        {
            HI_ERR_PVR("Get Stream ES head fail, Can't Analysis anymore.\n");
            return;
        }

        u32NalHeader = (u32NalHeader >> 24);

        if (PVR_INDEX_is_Iframe(&stEntry))
        {
            if(1 != pChnAttr->stFBAttr.stVdecCtrlInfo.u32IDRFlag)
            {
                if (5 == (u32NalHeader & 0x1f))
                {
                    pChnAttr->stFBAttr.stVdecCtrlInfo.u32IDRFlag = 1;
                    HI_WARN_PVR("The playing stream's I frame has IDR\n");
                }
            }

            pChnAttr->stFBAttr.u32GopNumOfStart++;
        }
        
        if ((PVR_INDEX_is_Bframe(&stEntry)) && (1 != pChnAttr->stFBAttr.stVdecCtrlInfo.u32BFrmRefFlag))
        {
            if (0 != ((u32NalHeader >> 5) & 3))
            {
                pChnAttr->stFBAttr.stVdecCtrlInfo.u32BFrmRefFlag = 1;
                HI_WARN_PVR("The playing stream's B frame can be reference\n");
            }
        }
        
        if((u32StartFrm > u32EndFrm) && ((u32StartFrm + u32PreReadCnt) > u32LastFrm) && (i == (u32LastFrm - 1)))
        {
            u32PreReadCnt = u32PreReadCnt - (u32LastFrm - u32StartFrm);
            i = 0;
            u32StartFrm = 0;
        }
    }
}
#endif

STATIC INLINE HI_S32 PVRPlayGetCurrentRecordEndTime(PVR_PLAY_CHN_S *pChnAttr, HI_U32 *pu32EndDisplayTime)
{
    PVR_INDEX_ENTRY_S stEndFrame;
    PVR_INDEX_ENTRY_S stStartFrame;
    HI_S32 s32Ret = 0;

    memset(&stStartFrame, 0, sizeof(stStartFrame));
    memset(&stEndFrame, 0, sizeof(stEndFrame));

    s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stEndFrame, pChnAttr->IndexHandle->stCycMgr.u32EndFrame);
    if (HI_SUCCESS != s32Ret)
    {
        if(HI_ERR_PVR_FILE_TILL_END == s32Ret)
        {
            if (HI_SUCCESS != PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stEndFrame, (pChnAttr->IndexHandle->stCycMgr.u32EndFrame - 1)))
            {
                HI_ERR_PVR("Can't get EndFrame:%d\n", (pChnAttr->IndexHandle->stCycMgr.u32EndFrame - 1));
                return s32Ret;
            }
        }
        else
        {
            HI_ERR_PVR("Can't get EndFrame:%d\n", (pChnAttr->IndexHandle->stCycMgr.u32EndFrame));
            return s32Ret;
        }
    }

    if (pChnAttr->IndexHandle->stCycMgr.u32EndFrame <= pChnAttr->IndexHandle->stCycMgr.u32StartFrame)
    {
        s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stStartFrame, pChnAttr->IndexHandle->stCycMgr.u32StartFrame);
        if ((HI_SUCCESS == s32Ret) 
            && (stEndFrame.u32DisplayTimeMs < stStartFrame.u32DisplayTimeMs) 
            && (0 < pChnAttr->IndexHandle->stCycMgr.u32EndFrame))
        {
            s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stEndFrame, (pChnAttr->IndexHandle->stCycMgr.u32EndFrame - 1));
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Can't get EndFrame:%d\n", (pChnAttr->IndexHandle->stCycMgr.u32EndFrame - 1));
                return s32Ret;
            }
        }
    }

    *pu32EndDisplayTime = stEndFrame.u32DisplayTimeMs;

    return HI_SUCCESS;;
}

STATIC INLINE HI_BOOL PVRPlayCheckTimeShiftBufferState(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32CurPlayTimeMs = 0;
    HI_U32 u32RecordEndTimeMs = 0;
    HI_U32 u32BufferTimeMs = 0;

    u32CurPlayTimeMs = PVRPlayGetCurPlayTime(pChnAttr);

    s32Ret = PVRPlayGetCurrentRecordEndTime(pChnAttr, &u32RecordEndTimeMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get record end time failed, ret = 0x%x\n", s32Ret);
        return HI_FALSE;
    }

    if (HI_UNF_PVR_PLAY_SPEED_NORMAL >= pChnAttr->enSpeed)
    {
        u32BufferTimeMs = pChnAttr->u32BufferTimeForTimeshift;
    }
    else
    {
        u32BufferTimeMs = (pChnAttr->u32BufferTimeForTimeshift + 5000) * pChnAttr->enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL;
    }
    HI_WARN_PVR("u32CurPlayTimeMs/u32BufferTimeMs/u32RecordEndTimeMs: %u/%u/%u\n",
        u32CurPlayTimeMs, u32BufferTimeMs, u32RecordEndTimeMs);
    if (u32CurPlayTimeMs + u32BufferTimeMs < u32RecordEndTimeMs)
    {
        HI_WARN_PVR("Buffer data more than %ums;current speed is: %u\n", u32BufferTimeMs, pChnAttr->enSpeed);
        return HI_TRUE;
    }

    return HI_FALSE;
}

STATIC INLINE HI_BOOL PVRPlayCheckTimeShiftForwardTillEnd(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_U32 u32CurFrmTimeMs = 0;
    HI_U32 u32RecordEndTimeMs = 0;
    HI_S32 s32Ret = 0;
    HI_U32 u32BufferTimeMs = 500;
    HI_BOOL bPlayReachRec = HI_FALSE;
    
    if ((!pChnAttr->IndexHandle->bIsRec) || (0 > pChnAttr->enSpeed))
    {
        return HI_FALSE;
    }

    pChnAttr->IndexHandle->u32TimeShiftTillEndCnt++;
    u32CurFrmTimeMs = PVRPlayGetCurPlayTime(pChnAttr);
    s32Ret = PVRPlayGetCurrentRecordEndTime(pChnAttr, &u32RecordEndTimeMs);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get record end time failed, ret = 0x%x\n", s32Ret);
        return HI_FALSE;
    }
#if 0 
    if (HI_UNF_PVR_PLAY_SPEED_NORMAL != pChnAttr->enSpeed)
    {
        u32BufferTimeMs = u32BufferTimeMs * pChnAttr->enSpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL + 1000;
    }
#else
    switch(pChnAttr->enSpeed)
    {
        case HI_UNF_PVR_PLAY_SPEED_2X_FAST_FORWARD:
            u32BufferTimeMs = 1500;
            break;
        case HI_UNF_PVR_PLAY_SPEED_4X_FAST_FORWARD:
            u32BufferTimeMs = 4000;
            break;
        case HI_UNF_PVR_PLAY_SPEED_8X_FAST_FORWARD:
            u32BufferTimeMs = 6000;
            break;
        case HI_UNF_PVR_PLAY_SPEED_16X_FAST_FORWARD:
            u32BufferTimeMs = 8000;
            break;
        case HI_UNF_PVR_PLAY_SPEED_32X_FAST_FORWARD:
            u32BufferTimeMs = 16000;
            break;
        case HI_UNF_PVR_PLAY_SPEED_64X_FAST_FORWARD:
            u32BufferTimeMs = 16000;
            break;
        default:
            break;
    }
#endif

#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    u32BufferTimeMs += 1000;
#endif
    bPlayReachRec = (u32CurFrmTimeMs + u32BufferTimeMs > u32RecordEndTimeMs) ? HI_TRUE : HI_FALSE;
    pChnAttr->bInterPauseForTimeshift = (u32CurFrmTimeMs + u32BufferTimeMs/2 > u32RecordEndTimeMs) ? HI_TRUE : HI_FALSE;

#ifndef HI_PVR_SOFT_INDEX_SUPPORT
    if (HI_UNF_PVR_PLAY_STATE_FF != pChnAttr->enState)
    {
        if (HI_TRUE == HI_PVR_SmoothCheckFFTillEnd(pChnAttr, u32BufferTimeMs/2))
            pChnAttr->bInterPauseForTimeshift = HI_TRUE;
        if (HI_TRUE == HI_PVR_SmoothCheckFFTillEnd(pChnAttr, u32BufferTimeMs))
            bPlayReachRec = HI_TRUE;
    }
#endif

    if (HI_TRUE == pChnAttr->bInterPauseForTimeshift)
    {
        bPlayReachRec = HI_TRUE;
        PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_BUFFER_START, pChnAttr->u32BufferTimeForTimeshift);
    }

    HI_WARN_PVR("u32CurPlayTimeMs/u32BufferTimeMs/u32RecordEndTimeMs/bPlayReachRec/bInterPauseForTimeshift: %u/%u/%u/%u/%u\n",
        u32CurFrmTimeMs, u32BufferTimeMs, u32RecordEndTimeMs, bPlayReachRec, pChnAttr->bInterPauseForTimeshift);
    return bPlayReachRec;
}

PVR_PLAY_CHN_S* PVRPlayGetChnAttrByName(const HI_CHAR *pFileName)
{
    HI_U32 i = 0;

    if(NULL == pFileName)
    {
        HI_ERR_PVR("File name point is NULL.\n");
        return NULL;
    }
    
    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        if  (!strncmp(g_stPvrPlayChns[i].stUserCfg.szFileName, pFileName, strlen(pFileName)) )
        {
            return (PVR_PLAY_CHN_S*)(&g_stPvrPlayChns[i]);
        }
    }

    return (PVR_PLAY_CHN_S*)NULL;
}
#ifndef PVR_ACQUIREDATAINDX
/*the following case, the record can catch up to the live stream */
HI_BOOL PVRPlayCheckSlowPauseBack(const HI_CHAR *pFileName)
{
    HI_U32 i;
    HI_UNF_PVR_PLAY_STATE_E enPlayStatus;

    if(NULL == pFileName)
    {
        HI_ERR_PVR("Input pointer parameter is NULL!\n");
        return HI_FALSE;
    }

    for (i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        if (!strncmp(g_stPvrPlayChns[i].stUserCfg.szFileName, pFileName,strlen(pFileName)))
        {
            break;
        }
    }

    if (i == PVR_PLAY_MAX_CHN_NUM)
    {
        HI_INFO_PVR("No PVRPlayChan Exist\n");
        return HI_FALSE;
    }

    enPlayStatus   = g_stPvrPlayChns[i].enState;

    /*two situation may cause rec catch play:
    1. paly pause or slow
    2. paly FB while rec rewind . add this situation for DTS2014052701682, 
    for this  situation,we suggest customer check playtime and start entry disptime 
    (inorder to wait the stream in demux/vdec/vfwm/vo buffer playing end), 
    if playtime meet the start entry disptime ,resume the play channel*/
    if ((enPlayStatus == HI_UNF_PVR_PLAY_STATE_PAUSE) 
        || (enPlayStatus == HI_UNF_PVR_PLAY_STATE_SF) 
        || (enPlayStatus == HI_UNF_PVR_PLAY_STATE_STEPB) 
        || (enPlayStatus == HI_UNF_PVR_PLAY_STATE_STEPF))
    {
        return HI_TRUE;
    }
    else if (enPlayStatus == HI_UNF_PVR_PLAY_STATE_FB) 
    {
        /*if alreadly rewind*/
        if ( g_stPvrPlayChns[i].IndexHandle->stCycMgr.u32StartFrame >= g_stPvrPlayChns[i].IndexHandle->stCycMgr.u32EndFrame )
        {
            return HI_TRUE;
        }
    }

    return HI_FALSE;
}
#endif
HI_BOOL PVRPlayCheckInitStat(void)
{
    return g_stPlayInit.bInit;
}
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
HI_S32 PVRPlayProcessNewVidFrame(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para)
{
    HI_U32 i = 0;
    
    PVR_PLAY_CHN_S *pChnAttr = (PVR_PLAY_CHN_S *)HI_NULL;
    if (HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME != enEvent)
    {
        HI_ERR_PVR("Invalid event, 0x%08x)\n", enEvent);
    }
    for(i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        pChnAttr = &g_stPvrPlayChns[i];

        if (hAvplay == pChnAttr->hAvplay)
        {
            if (HI_UNF_PVR_PLAY_STATE_FB != pChnAttr->enState)
            {
                continue;
            }
            PVR_LOCK(&(pChnAttr->stPlayChnMutex));
            pChnAttr->stTrickAttr.u32VdecCount++;
            pChnAttr->stTrickAttr.stFbAttr.bFBIFrameDecoded = HI_TRUE;
            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        }
    }

    return HI_SUCCESS;
}
#endif
HI_S32 PVRPlayProcessEos(HI_HANDLE hAvplay, HI_UNF_AVPLAY_EVENT_E enEvent, HI_U32 u32Para)
{
    HI_U32 i = 0;
    PVR_PLAY_CHN_S *pChnAttr = (PVR_PLAY_CHN_S *)HI_NULL;

    for(i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        pChnAttr = &g_stPvrPlayChns[i];

        if (hAvplay == pChnAttr->hAvplay)
        {
            PVR_LOCK(&(pChnAttr->stPlayChnMutex));

            pChnAttr->bEndOfFile = HI_TRUE;
            pChnAttr->bEofEvent = HI_TRUE;

            if (pChnAttr->bSeekEnd == HI_TRUE)
            {
                HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
                if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
                {
                    if (stSyncAttr.enSyncRef == HI_UNF_SYNC_REF_NONE)
                    {
                        stSyncAttr.enSyncRef = pChnAttr->enLastSyncState;
                        HI_ERR_PVR(" EOS enLastSyncState %d\n", pChnAttr->enLastSyncState);
                        if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
                        {
                            HI_ERR_PVR("Set sync attr failed!\n");
                        }
                    }
                }
                else
                {
                    HI_ERR_PVR("Get sync attr failed!\n");
                }
                pChnAttr->bSeekEnd = HI_FALSE;
            }
            (HI_VOID)HI_UNF_AVPLAY_Reset(pChnAttr->hAvplay, HI_NULL);
//            (HI_VOID)HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, HI_UNF_VCODEC_MODE_NORMAL);

            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        }
    }

    return HI_SUCCESS;
}

HI_S32 PVRPlaySetFrmRate(HI_HANDLE hAvplay,HI_UNF_AVPLAY_FRMRATE_TYPE_E enFrmRateType,HI_U32 u32fpsInteger,HI_U32 u32fpsDecimal)
{
    HI_S32 s32Ret = HI_SUCCESS;
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv320) || defined(CHIP_TYPE_hi3716mv330) || defined(CHIP_TYPE_hi3110ev500)
    HI_UNF_AVPLAY_FRAMERATE_PARAM_S stFrmRateAttr;
    stFrmRateAttr.stSetFrameRate.u32fpsInteger = u32fpsInteger;
    stFrmRateAttr.stSetFrameRate.u32fpsDecimal = u32fpsDecimal;
#else
    HI_UNF_AVPLAY_FRMRATE_PARAM_S   stFrmRateAttr;
    stFrmRateAttr.stSetFrmRate.u32fpsInteger = u32fpsInteger;
    stFrmRateAttr.stSetFrmRate.u32fpsDecimal = u32fpsDecimal;
#endif

    stFrmRateAttr.enFrmRateType = enFrmRateType; 
    s32Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrmRateAttr);
    if (HI_SUCCESS != s32Ret)   
    {
        HI_ERR_PVR("set frame to VO fail. u32fpsInteger:%d, u32fpsDecimal:%d\n",u32fpsInteger,u32fpsDecimal);
    }
    return s32Ret;
}

HI_S32 PVRPlaySetFrmRateAutoDetect(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if ( pChnAttr->bRecordedVideoExist == HI_TRUE)
    {
        return PVRPlaySetFrmRate(pChnAttr->hAvplay,HI_UNF_AVPLAY_FRMRATE_TYPE_PTS,0,0);
    }
    return s32Ret;
}

HI_S32 PVRPlaySetDecoderSpeed(PVR_PLAY_CHN_S  *pChnAttr, HI_UNF_PVR_PLAY_SPEED_E enPlaySpeed)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_TRUE == pChnAttr->bRecordedVideoExist)  
    {
#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv320) || defined(CHIP_TYPE_hi3716mv330) || defined(CHIP_TYPE_hi3110ev500)
        
        s32Ret = HI_MPI_AVPLAY_PlayTrickMode(pChnAttr->hAvplay, enPlaySpeed);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Set decoder speed failed! ret=%#x\n", s32Ret);
        }
#else
        HI_U32 u32DecoderSpeed = 0;
        HI_CODEC_VIDEO_CMD_S  stVdecCmdPara = {0};
        HI_UNF_AVPLAY_TPLAY_OPT_S stTPlayOpt = {0};
        stTPlayOpt.enTplayDirect = (enPlaySpeed >= 0) ? HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD : HI_UNF_AVPLAY_TPLAY_DIRECT_BACKWARD;
        if (abs(enPlaySpeed) > HI_UNF_PVR_PLAY_SPEED_NORMAL)
        {
            u32DecoderSpeed = abs(enPlaySpeed) / HI_UNF_PVR_PLAY_SPEED_NORMAL;
            stTPlayOpt.u32SpeedInteger = u32DecoderSpeed;
            stTPlayOpt.u32SpeedDecimal = 0;
        }
        else
        {
            u32DecoderSpeed = (abs(enPlaySpeed)*1000) / HI_UNF_PVR_PLAY_SPEED_NORMAL;
            stTPlayOpt.u32SpeedInteger = 0;
            stTPlayOpt.u32SpeedDecimal = u32DecoderSpeed;
        }
        stVdecCmdPara.u32CmdID = HI_UNF_AVPLAY_SET_TPLAY_PARA_CMD;
        stVdecCmdPara.pPara = &stTPlayOpt;

        s32Ret = HI_UNF_AVPLAY_Invoke(pChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_VCODEC, (void *)&stVdecCmdPara);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Set decoder speed failed! ret=%#x\n", s32Ret);
        }
#endif
    }

    return s32Ret;
}

HI_S32 PVRPlaySetPlayerSpeed(PVR_PLAY_CHN_S  *pChnAttr, HI_UNF_PVR_PLAY_SPEED_E enPlaySpeed)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32PlayerSpeed = 0;

#if defined(CHIP_TYPE_hi3716mv310) || defined(CHIP_TYPE_hi3716mv320) || defined(CHIP_TYPE_hi3716mv330) || defined(CHIP_TYPE_hi3110ev500)
    HI_HANDLE hWindow;    
    s32Ret = HI_MPI_AVPLAY_GetWindowHandle(pChnAttr->hAvplay, &hWindow);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("AVPLAY get window handle failed!\n");
        return HI_FAILURE;
    }

    if (abs(enPlaySpeed) >= HI_UNF_PVR_PLAY_SPEED_NORMAL)
    {
        u32PlayerSpeed = abs(enPlaySpeed)/HI_UNF_PVR_PLAY_SPEED_NORMAL * 256;
    }
    else
    {
        u32PlayerSpeed = 256 / (HI_UNF_PVR_PLAY_SPEED_NORMAL/abs(enPlaySpeed));
    }

    s32Ret = HI_UNF_VO_SetWindowRatio(hWindow, u32PlayerSpeed);//256 for normal speed
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("HI_UNF_VO_SetWindowRatio failed:%x!\n",s32Ret);
        return HI_FAILURE;
    }
#else
    HI_UNF_AVPLAY_TPLAY_OPT_S stTPlayOpt;
    stTPlayOpt.enTplayDirect = (enPlaySpeed >= 0) ? HI_UNF_AVPLAY_TPLAY_DIRECT_FORWARD : HI_UNF_AVPLAY_TPLAY_DIRECT_BACKWARD;

    if (abs(enPlaySpeed) >= HI_UNF_PVR_PLAY_SPEED_NORMAL)
    {
        u32PlayerSpeed = abs(enPlaySpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
        stTPlayOpt.u32SpeedInteger = u32PlayerSpeed;
        stTPlayOpt.u32SpeedDecimal = 0;
    }
    else
    {
        u32PlayerSpeed = (abs(enPlaySpeed) * 1000) / HI_UNF_PVR_PLAY_SPEED_NORMAL;
        stTPlayOpt.u32SpeedInteger = 0;
        stTPlayOpt.u32SpeedDecimal = u32PlayerSpeed;
    }
    if (HI_TRUE == pChnAttr->bRecordedVideoExist)  
    {
        s32Ret = HI_UNF_AVPLAY_Tplay(pChnAttr->hAvplay, &stTPlayOpt);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Set player speed failed! ret=%#x\n", s32Ret);
        }
    }
#endif

    return s32Ret;
}

/*****************************************************************************
 Prototype       : PVRPlayAudSyncCtrl
 Description     : Stop or start audio and sync
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2014/9/30
    Author       : z00111416
    Modification : Created function

*****************************************************************************/
HI_S32 PVRPlayAudSyncCtrl(PVR_PLAY_CHN_S  *pChnAttr, HI_BOOL bCtrlFlag)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32AudPid = 0x1fff;
    HI_UNF_SYNC_ATTR_S         stSyncAttr = {0};
    HI_UNF_AVPLAY_MEDIA_CHAN_E enMediaChn = HI_UNF_AVPLAY_MEDIA_CHAN_BUTT;

    s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &u32AudPid);
    if (HI_SUCCESS == s32Ret)
    {
        if (0x1fff != u32AudPid)
        {
            enMediaChn = HI_UNF_AVPLAY_MEDIA_CHAN_AUD;
        }
        else
        {
            return HI_SUCCESS;
        }
    }
    else
    {
        HI_ERR_PVR("Get audio pid fail! ret=%#x\n", s32Ret);
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Get avplay sync attr fail! ret=%#x\n", s32Ret);
        return s32Ret;
    }

    if (HI_FALSE == bCtrlFlag)
    {
        stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;

        s32Ret = HI_UNF_AVPLAY_SetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Set avplay sync attr fail! ret=%#x\n", s32Ret);
        }

        if (enMediaChn == HI_UNF_AVPLAY_MEDIA_CHAN_AUD)
        {
            s32Ret = HI_UNF_AVPLAY_Stop(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, (HI_UNF_AVPLAY_STOP_OPT_S *)HI_NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Stop avplay fail! ret=%#x\n", s32Ret);
            }
        }        
    }
    else
    {
        if (enMediaChn == HI_UNF_AVPLAY_MEDIA_CHAN_AUD)
        {
            s32Ret = HI_UNF_AVPLAY_Start(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Stop avplay fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }

        if(HI_UNF_AVPLAY_SYNC_REF_BUTT != pChnAttr->enLastSyncState)
        {
            stSyncAttr.enSyncRef = pChnAttr->enLastSyncState;

            s32Ret = HI_UNF_AVPLAY_SetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set avplay sync attr fail! ret=%#x\n", s32Ret);
            }
        }
    }

    return s32Ret;
}

HI_S32 PVRPlaySetPauseMode(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_PVR_PLAY_STATE_E enCurPlayState = HI_UNF_PVR_PLAY_STATE_BUTT;

    enCurPlayState = pChnAttr->enState;

    switch(enCurPlayState)
    {
        case HI_UNF_PVR_PLAY_STATE_PAUSE:
        {
            return HI_SUCCESS;
        }
        case HI_UNF_PVR_PLAY_STATE_FF:
        case HI_UNF_PVR_PLAY_STATE_FB:   
        {
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
            HI_PVR_SmoothExit(&pChnAttr->stFBAttr);   
#endif
            s32Ret = PVRPlaySetFrmRateAutoDetect(pChnAttr);
            if (HI_SUCCESS != s32Ret)  
            {
                HI_ERR_PVR("Enable frame rate auto detect fail]! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_STEPF:
        case HI_UNF_PVR_PLAY_STATE_SF:
        {
            s32Ret = PVRPlaySetPlayerSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to player fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlaySetDecoderSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to decoder fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlayResetToCurFrame(pChnAttr);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Reset to current display frame fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = HI_UNF_AVPLAY_Resume(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Resume avplay failed! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlayAudSyncCtrl(pChnAttr, HI_TRUE);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Disable audio and sync fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_PLAY:
        {
            s32Ret = HI_UNF_AVPLAY_Pause(pChnAttr->hAvplay, HI_NULL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set avplay pause fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            pChnAttr->bEndOfFile = HI_FALSE;
            pChnAttr->bStartOfFile = HI_FALSE;
            pChnAttr->bFlushingFlag = HI_FALSE;
            break;
        }
        case HI_UNF_PVR_PLAY_STATE_STEPB:
        case HI_UNF_PVR_PLAY_STATE_SB:
        {
            HI_ERR_PVR("Current play state is not support! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_NOT_SUPPORT;
        }
        default:
        {
            HI_ERR_PVR("Current play state is invalid! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_PLAY_INVALID_STATE;
        }
    };
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;
    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_PAUSE;
    pChnAttr->bInterPauseForTimeshift = HI_FALSE;
    return s32Ret;
}

HI_S32 PVRPlaySetNormalMode(PVR_PLAY_CHN_S  *pChnAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_PVR_PLAY_STATE_E enCurPlayState = HI_UNF_PVR_PLAY_STATE_BUTT;

    enCurPlayState = pChnAttr->enState;

    switch(enCurPlayState)
    {
        case HI_UNF_PVR_PLAY_STATE_PLAY:
        {
            HI_INFO_PVR("Have no use for change play speed! curspeed=%d\n", pChnAttr->enSpeed);
            return s32Ret;
        }
        case HI_UNF_PVR_PLAY_STATE_FF:
        case HI_UNF_PVR_PLAY_STATE_FB:  
        {
#ifndef HI_PVR_SOFT_INDEX_SUPPORT            
            HI_PVR_SmoothExit(&pChnAttr->stFBAttr);
#endif
        }
        case HI_UNF_PVR_PLAY_STATE_SF:
        {
            s32Ret = PVRPlaySetFrmRateAutoDetect(pChnAttr);
            if (HI_SUCCESS != s32Ret)  
            {
                HI_ERR_PVR("Enable frame rate auto detect fail]! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlaySetPlayerSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to player fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
            
            s32Ret = PVRPlaySetDecoderSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to decoder fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_STEPF:
        {
            s32Ret = PVRPlayResetToCurFrame(pChnAttr);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Reset to current display frame fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlayAudSyncCtrl(pChnAttr, HI_TRUE);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Disable audio and sync fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_PAUSE:
        {
            s32Ret = HI_UNF_AVPLAY_Resume(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Resume avplay failed! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            pChnAttr->bEndOfFile    = HI_FALSE;
            pChnAttr->bStartOfFile  = HI_FALSE;
            pChnAttr->bFlushingFlag = HI_FALSE;
            break;
        }
        case HI_UNF_PVR_PLAY_STATE_SB:
        case HI_UNF_PVR_PLAY_STATE_STEPB:
        {
            HI_ERR_PVR("Current play state is not support! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_NOT_SUPPORT;
        }
        default:
        {
            HI_ERR_PVR("Current play state is invalid! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_PLAY_INVALID_STATE;
        }
    };
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;
    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_PLAY;

    return s32Ret;
}

#ifdef HI_PVR_SOFT_INDEX_SUPPORT
#define PVR_PLAY_IPPERCENT  (0.7)
static HI_UNF_VCODEC_MODE_E PVRPlayCalcDecoderMode(PVR_PLAY_CHN_S  *pChnAttr, HI_U32 u32SpeedRate)
{
    HI_U32 u32TrueFrameNumOneSec = 0;
    HI_U32 u32Width = 0;
    HI_U32 u32Height = 0;
    HI_FLOAT fFrameRate = 0;
    HI_UNF_VCODEC_MODE_E enDecordMode = HI_UNF_VCODEC_MODE_I;
    HI_UNF_AVPLAY_FRAMERATE_PARAM_S stFrameInfo;
    HI_UNF_AVPLAY_STREAM_INFO_S stStreamInfo;
    
    memset(&stStreamInfo, 0x00, sizeof(stStreamInfo));
    memset(&stFrameInfo, 0x00, sizeof(stFrameInfo));
    if (HI_SUCCESS != HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrameInfo))
    {
        HI_ERR_PVR("Get FrameInfo failed!\n");
        stFrameInfo.enFrmRateType = HI_UNF_AVPLAY_FRMRATE_TYPE_BUTT;
        return enDecordMode;
    }
    if ((0 != pChnAttr->stStreamInfo.u32Width)
        || (0 != pChnAttr->stStreamInfo.u32Height))
    {
        u32Width = pChnAttr->stStreamInfo.u32Width;
        u32Height = pChnAttr->stStreamInfo.u32Height;
    }
    else if (HI_SUCCESS == HI_UNF_AVPLAY_GetStreamInfo(pChnAttr->hAvplay, &stStreamInfo))
    {
        u32Width = stStreamInfo.stVidStreamInfo.u32Width;
        u32Height = stStreamInfo.stVidStreamInfo.u32Height;
    }
    if (0 == u32Width * u32Height)
    {
        return enDecordMode;
    }
    u32TrueFrameNumOneSec = pChnAttr->stTrickAttr.u32DecodeAblity * 1920/u32Width * 1088/u32Height;
    if ((0 != pChnAttr->stStreamInfo.u32FpsDemical)
        || (0 != pChnAttr->stStreamInfo.u32FpsInteger))
    {
        fFrameRate = pChnAttr->stStreamInfo.u32FpsInteger*1000 + pChnAttr->stStreamInfo.u32FpsDemical;
    }
    else if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrameInfo))
    {
        if ((HI_UNF_AVPLAY_FRMRATE_TYPE_PTS == stFrameInfo.enFrmRateType)
        || (HI_UNF_AVPLAY_FRMRATE_TYPE_STREAM == stFrameInfo.enFrmRateType))
        {
            fFrameRate = stStreamInfo.stVidStreamInfo.u32fpsDecimal + stStreamInfo.stVidStreamInfo.u32fpsInteger*1000;
        }
        else
        {
            fFrameRate = 30*1000;
        }
    }
    fFrameRate = fFrameRate/1000;
    if ((u32TrueFrameNumOneSec >= fFrameRate * u32SpeedRate ) && (2 >= u32SpeedRate))
    {
        enDecordMode = HI_UNF_VCODEC_MODE_NORMAL;
    }
    else if ((u32TrueFrameNumOneSec >= fFrameRate * u32SpeedRate * PVR_PLAY_IPPERCENT) && (4 >= u32SpeedRate))
    {
        enDecordMode = HI_UNF_VCODEC_MODE_IP;
    }

    return enDecordMode;
}
static HI_S32 PVRPlayTrickProcessSoftIndex(PVR_PLAY_CHN_S *pChnAttr, HI_UNF_PVR_PLAY_STATE_E enPlayStateTmp, HI_UNF_PVR_PLAY_SPEED_E enPlaySpeed)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_VCODEC_MODE_E enDecordMode = HI_UNF_VCODEC_MODE_I;
    
    if ((HI_UNF_PVR_PLAY_STATE_FB == enPlayStateTmp) || (HI_UNF_PVR_PLAY_STATE_FF == enPlayStateTmp))
    {
        pChnAttr->stTrickAttr.enState = enPlayStateTmp;
        if (HI_UNF_PVR_PLAY_STATE_FB == enPlayStateTmp)
        {
            enDecordMode = HI_UNF_VCODEC_MODE_FIRST_I;
        }
        else
        {
            enDecordMode = PVRPlayCalcDecoderMode(pChnAttr, enPlaySpeed/HI_UNF_PVR_PLAY_SPEED_NORMAL);
        }
        s32Ret = HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, enDecordMode);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("set decoder mode %d failed, ret = 0x%08x\n", enDecordMode, s32Ret);
        }
        else
        {
            s32Ret = HI_PVR_ProcessTrickPlay(&(pChnAttr->stTrickAttr), HI_TRUE);
        }
    }
    return s32Ret;
}
#endif

HI_S32 PVRPlaySetTrickMode(PVR_PLAY_CHN_S  *pChnAttr, HI_UNF_PVR_PLAY_SPEED_E enPlaySpeed)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_PVR_PLAY_STATE_E enPlayStateTmp = HI_UNF_PVR_PLAY_STATE_BUTT;
    HI_UNF_PVR_PLAY_STATE_E enCurPlayState = HI_UNF_PVR_PLAY_STATE_BUTT;

    enCurPlayState = pChnAttr->enState;

    if (enPlaySpeed == pChnAttr->enSpeed)
    {
        HI_INFO_PVR("Have no use for change play speed! curspeed=%d setspeed=%d\n", pChnAttr->enSpeed, enPlaySpeed);
        return s32Ret;
    }

    /*if play speed is normal, set play mode to normal*/
    if (HI_UNF_PVR_PLAY_SPEED_NORMAL == enPlaySpeed)
    {
        s32Ret = PVRPlaySetNormalMode(pChnAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Resume avplay failed! ret=%#x\n", s32Ret);
        }

        return s32Ret;
    }

    /*Not support SLOW BACKWARD mode yet*/
    PVR_GET_STATE_BY_SPEED(enPlayStateTmp, enPlaySpeed);
    if (HI_UNF_PVR_PLAY_STATE_SB == enPlayStateTmp)
    {
        HI_ERR_PVR("Slow backward play mode is not support yet!\n");
        return HI_ERR_PVR_NOT_SUPPORT;
    }

    if ((HI_UNF_PVR_PLAY_STATE_PAUSE == enCurPlayState) ||
        (HI_UNF_PVR_PLAY_STATE_PLAY == enCurPlayState))
    {
        HI_UNF_SYNC_ATTR_S stSyncAttr;
        memset(&stSyncAttr, 0x00, sizeof(stSyncAttr));
        s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Get sync attr failed! ret %x\n", s32Ret);
            return s32Ret;
        }

        pChnAttr->enLastSyncState = stSyncAttr.enSyncRef;
    }

    switch(enCurPlayState)
    {
        case HI_UNF_PVR_PLAY_STATE_PAUSE:
        case HI_UNF_PVR_PLAY_STATE_STEPF:
        {
            s32Ret = HI_UNF_AVPLAY_Resume(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Resume avplay failed! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_PLAY:
        case HI_UNF_PVR_PLAY_STATE_SF:
        case HI_UNF_PVR_PLAY_STATE_FB: 
        case HI_UNF_PVR_PLAY_STATE_FF:
        {
#ifndef HI_PVR_SOFT_INDEX_SUPPORT            
            pChnAttr->stFBAttr.enFBTimeCtrlLastSpeed = pChnAttr->enSpeed;

            if ((enPlayStateTmp != HI_UNF_PVR_PLAY_STATE_FB) && 
                (enPlayStateTmp != HI_UNF_PVR_PLAY_STATE_FF))
            {
                HI_PVR_SmoothExit(&pChnAttr->stFBAttr);
            }
#endif
            s32Ret = PVRPlayResetToCurFrame(pChnAttr);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Reset to current display frame fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlayAudSyncCtrl(pChnAttr, HI_FALSE);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Disable audio and sync fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlaySetFrmRateAutoDetect(pChnAttr);
            if (HI_SUCCESS != s32Ret)  
            {
                HI_ERR_PVR("Enable frame rate auto detect fail]! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            /*Pay attention: every time when call PVRPlaySetPlayerSpeed and PVRPlaySetDecoderSpeed,
            should first call PVRPlaySetPlayerSpeed then call PVRPlaySetDecoderSpeed; do not reverse the calling sequency.
            As PVRPlaySetPlayerSpeed will call AVPlayReset ,if first call PVRPlaySetDecoderSpeed, The Decodeing speed will be reset.
            in This situation ,For some H264 stream with out IDR ,smooth FB will have problem*/

            s32Ret = PVRPlaySetPlayerSpeed(pChnAttr, enPlaySpeed);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to player fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
#ifndef HI_PVR_SOFT_INDEX_SUPPORT  
            s32Ret = PVRPlaySetDecoderSpeed(pChnAttr, enPlaySpeed);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to decoder fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
#endif
            pChnAttr->bEndOfFile = HI_FALSE;
            pChnAttr->bStartOfFile = HI_FALSE;
            pChnAttr->bFlushingFlag = HI_FALSE;
            break;
        }

        case HI_UNF_PVR_PLAY_STATE_STEPB:
        case HI_UNF_PVR_PLAY_STATE_SB:
        {
            HI_ERR_PVR("Current play state is not support! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_NOT_SUPPORT;
        }
        default:
        {
            HI_ERR_PVR("Current play state is invalid! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_PLAY_INVALID_STATE;
        }
    };
    pChnAttr->enSpeed = enPlaySpeed;
#ifdef HI_PVR_SOFT_INDEX_SUPPORT 
    s32Ret = PVRPlayTrickProcessSoftIndex(pChnAttr, enPlayStateTmp, enPlaySpeed);   
#endif  
    PVR_GET_STATE_BY_SPEED(pChnAttr->enState, enPlaySpeed);

    return s32Ret;
}

HI_S32 PVRPlaySetStepMode(PVR_PLAY_CHN_S  *pChnAttr, HI_S32 s32Direction)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_PVR_PLAY_STATE_E enCurPlayState = HI_UNF_PVR_PLAY_STATE_BUTT;

    enCurPlayState = pChnAttr->enState;

    if ((HI_UNF_PVR_PLAY_STATE_PAUSE == enCurPlayState) ||
        (HI_UNF_PVR_PLAY_STATE_PLAY == enCurPlayState))
    {
        HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
        s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_PVR("Get sync attr failed! ret %x\n", s32Ret);
            return s32Ret;
        }

        pChnAttr->enLastSyncState = stSyncAttr.enSyncRef;
    }

    switch(enCurPlayState)
    {
        case HI_UNF_PVR_PLAY_STATE_FF:
        case HI_UNF_PVR_PLAY_STATE_FB:
        case HI_UNF_PVR_PLAY_STATE_SF:
        {
            s32Ret = PVRPlaySetFrmRateAutoDetect(pChnAttr);
            if (HI_SUCCESS != s32Ret)  
            {
                HI_ERR_PVR("Enable frame rate auto detect fail]! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlaySetPlayerSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to player fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }

            s32Ret = PVRPlaySetDecoderSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Set speed to decoder fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
            
            s32Ret = PVRPlayResetToCurFrame(pChnAttr);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Reset to current display frame fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_PAUSE:
        {
            s32Ret = HI_UNF_AVPLAY_Resume(pChnAttr->hAvplay, NULL);
            if (HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Resume avplay failed! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_PLAY:
        {
            s32Ret = PVRPlayAudSyncCtrl(pChnAttr, HI_FALSE);
            if(HI_SUCCESS != s32Ret)
            {
                HI_ERR_PVR("Disable audio and sync fail! ret=%#x\n", s32Ret);
                return s32Ret;
            }
        }
        case HI_UNF_PVR_PLAY_STATE_STEPF:
        {
            if (s32Direction > 0)
            {
                s32Ret = HI_UNF_AVPLAY_Step(pChnAttr->hAvplay, NULL);
                if (HI_SUCCESS != s32Ret)
                {
                    HI_ERR_PVR("Step play failed!\n");
                    return s32Ret;
                }

                pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_STEPF;
            }
            else
            {
                HI_ERR_PVR("Step play backward not support yet!\n");
                return HI_ERR_PVR_NOT_SUPPORT;
            }

            pChnAttr->bEndOfFile = HI_FALSE;
            pChnAttr->bStartOfFile = HI_FALSE;
            pChnAttr->bFlushingFlag = HI_FALSE;
            break;
        }
        case HI_UNF_PVR_PLAY_STATE_SB:
        case HI_UNF_PVR_PLAY_STATE_STEPB:
        {
            HI_ERR_PVR("Current play state is not support! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_NOT_SUPPORT;
        }
        default:
        {
            HI_ERR_PVR("Current play state is invalid! state=%d\n", enCurPlayState);
            return HI_ERR_PVR_PLAY_INVALID_STATE;
        }
    };
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;

    return s32Ret;
}
static HI_S32 PVRPlayReadNextFrame(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pstIdxEntry)
{
    HI_S32 s32Ret = HI_SUCCESS;
    if ((HI_FALSE == pChnAttr->bNotAvailableTsBuff) && (HI_FALSE == pChnAttr->bIndexBeyondTs))
    {
        s32Ret = PVR_Index_GetNextFrame(pChnAttr->IndexHandle, pstIdxEntry);
        if (HI_SUCCESS != s32Ret)
        {
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
            if ((pChnAttr->IndexHandle->bIsRec) && (HI_ERR_PVR_FILE_TILL_END == s32Ret))
            {
                s32Ret = HI_ERR_PVR_FILE_TILL_END;
            }
#endif            
        }
        else
        {
            /*if read next index successfuly, then check the ts data is avlibale or not*/
            if (!PVRPlayIsTsSaved(pChnAttr, pstIdxEntry))
            {
                HI_WARN_PVR("Next frame is not avalible in ts file\n");

                s32Ret = HI_ERR_PVR_FILE_TILL_END;
                pChnAttr->bEndOfFile = HI_FALSE;

                /*just rewind read frame number, not use the getted index entry*/
                (void)PVR_Index_GetPreFrame(pChnAttr->IndexHandle, pstIdxEntry);
            }
        }

        if (HI_SUCCESS == s32Ret)
        {
            pChnAttr->bEndOfFile = HI_FALSE;
        }
    }
    return s32Ret;
}
static HI_S32 PVRPlayControlSendAll(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pstIdxEntry)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if ((HI_UNF_PVR_PLAY_STATE_PLAY == pChnAttr->enState)
        || (HI_UNF_PVR_PLAY_STATE_SF == pChnAttr->enState)
        || (HI_UNF_PVR_PLAY_STATE_STEPF == pChnAttr->enState))
    {
        /* noexistent index file, read fixed size per-time*/
        if (pChnAttr->bPlayingTsNoIdx || !pChnAttr->stUserCfg.bIsClearStream)
        {
            if (pChnAttr->u64CurReadPos >= pChnAttr->u64TsFileSize)
            {
                s32Ret = HI_ERR_PVR_FILE_TILL_END;
            }
            else
            {
                pstIdxEntry->u64Offset = pChnAttr->u64CurReadPos;
                pstIdxEntry->u32FrameSize = PVR_FIFO_WRITE_BLOCK_SIZE;
            }
        }
        else
        {
            s32Ret = PVRPlayReadNextFrame(pChnAttr, pstIdxEntry);
        }
    };

    return s32Ret;
}
static HI_S32 PVRPlayControlTplay(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pstIdxEntry)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)
    {
        if ((pChnAttr->bStartOfFile == HI_TRUE) || (pChnAttr->bFlushingFlag == HI_TRUE))
        {
            return HI_ERR_PVR_FILE_TILL_START;
        }

        /* noexistent index file, read fixed size per-time*/
        if (pChnAttr->bPlayingTsNoIdx)
        {
            pstIdxEntry->u64Offset = pChnAttr->u64CurReadPos;
            pstIdxEntry->u32FrameSize = PVR_FIFO_WRITE_BLOCK_SIZE;
        }
        else
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        {
            if (HI_TRUE == pChnAttr->bStartOfFile)
            {
                return HI_ERR_PVR_FILE_TILL_START;
            }
            if ((HI_TRUE == pChnAttr->bNotAvailableTsBuff) || (HI_TRUE == pChnAttr->bIndexBeyondTs))
            {
                HI_WARN_PVR("pChnAttr->IndexHandle->u32ReadFrame = %u\n", pChnAttr->IndexHandle->u32ReadFrame);
                return s32Ret;
            }
            
            pChnAttr->stTrickAttr.enState = HI_UNF_PVR_PLAY_STATE_FB;
            s32Ret = HI_PVR_ProcessTrickPlay(&(pChnAttr->stTrickAttr), HI_FALSE);
            if (HI_ERR_PVR_FILE_TILL_START == s32Ret)
            {
                return s32Ret;
            }
            if (pChnAttr->IndexHandle->u32ReadFrame >= pChnAttr->stTrickAttr.stFbAttr.u32FBEndFrmNo)
            {
                HI_WARN_PVR("pChnAttr->IndexHandle->u32ReadFrame = %u;pChnAttr->stTrickAttr.stFbAttr.u32FBEndFrmNo = %u\n", 
                    pChnAttr->IndexHandle->u32ReadFrame, pChnAttr->stTrickAttr.stFbAttr.u32FBEndFrmNo);
                return s32Ret;
            }
            s32Ret = PVRPlayReadNextFrame(pChnAttr, pstIdxEntry);
        }
#else
        {
            memcpy((void *)pstIdxEntry, 
                   (void *)&(pChnAttr->IndexHandle->stCurPlayFrame), 
                   sizeof(PVR_INDEX_ENTRY_S));
        }
        s32Ret = HI_PVR_SmoothCheckStatus(&pChnAttr->stFBAttr,pChnAttr->enSpeed);
#endif      
    }
    if (HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState)
    {
        /* noexistent index file, read fixed size per-time*/
        if (pChnAttr->bPlayingTsNoIdx)
        {
            pstIdxEntry->u64Offset = pChnAttr->u64CurReadPos;
            pstIdxEntry->u32FrameSize = PVR_FIFO_WRITE_BLOCK_SIZE;
        }
        else
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        {
            if ((HI_TRUE == pChnAttr->bNotAvailableTsBuff) || (HI_TRUE == pChnAttr->bIndexBeyondTs))
            {
                HI_WARN_PVR("pChnAttr->IndexHandle->u32ReadFrame = %u\n", pChnAttr->IndexHandle->u32ReadFrame);
                return s32Ret;
            }
            pChnAttr->stTrickAttr.enState = HI_UNF_PVR_PLAY_STATE_FF;
            s32Ret = HI_PVR_ProcessTrickPlay(&(pChnAttr->stTrickAttr), HI_FALSE);
            if (HI_ERR_PVR_FILE_TILL_END == s32Ret)
            {
                return s32Ret;
            }
            s32Ret = PVRPlayReadNextFrame(pChnAttr, pstIdxEntry);
            
        }
#else                
        {
            memcpy((void *)pstIdxEntry, 
                   (void *)&(pChnAttr->IndexHandle->stCurPlayFrame), 
                   sizeof(PVR_INDEX_ENTRY_S));
        }
#endif
    }
    return s32Ret;
}

HI_S32 PVRPlayControl(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pstIdxEntry)
{
    HI_S32 s32Ret = HI_SUCCESS;

    /*check recording index write over playing index when timeshift rewind
    return till start when current state is FB; otherwise seek to start and reset play channel*/
    s32Ret = PVRPlayDealWithRecReachPlay(pChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        return s32Ret;
    }

    switch(pChnAttr->enState)
    {
        case HI_UNF_PVR_PLAY_STATE_PLAY:
        case HI_UNF_PVR_PLAY_STATE_SF:
        case HI_UNF_PVR_PLAY_STATE_STEPF:
        {
            s32Ret = PVRPlayControlSendAll(pChnAttr, pstIdxEntry);
            break;
        }

        case HI_UNF_PVR_PLAY_STATE_FB:
        case HI_UNF_PVR_PLAY_STATE_FF:
        {
            s32Ret = PVRPlayControlTplay(pChnAttr, pstIdxEntry);
            break;
        }

        case HI_UNF_PVR_PLAY_STATE_PAUSE:
        {
            return HI_ERR_PVR_PLAY_INVALID_STATE;
        }
        case HI_UNF_PVR_PLAY_STATE_SB:
        case HI_UNF_PVR_PLAY_STATE_STEPB:
        {
            HI_ERR_PVR("Not support status %d.\n",pChnAttr->enState);
            return HI_ERR_PVR_NOT_SUPPORT;
        }

        default:
            break;
    };

    return s32Ret;
}

HI_S32 PVRPlaySendStream(PVR_PLAY_CHN_S *pChnAttr, PVR_INDEX_ENTRY_S *pstPlayIdxEntry)
{
    HI_S32 s32Ret = HI_SUCCESS;
#ifndef HI_PVR_SOFT_INDEX_SUPPORT    
    HI_U32 u32DmxFreeTsBufSize = 0;
    HI_UNF_DMX_TSBUF_STATUS_S stDmxTsBufState;
#endif    

#ifndef HI_PVR_SOFT_INDEX_SUPPORT
    if (!(pChnAttr->bPlayingTsNoIdx) && 
        ((HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState) || 
        (HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)))
    {
        s32Ret = HI_UNF_DMX_GetTSBufferStatus(pChnAttr->hTsBuffer, &stDmxTsBufState);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Get demux ts buffer status fail.ret=%#x\n",s32Ret);
            return s32Ret;
        }

        u32DmxFreeTsBufSize = stDmxTsBufState.u32BufSize - stDmxTsBufState.u32UsedSize;
        if (u32DmxFreeTsBufSize < stDmxTsBufState.u32BufSize/2)
        {
            pChnAttr->bNotAvailableTsBuff = HI_TRUE;
            return HI_ERR_PVR_PLAY_INVALID_TSBUFFER;
        }

        pChnAttr->bNotAvailableTsBuff = HI_FALSE;
        if (pChnAttr->bRecordedVideoExist == HI_TRUE)
        {
            s32Ret = HI_PVR_SmoothEntry(&pChnAttr->stFBAttr,pChnAttr->enSpeed);
            if (HI_SUCCESS == s32Ret)
            {
                pChnAttr->IndexHandle->u32TimeShiftTillEndCnt = 0;
            }
        }
    }
    else
#endif        
    {
        pChnAttr->IndexHandle->u32TimeShiftTillEndCnt = 0;
        s32Ret = PVRPlaySendAframe(pChnAttr, pstPlayIdxEntry);
        if ((HI_SUCCESS != s32Ret) && 
            (HI_ERR_PVR_PLAY_INVALID_TSBUFFER != s32Ret) &&
            (HI_ERR_PVR_PLAY_INDEX_BEYOND_TS != s32Ret) &&
            (HI_ERR_PVR_FILE_TILL_END != s32Ret))
        {
            HI_ERR_PVR("======== send a frame err:%x ==========\n", s32Ret);
        }
    }

    return s32Ret;
}

HI_VOID PVRPlayCheck(PVR_PLAY_CHN_S  *pChnAttr, HI_S32 s32RetCode)
{
    if (HI_SUCCESS != s32RetCode)
    {
        if (HI_ERR_PVR_FILE_TILL_END == s32RetCode)
        {
            if ((HI_TRUE == pChnAttr->bEndOfFile) ||
                (HI_TRUE == PVRPlayCheckTimeShiftForwardTillEnd(pChnAttr)))
            {
                /*trigger error event report*/
                PVRPlayCheckError(pChnAttr, s32RetCode);
                pChnAttr->bEofEvent = HI_FALSE;
            }
            else
            {
                /*flush the last frame to display*/
                if (HI_TRUE != pChnAttr->IndexHandle->bIsRec)
                {
                    (HI_VOID)HI_UNF_AVPLAY_FlushStream(pChnAttr->hAvplay, HI_NULL);
                }
                if (pChnAttr->bSeekEnd)
                {
                    HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
                    if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
                    {
                        if (stSyncAttr.enSyncRef != HI_UNF_SYNC_REF_NONE)
                        {
                            pChnAttr->enLastSyncState = stSyncAttr.enSyncRef;
                            HI_ERR_PVR(" flush enLastSyncState %d\n", pChnAttr->enLastSyncState);
                            stSyncAttr.enSyncRef = HI_UNF_SYNC_REF_NONE;
                            if (HI_SUCCESS != HI_UNF_AVPLAY_SetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr))
                            {
                                HI_ERR_PVR("Set sync attr failed!\n");
                            }
                        }
                    }
                    else
                    {
                        HI_ERR_PVR("Get sync attr failed!\n");
                    }
                }
            }
        }
    }
}

/*
���ڻ�ȡоƬ���������� �� 1S �ܽ���� ֡�� (1080p HD)��
ʵ����ÿ�����ܽ����֡��������ʵ��ͼ��ķֱ��������㣻
����CV200 �Ľ��������� 65 (HD)��
�򲥷�720p ��ͼ���ʱ���ܽ���֡����:
TrueFrameNumOneSec = (((65*1920)/1080)*1088)/720 = 174;
*/
static HI_U32 PVRPlayGetVDECDecodeAblity(HI_CHIP_TYPE_E ChipID, HI_CHIP_VERSION_E ChipVersion)
{
    HI_U32 HDFrameNumOneSec = 0;

    switch (ChipID)
    {
        case HI_CHIP_TYPE_HI3712:
            HDFrameNumOneSec = 35;
            break;
        case HI_CHIP_TYPE_HI3716M:
            if(HI_CHIP_VERSION_V300 == ChipVersion)
            {
                HDFrameNumOneSec = 35;
            }
            else if (HI_CHIP_VERSION_V310 == ChipVersion)
            {
                HDFrameNumOneSec = 25;
            }
            else if (HI_CHIP_VERSION_V320 == ChipVersion)
            {
                HDFrameNumOneSec = 25;
            }
            else if (HI_CHIP_VERSION_V330 == ChipVersion)
            {
                HDFrameNumOneSec = 25;
            }
            break;
        case HI_CHIP_TYPE_HI3716H:
                HDFrameNumOneSec = 40;
            break;
        case HI_CHIP_TYPE_HI3720:
                HDFrameNumOneSec = 40;
            break;
        case HI_CHIP_TYPE_HI3716C:
            if (HI_CHIP_VERSION_V100 == ChipVersion)
            {
                HDFrameNumOneSec = 40;
            }
            else if(HI_CHIP_VERSION_V200 == ChipVersion)
            {
                HDFrameNumOneSec = 65;
            }
            break;
        case HI_CHIP_TYPE_HI3716CES:
            if (HI_CHIP_VERSION_V100 == ChipVersion)
            {
                HDFrameNumOneSec = 65;
            }
            break;
        case HI_CHIP_TYPE_HI3110E:
            if (HI_CHIP_VERSION_V500 == ChipVersion)
            {
                HDFrameNumOneSec = 25;
            }
            break;
        default:
            HDFrameNumOneSec = 30;
    }

    return HDFrameNumOneSec;
}

HI_S32 PVRPlaySmoothInit(PVR_PLAY_CHN_S** ppChnAttr)
{
    PVR_PLAY_CHN_S* pstChnAttr = HI_NULL;
#ifndef HI_PVR_SOFT_INDEX_SUPPORT  
    HI_S32 ret;
    HI_PVR_SMOOTH_PARA_S *pstFBAttr = HI_NULL;
    HI_CODEC_VIDEO_CMD_S stVdecCmd;
    HI_UNF_AVPLAY_VDEC_INFO_S stVdecInfo = {0};
    HI_UNF_AVPLAY_PRIVATE_STATUS_INFO_S stAvplayPrivInfo = {0};
#endif
    HI_SYS_VERSION_S stSysVer;

    if ((PVR_PLAY_CHN_S**)NULL == ppChnAttr)
    {
        HI_ERR_PVR("NULL pointer error\n");
        return HI_FAILURE;
    }

    pstChnAttr = *ppChnAttr;
    if ((PVR_PLAY_CHN_S*)NULL == pstChnAttr)
    {
        HI_ERR_PVR("NULL pointer error, pChnAttr is NULL\n");
        return HI_FAILURE;
    }
    pstChnAttr->stStreamInfo.u32Width = 0;
    pstChnAttr->stStreamInfo.u32Height = 0;
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    pstChnAttr->stStreamInfo.u32FpsInteger = 0;
    pstChnAttr->stStreamInfo.u32FpsDemical = 0;
#endif
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
    /*1. ������˿��ƽṹ���ʼ��*/
    pstFBAttr = &(pstChnAttr->stFBAttr);
    memset(pstFBAttr, 0x0, sizeof(HI_PVR_SMOOTH_PARA_S));
    pstFBAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_BUTT;
    pstFBAttr->hAvplay = pstChnAttr->hAvplay;
    pstFBAttr->pIndexHandle = &(pstChnAttr->IndexHandle);
    pstFBAttr->enState = pstChnAttr->enState;
    pstFBAttr->enSpeed = pstChnAttr->enSpeed;
    pstFBAttr->pChnAttr = pstChnAttr;
    
#else
    memset(&(pstChnAttr->stTrickAttr.stFbAttr), 0x00, sizeof(pstChnAttr->stTrickAttr.stFbAttr));
    memset(&(pstChnAttr->stTrickAttr.stFfAttr), 0x00, sizeof(pstChnAttr->stTrickAttr.stFfAttr));
    pstChnAttr->stTrickAttr.pChnAttr = (HI_VOID *)pstChnAttr;
    pstChnAttr->stTrickAttr.enState = pstChnAttr->enState;
    pstChnAttr->stTrickAttr.u32DecodeAblity = 0;
    pstChnAttr->stTrickAttr.u32VdecCount = 0;
#endif
    /* 2. ��ȡоƬ����*/
    memset(&stSysVer, 0, sizeof(HI_SYS_VERSION_S));
    if (HI_SUCCESS != HI_SYS_GetVersion(&stSysVer))
    {
        HI_FATAL_PVR("Cannot get system version\n");
        return HI_FAILURE;
    }

     
#ifdef HI_PVR_SOFT_INDEX_SUPPORT 
    pstChnAttr->stTrickAttr.u32DecodeAblity = PVRPlayGetVDECDecodeAblity(stSysVer.enChipTypeHardWare, stSysVer.enChipVersion);
#else
    /* 3.����������� */
    pstFBAttr->u32DecodeAblity = PVRPlayGetVDECDecodeAblity(stSysVer.enChipTypeHardWare, stSysVer.enChipVersion);
     /* 4. �ж��Ƿ����ʹ�� ��ʾ �Ż������������Ҫ��Կ��� */
    stVdecCmd.u32CmdID = HI_UNF_AVPLAY_GET_VDEC_INFO_CMD;
    stVdecCmd.pPara = &stVdecInfo;
    if (HI_SUCCESS != HI_UNF_AVPLAY_Invoke(pstChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_VCODEC, &stVdecCmd))
    {
        HI_ERR_PVR("HI_UNF_AVPLAY_Invoke get vdec info fail\n");
        pstFBAttr->u32DispFrmBufNum = PVR_DEFAULT_FRAME_BUFF_NUM; 
    }
    else
    {
        pstFBAttr->u32DispFrmBufNum = stVdecInfo.u32DispFrmBufNum;
        ret = HI_UNF_AVPLAY_Invoke(pstChnAttr->hAvplay, HI_UNF_AVPLAY_INVOKE_GET_PRIV_PLAYINFO, &stAvplayPrivInfo);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("HI_UNF_AVPLAY_Invoke get private info fail\n");
        }

        if (1 == stAvplayPrivInfo.u32DispOptimizeFlag)
        {
            if ((pstChnAttr->IndexHandle->stRecIdxInfo.stIdxInfo.u32MaxGopSize + PVR_VO_FRMBUFF_NUM_OF_ENABLE_DEI) <= pstFBAttr->u32DispFrmBufNum)
            {
                pstFBAttr->u32VoUsedFrmBufNum = PVR_VO_FRMBUFF_NUM_OF_ENABLE_DEI;
                pstFBAttr->stVdecCtrlInfo.u32DispOptimizeFlag = PVR_ENABLE_DISP_OPTIMIZE;
            }
            else
            {
                HI_ERR_PVR("Can not enable display optimize, max gop size=%d but frame buffer=%d.\n", 
                            pstChnAttr->IndexHandle->stRecIdxInfo.stIdxInfo.u32MaxGopSize,
                            stVdecInfo.u32DispFrmBufNum);
                return HI_FAILURE;
            }
        }
        else
        {
            pstFBAttr->u32VoUsedFrmBufNum = PVR_VO_FRMBUFF_NUM_OF_DISABLE_DEI;
            pstFBAttr->stVdecCtrlInfo.u32DispOptimizeFlag = PVR_DISABLE_DISP_OPTIMIZE;
        }
    }

    (HI_VOID)PVRPlayAnalysisStream(pstChnAttr);
#endif
    return HI_SUCCESS;
}

HI_VOID PVRPlayCreateTimer(PVR_PLAY_CHN_S **pChnAttr)
{
    struct sigevent stSigEvt;
    struct itimerspec stTimeSpec;
    PVR_INDEX_ENTRY_S stStartEntry = {0};

    if (HI_SUCCESS == PVR_Index_GetFrameByNum((*pChnAttr)->IndexHandle, &stStartEntry, (*pChnAttr)->IndexHandle->u32ReadFrame))
    {
        (*pChnAttr)->u32CurPlayTimeMs = stStartEntry.u32DisplayTimeMs;
    }
    else
    {
        (*pChnAttr)->u32CurPlayTimeMs = 0;
    }

    /* init timer only once */
    if (HI_FALSE == g_bPlayTimerInitFlag)
    {
        memset (&stSigEvt, 0, sizeof (struct sigevent));
        stSigEvt.sigev_value.sival_ptr = &g_stPlayTimer;
        stSigEvt.sigev_notify = SIGEV_THREAD;
        stSigEvt.sigev_notify_function = PVRPlayCalcTime;
        
        if(HI_SUCCESS != timer_create(CLOCK_REALTIME, &stSigEvt, &g_stPlayTimer))
        {
            HI_ERR_PVR("Create play timer failed!\n");
        }

        stTimeSpec.it_interval.tv_sec = 0;
        stTimeSpec.it_interval.tv_nsec = PVR_TIME_CTRL_TIMEBASE_NS;
        stTimeSpec.it_value.tv_sec = 0;
        stTimeSpec.it_value.tv_nsec = PVR_TIME_CTRL_TIMEBASE_NS;
        
        if(HI_SUCCESS != timer_settime(g_stPlayTimer, 0,/*TIMER_ABSTIME,*/ &stTimeSpec, NULL))
        {
            HI_ERR_PVR("Start play timer failed!\n");
        }
        
        g_bPlayTimerInitFlag = HI_TRUE;
    }
}

HI_VOID PVRPlayDestroyTimer(HI_VOID)
{
    HI_U32 i = 0;

    for(i = 0; i < PVR_PLAY_MAX_CHN_NUM; i++)
    {
        if ((HI_UNF_PVR_PLAY_STATE_STOP != g_stPvrPlayChns[i].enState) &&
            (HI_UNF_PVR_PLAY_STATE_INVALID != g_stPvrPlayChns[i].enState) &&
            (HI_UNF_PVR_PLAY_STATE_BUTT != g_stPvrPlayChns[i].enState))
        {
            break;
        }
    }

    if (i == PVR_PLAY_MAX_CHN_NUM)
    {
        if(HI_SUCCESS != timer_delete(g_stPlayTimer))
        {
            HI_ERR_PVR("Delete play timer failed!\n");
        }

        g_bPlayTimerInitFlag = HI_FALSE;
    }
}

static HI_S32 PVRPlayCheckSeekCondition(PVR_PLAY_CHN_S  *pChnAttr, const HI_UNF_PVR_PLAY_POSITION_S *pPosition)
{
    HI_INFO_PVR("Seek: type:%s, whence:%s, offset:%lld. \n",
                HI_UNF_PVR_PLAY_POS_TYPE_TIME == pPosition->enPositionType ? "TIME" : "Frame",
                WHENCE_STRING(pPosition->s32Whence),
                pPosition->s64Offset);

    /* for scramble stream, not support seek, presently */
    if (!pChnAttr->stUserCfg.bIsClearStream)
    {
        /* TODO: about the scramble stream */
        HI_ERR_PVR("Not support scram ts to seek!\n");
        return HI_ERR_PVR_NOT_SUPPORT;
    }
    if (HI_UNF_PVR_PLAY_POS_TYPE_SIZE == pPosition->enPositionType)
    {
        HI_ERR_PVR("Not support seek by size!\n");
        return HI_ERR_PVR_NOT_SUPPORT;
    }
    if (HI_TRUE == pChnAttr->bPlayingTsNoIdx)
    {
        HI_ERR_PVR("Not support seek for no idx file\n");
        return HI_ERR_PVR_NOT_SUPPORT;
    }
    if ((pPosition->s32Whence == SEEK_SET) && (pPosition->s64Offset < 0))
    {
        HI_ERR_PVR("seek from start, offset error: %d!\n", pPosition->s64Offset);
        return HI_ERR_PVR_INVALID_PARA;
    }
    if ((pPosition->s32Whence == SEEK_END) && (pPosition->s64Offset > 0))
    {
        HI_ERR_PVR("seek from end, offset error: %d!\n", pPosition->s64Offset);
        return HI_ERR_PVR_INVALID_PARA;
    }
    if ((pPosition->s32Whence == SEEK_CUR) && (pPosition->s64Offset == 0))
    {
        HI_WARN_PVR("seek from current, offset is %lld!\n", pPosition->s64Offset);
        return HI_SUCCESS;
    }

    return HI_SUCCESS;
}
STATIC HI_VOID PVRPlayUpdateStreamInfo(PVR_PLAY_CHN_S *pChnAttr)
{
    HI_BOOL bUpdateWH = HI_TRUE;
	HI_UNF_AVPLAY_STREAM_INFO_S stStreamInfo;
#ifdef HI_PVR_SOFT_INDEX_SUPPORT    
    HI_BOOL bUpdateFps = HI_TRUE;
	HI_UNF_AVPLAY_FRAMERATE_PARAM_S stFrameInfo;
#endif  

    if ((0 != pChnAttr->stStreamInfo.u32Width)
        || (0 != pChnAttr->stStreamInfo.u32Height))
    {
        bUpdateWH = HI_FALSE;
    }
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    if ((0 != pChnAttr->stStreamInfo.u32FpsInteger)
        || (0 != pChnAttr->stStreamInfo.u32FpsDemical))
    {
        bUpdateFps = HI_FALSE;
    }
    if ((HI_FALSE == bUpdateWH) &&(HI_FALSE == bUpdateFps))
#else
    if (HI_FALSE == bUpdateWH)
#endif
    {
        return;
    }
    
    memset(&stStreamInfo, 0x00, sizeof(stStreamInfo)); 
    if (HI_TRUE == bUpdateWH)
    {
        if (HI_SUCCESS == HI_UNF_AVPLAY_GetStreamInfo(pChnAttr->hAvplay, &stStreamInfo))
        {
            pChnAttr->stStreamInfo.u32Width = stStreamInfo.stVidStreamInfo.u32Width;
            pChnAttr->stStreamInfo.u32Height = stStreamInfo.stVidStreamInfo.u32Height;
        }
    }
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
	memset(&stFrameInfo, 0x00, sizeof(stFrameInfo));
    if (HI_TRUE == bUpdateFps)
    {
        if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_FRMRATE_PARAM, &stFrameInfo))
        {
            if ((HI_UNF_AVPLAY_FRMRATE_TYPE_PTS == stFrameInfo.enFrmRateType)
                || (HI_UNF_AVPLAY_FRMRATE_TYPE_STREAM == stFrameInfo.enFrmRateType))
            {
                pChnAttr->stStreamInfo.u32FpsDemical = stFrameInfo.stSetFrameRate.u32fpsDecimal;
                pChnAttr->stStreamInfo.u32FpsInteger = stFrameInfo.stSetFrameRate.u32fpsInteger;
            }
        }
    }
#endif
}

STATIC HI_S32 PVRPlaySeekBeforeStart(PVR_PLAY_CHN_S *pstChnAttr, const HI_UNF_PVR_PLAY_POSITION_S *pPosition)
{
    if ((pPosition->s32Whence == SEEK_CUR) && (pPosition->s64Offset <= 0))
    {
        HI_ERR_PVR("Invalid seek parameter before starting the channel!\n");
        return HI_ERR_PVR_INVALID_PARA;
    }
    pstChnAttr->stStartSeekInfo.s32Whence = pPosition->s32Whence;
    pstChnAttr->stStartSeekInfo.s64Offset = pPosition->s64Offset;
    pstChnAttr->stStartSeekInfo.bSeek = HI_TRUE;
    return HI_SUCCESS;
}

STATIC HI_VOID* PVRPlayMainRoutine(void *args)
{
    HI_S32 s32Ret = 0;
    PVR_INDEX_ENTRY_S stPlayIdxEntry;
    PVR_PLAY_CHN_S *pChnAttr = (PVR_PLAY_CHN_S *)NULL;
    pChnAttr = (PVR_PLAY_CHN_S*)args;
    HI_UNF_PVR_PLAY_SPEED_E enLastSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;

    if((PVR_PLAY_CHN_S *)NULL == pChnAttr)
    {
        HI_ERR_PVR("play channel attr is null! args=%#x\n", args);
        return HI_NULL;
    }

#ifdef DMUP_FILE_DATA
    if (NULL == g_pvrfpSend)
    {
        HI_CHAR saveName[256];

        snprintf(saveName, 255, "%s_send.ts", pChnAttr->stUserCfg.szFileName);
        g_pvrfpSend = fopen(saveName, "wb");
    }
#endif
    memset(&stPlayIdxEntry, 0x00, sizeof(stPlayIdxEntry));
    while(!pChnAttr->bPlayMainThreadStop)
    {
        PVR_LOCK(&(pChnAttr->stPlayChnMutex));

        /*check play state invalid or not*/
        if ((pChnAttr->enState > HI_UNF_PVR_PLAY_STATE_STEPF) ||
            (pChnAttr->enState < HI_UNF_PVR_PLAY_STATE_PLAY) ||
            ((pChnAttr->bEndOfFile) && (HI_FALSE == pChnAttr->bEofEvent)))
        {
            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
            usleep(1000);
            continue;
        }

        if ((HI_TRUE == pChnAttr->bInterPauseForTimeshift)
            &&((HI_UNF_PVR_PLAY_STATE_PLAY == pChnAttr->enState) 
                || (HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState)))
        {
            if (HI_TRUE != PVRPlayCheckTimeShiftBufferState(pChnAttr))
            {
                PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
                usleep(10*1000);
                continue;
            }
            if (HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState)
            {
                enLastSpeed = pChnAttr->enSpeed;
                (HI_VOID)PVRPlaySetNormalMode(pChnAttr);
                usleep(1000);
                HI_WARN_PVR("\tenLastSpeed = %u\n", enLastSpeed);
                (HI_VOID)PVRPlaySetTrickMode(pChnAttr, enLastSpeed);
            }

            PVR_Intf_DoEventCallback(pChnAttr->u32chnID, HI_UNF_PVR_EVENT_PLAY_BUFFER_END, pChnAttr->u32BufferTimeForTimeshift);
        }

        PVRPlayUpdateStreamInfo(pChnAttr);
        s32Ret = PVRPlayControl(pChnAttr, &stPlayIdxEntry);
        if (HI_SUCCESS == s32Ret)
        {
            pChnAttr->u64CurReadPos = stPlayIdxEntry.u64Offset + (HI_U64)stPlayIdxEntry.u32FrameSize;

            s32Ret = PVRPlaySendStream(pChnAttr, &stPlayIdxEntry);
            if (HI_SUCCESS != s32Ret)
            {
                if ((HI_ERR_PVR_PLAY_INVALID_TSBUFFER == s32Ret) ||
                    (HI_ERR_PVR_PLAY_INDEX_BEYOND_TS == s32Ret))
                {
                    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
                    usleep(1000);
                    continue;
                }
                else if (HI_ERR_PVR_FILE_TILL_START == s32Ret)
                {
                    if (!pChnAttr->bStartOfFile)
                    {
                        if (pChnAttr->bFlushingFlag == HI_FALSE)
                        {
                            (HI_VOID)HI_UNF_AVPLAY_FlushStream(pChnAttr->hAvplay, HI_NULL);
                            pChnAttr->bFlushingFlag = HI_TRUE;
                        }

                        if (HI_TRUE == pChnAttr->bEofEvent)
                        {
                            PVRPlayCheckError(pChnAttr, s32Ret);
                            pChnAttr->bEofEvent     = HI_FALSE;
                            pChnAttr->bFlushingFlag = HI_FALSE;
                            pChnAttr->bStartOfFile  = HI_TRUE;
                        }
                    }
                }
                else if (HI_ERR_PVR_FILE_TILL_END == s32Ret)
                {
                    PVRPlayCheck(pChnAttr, s32Ret);
                }
                else
                {
                    PVRPlayCheckError(pChnAttr, s32Ret);
                }
            }
        }
        else if (HI_ERR_PVR_FILE_TILL_START == s32Ret)      //record will reach play when timeshif fb
        {
            if (!pChnAttr->bStartOfFile)
            {
                if (pChnAttr->bFlushingFlag == HI_FALSE)
                {
                    (HI_VOID)HI_UNF_AVPLAY_FlushStream(pChnAttr->hAvplay, HI_NULL);
                    pChnAttr->bFlushingFlag = HI_TRUE;
                }

                if (HI_TRUE == pChnAttr->bEofEvent)
                {
                    PVRPlayCheckError(pChnAttr, s32Ret);
                    pChnAttr->bEofEvent     = HI_FALSE;
                    pChnAttr->bFlushingFlag = HI_FALSE;
                    pChnAttr->bStartOfFile  = HI_TRUE;
                }
            }
        }
        else if ((HI_ERR_PVR_FILE_TILL_END == s32Ret) ||
                 (HI_ERR_PVR_NOT_SUPPORT == s32Ret))
        {
            PVRPlayCheck(pChnAttr, s32Ret);
        }

        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        usleep(1000);
    }
#ifndef HI_PVR_SOFT_INDEX_SUPPORT
    //���߳̽������ͷ�smooth��Դ����ֹ�ڴ�й¶
    HI_PVR_SmoothExit(&pChnAttr->stFBAttr);
#endif

#ifdef DMUP_FILE_DATA
    if (NULL != g_pvrfpSend)
    {
        fclose(g_pvrfpSend);

        g_pvrfpSend = NULL;
    }
#endif

    return HI_NULL;
}


/*****************************************************************************
 Prototype       : HI_PVR_PlayInit
 Description     : play module initializde
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/14
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayInit(HI_VOID)
{
    HI_U32 i,j;
    HI_S32 ret;
    PVR_PLAY_CHN_S *pChnAttr;
#ifdef PVR_PROC_SUPPORT
    HI_U32 u32CurPid = getpid();
    static HI_CHAR pProcDirName[32] = {0};
#endif

    if (HI_TRUE == g_stPlayInit.bInit)
    {
        HI_WARN_PVR("Play Module has been Initialized!\n");
        return HI_SUCCESS;
    }
    else
    {
        /*initialize whole of  index */
        PVR_Index_Init();

        ret = PVRPlayDevInit();
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        ret = PVRIntfInitEvent();
        if (HI_SUCCESS != ret)
        {
            close(g_s32PvrFd);
            g_s32PvrFd = -1;
            return ret;
        }

        /* set all play channel as INVALID status */
        for (i = 0 ; i < PVR_PLAY_MAX_CHN_NUM; i++)
        {
            pChnAttr = &g_stPvrPlayChns[i];
            
            if (0 != pthread_mutex_init(&(pChnAttr->stPlayChnMutex), NULL))
            {
                close(g_s32PvrFd);
                for (j = 0; j < i; j++)
                {
                    (HI_VOID)pthread_mutex_destroy(&(g_stPvrPlayChns[j].stPlayChnMutex));
                }
                PVRIntfDeInitEvent();
                HI_ERR_PVR("init mutex lock for PVR play chn%d failed \n", i);
                return HI_FAILURE;
            }

            PVR_LOCK(&(pChnAttr->stPlayChnMutex));
            pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INVALID;
            pChnAttr->bPlayMainThreadStop = HI_TRUE;
            pChnAttr->u32chnID = i;
            pChnAttr->s32DataFile = PVR_FILE_INVALID_FILE;
            pChnAttr->u64CurReadPos = 0;
            pChnAttr->IndexHandle = NULL;
            pChnAttr->hCipher = 0;
            pChnAttr->stPlayMainThread = 0;
            pChnAttr->readCallBack = NULL;
            pChnAttr->u32BufferTimeForTimeshift = PVR_PLAY_BUFFER_DEFAULT_TIME_TIMESHIFT;
            memset(&pChnAttr->stUserCfg, 0, sizeof(HI_UNF_PVR_PLAY_ATTR_S));
            pChnAttr->bTimeShiftStartFlg = HI_FALSE;
            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        }

#ifdef PVR_PROC_SUPPORT
        memset(pProcDirName, 0, sizeof(pProcDirName));
        if (!PVRRecCheckInitStat())
        {
            ret = HI_MODULE_Register(HI_ID_PVR, PVR_USR_PROC_DIR);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("HI_MODULE_Register(\"%s\") return %d\n", PVR_USR_PROC_DIR, ret);
            }

            /* Add proc dir */
            snprintf(pProcDirName, sizeof(pProcDirName), "%s_%d", PVR_USR_PROC_DIR, u32CurPid);
            ret = HI_PROC_AddDir(pProcDirName);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_PVR("HI_PROC_AddDir(\"%s\") return %d\n", pProcDirName, ret);
            }
        }

        if (0 == strlen(pProcDirName))
        {
            snprintf(pProcDirName, sizeof(pProcDirName), "%s_%d", PVR_USR_PROC_DIR, u32CurPid);
        }

        /* Will be added at /proc/hisi/${DIRNAME} directory */
        g_stPvrPlayProcEntry.pszDirectory = pProcDirName;
        g_stPvrPlayProcEntry.pszEntryName = PVR_USR_PROC_PLAY_ENTRY_NAME;
        g_stPvrPlayProcEntry.pfnShowProc = PVRPlayShowProc;
        g_stPvrPlayProcEntry.pfnCmdProc = PVRPlaySetProc;
        g_stPvrPlayProcEntry.pPrivData = g_stPvrPlayChns;
        ret = HI_PROC_AddEntry(HI_ID_PVR, &g_stPvrPlayProcEntry);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_PVR("HI_PROC_AddEntry(\"%s\") return %d\n", PVR_USR_PROC_PLAY_ENTRY_NAME, ret);
        }
#endif

        g_stPlayInit.bInit = HI_TRUE;

        return HI_SUCCESS;
    }
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayDeInit
 Description     : play module de-initialize
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/14
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayDeInit(HI_VOID)
{
    HI_U32 i;

    if ( HI_FALSE == g_stPlayInit.bInit )
    {
        HI_WARN_PVR("Play Module is not Initialized!\n");
        return HI_SUCCESS;
    }
    else
    {
        /* set all play channel as INVALID status */
        for (i = 0 ; i < PVR_PLAY_MAX_CHN_NUM; i++)
        {
            if (g_stPvrPlayChns[i].enState != HI_UNF_PVR_PLAY_STATE_INVALID)
            {
                HI_ERR_PVR("play chn%d is in use, can NOT deInit PLAY!\n", i);
                return HI_ERR_PVR_BUSY;
            }

            (HI_VOID)pthread_mutex_destroy(&(g_stPvrPlayChns[i].stPlayChnMutex));
        }

#ifdef PVR_PROC_SUPPORT
        HI_PROC_RemoveEntry(HI_ID_PVR, &g_stPvrPlayProcEntry);
        if (!PVRRecCheckInitStat())
        {
            HI_PROC_RemoveDir(g_stPvrPlayProcEntry.pszDirectory);
            HI_MODULE_UnRegister(HI_ID_PVR);
        }
#endif

        g_stPlayInit.bInit = HI_FALSE;
        PVRIntfDeInitEvent();
        return HI_SUCCESS;
    }
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayCreateChn
 Description     : create one play channel
 Input           : pAttr  **the attribute of channel
 Output          : pchn   **play channel number
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayCreateChn(HI_U32 *pChn, const HI_UNF_PVR_PLAY_ATTR_S *pAttr, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer)
{
    HI_S32 ret;
    PVR_PLAY_CHN_S *pChnAttr =NULL;
#ifdef HI_PVR_SOFT_INDEX_SUPPORT     
    HI_UNF_VCODEC_ATTR_S stVdecAttr;
#endif

    PVR_CHECK_POINTER(pAttr);
    PVR_CHECK_POINTER(pChn);

    PVR_PLAY_CHECK_INIT(&g_stPlayInit);

    ret = PVRPlayCheckUserCfg(pAttr, hAvplay, hTsBuffer);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    pChnAttr = PVRPlayFindFreeChn();
    if (NULL == pChnAttr)
    {
        HI_ERR_PVR("Not enough channel to be used!\n");
        return HI_ERR_PVR_NO_CHN_LEFT;
    }

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));

    pChnAttr->s32DataFile = PVR_FILE_INVALID_FILE;
    pChnAttr->IndexHandle = HI_NULL;
    pChnAttr->hCipher = 0;
    pChnAttr->stPlayMainThread = 0;
    pChnAttr->bPlayMainThreadStop = HI_TRUE;
    pChnAttr->bFlushingFlag = HI_FALSE;
    pChnAttr->bStartOfFile = HI_FALSE;
    pChnAttr->bEndOfFile = HI_FALSE;
    pChnAttr->bSeekEnd = HI_FALSE;
    pChnAttr->u64CurReadPos = 0;
    //pChnAttr->bAdecStoped = HI_FALSE;
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;
    pChnAttr->bPlayingTsNoIdx = HI_FALSE;
    pChnAttr->enLastSyncState = HI_UNF_AVPLAY_SYNC_REF_BUTT;
	pChnAttr->stStartSeekInfo.bSeek = HI_FALSE;
    pChnAttr->stStartSeekInfo.s64Offset = 0;
    pChnAttr->stStartSeekInfo.s32Whence = 0;
    pChnAttr->u32SeekSysTime = 0;
    pChnAttr->bTimeShiftStartFlg = HI_FALSE;
    pChnAttr->u32BufferTimeForTimeshift = PVR_PLAY_BUFFER_DEFAULT_TIME_TIMESHIFT;
    if (0 != pAttr->u32TimeshiftBufferMs)
    {
        pChnAttr->u32BufferTimeForTimeshift = pAttr->u32TimeshiftBufferMs;
    }
    memset(&pChnAttr->stStreamInfo, 0x00, sizeof(pChnAttr->stStreamInfo)); 
    memcpy(&pChnAttr->stUserCfg, pAttr, sizeof(HI_UNF_PVR_PLAY_ATTR_S));
    //pChnAttr->stUserCfg.bIsClearStream = HI_FALSE;
    /* initialize cipher module */
    ret = PVRPlayPrepareCipher(pChnAttr);
    if (ret != HI_SUCCESS)
    {
        goto ErrorExit;
    }

    /*  check whether current to open file is recording or not
        if recording, return the recording file index handle, regard it as timeshift play channel to manage
        or alone play channel, which not support record the playing file.
    */
    pChnAttr->IndexHandle = PVR_Index_CreatPlay(pChnAttr->u32chnID, pAttr, &pChnAttr->bPlayingTsNoIdx);
    if (NULL == pChnAttr->IndexHandle)
    {
        HI_ERR_PVR("index init failed.\n");
        ret = HI_ERR_PVR_FILE_CANT_READ;
        goto ErrorExit;
    }
    
#ifdef HI_PVR_SOFT_INDEX_SUPPORT     
    memset(&stVdecAttr, 0x00, sizeof(stVdecAttr));
    if (HI_SUCCESS == HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr))
    {
        stVdecAttr.u32ErrCover = 0;
        (HI_VOID)HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &stVdecAttr);
    }
    else
    {
        HI_ERR_PVR("calling HI_UNF_AVPLAY_GetAttr failed\n");
    }
#endif

    /* open ts file */
    pChnAttr->s32DataFile = PVR_OPEN64(pAttr->szFileName,PVR_FOPEN_MODE_DATA_READ);
    if (PVR_FILE_INVALID_FILE == pChnAttr->s32DataFile)
    {
        ret = HI_ERR_PVR_FILE_CANT_OPEN;
        goto ErrorExit;
    }

    pChnAttr->u64TsFileSize = PVR_FILE_GetFileSize64(pAttr->szFileName);

    pChnAttr->hAvplay = hAvplay;
    pChnAttr->hTsBuffer = hTsBuffer;
    *pChn = pChnAttr->u32chnID;

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    HI_INFO_PVR("\n--------PVR.PLAY.Ver:%s_%s\n\n", __DATE__, __TIME__);

    return HI_SUCCESS;

ErrorExit:
    if (PVR_FILE_INVALID_FILE != pChnAttr->s32DataFile)
    {
        (HI_VOID)PVR_CLOSE64(pChnAttr->s32DataFile);
        pChnAttr->s32DataFile = PVR_FILE_INVALID_FILE;
    }

    if (pChnAttr->IndexHandle)
    {
       (HI_VOID)PVR_Index_Destroy(pChnAttr->IndexHandle, PVR_INDEX_PLAY);
       pChnAttr->IndexHandle = NULL;
    }

    (HI_VOID)PVRPlayReleaseCipher(pChnAttr);

    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INVALID;
    if (HI_SUCCESS != ioctl(g_s32PvrFd, CMD_PVR_DESTROY_PLAY_CHN, (HI_S32)&(pChnAttr->u32chnID)))
    {
        HI_ERR_PVR("Destroy rec chn failed\n");
    }
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return ret;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayDestroyChn
 Description     : destroy one play channel
 Input           : u32Chn  **channel number
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayDestroyChn(HI_U32 u32Chn)
{
    PVR_PLAY_CHN_S *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    /* check channel state */
    if (!(HI_UNF_PVR_PLAY_STATE_STOP == pChnAttr->enState
        || HI_UNF_PVR_PLAY_STATE_INIT == pChnAttr->enState))
    {
        HI_ERR_PVR("You should stop channel first!\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_ERR_PVR_PLAY_INVALID_STATE;
    }

    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INVALID;
    if (HI_SUCCESS != ioctl(g_s32PvrFd, CMD_PVR_DESTROY_PLAY_CHN, (HI_S32)&u32Chn))
    {
        HI_FATAL_PVR("pvr play destroy channel error.\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    /* close stream file */
    (HI_VOID)PVR_CLOSE64(pChnAttr->s32DataFile);

    (HI_VOID)PVR_Index_SetCurReadFrameNum(pChnAttr->IndexHandle, 0);
    (HI_VOID)PVR_Index_Destroy(pChnAttr->IndexHandle, PVR_INDEX_PLAY);
    pChnAttr->IndexHandle = NULL;

    (HI_VOID)PVRPlayReleaseCipher(pChnAttr);

    pChnAttr->stPlayMainThread = 0;

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return HI_SUCCESS;
}


/*****************************************************************************
 Prototype       : HI_PVR_PlaySetChn
 Description     : set play channle attributes
 Input           : chn    **
                   pAttr  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/29
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32  HI_PVR_PlaySetChn(HI_U32 u32ChnID, const HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr)
{
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32ChnID);
    pChnAttr = &g_stPvrPlayChns[u32ChnID];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);

    PVR_CHECK_POINTER(pstPlayAttr);

    /* TODO: we set several attributes which can be set dynamically. */

    return HI_ERR_PVR_NOT_SUPPORT;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayGetChn
 Description     : get play channel attribute
 Input           : chn    **
                   pAttr  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/29
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayGetChn(HI_U32 u32ChnID, HI_UNF_PVR_PLAY_ATTR_S *pstPlayAttr)
{
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32ChnID);
    pChnAttr = &g_stPvrPlayChns[u32ChnID];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);

    PVR_CHECK_POINTER(pstPlayAttr);

    memcpy(pstPlayAttr, &pChnAttr->stUserCfg, sizeof(HI_UNF_PVR_PLAY_ATTR_S));

    return HI_SUCCESS;
}


/*****************************************************************************
 Prototype       : HI_PVR_PlayStartChn
 Description     : start play channel
 Input           : u32ChnID **channel number
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayStartChn(HI_U32 u32ChnID) /* pause when end of file */
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32Pid = 0x1fff;
    PVR_PLAY_CHN_S *pChnAttr = (PVR_PLAY_CHN_S *)HI_NULL;
    HI_BOOL bAudExist = HI_FALSE;

    PVR_PLAY_CHECK_CHN(u32ChnID);
    pChnAttr = &g_stPvrPlayChns[u32ChnID];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    pChnAttr->bInterPauseForTimeshift = HI_FALSE;
    
    if (!(HI_UNF_PVR_PLAY_STATE_STOP == pChnAttr->enState
          || HI_UNF_PVR_PLAY_STATE_INIT == pChnAttr->enState))
    {
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        HI_ERR_PVR("Can't start play channel at current state!\n");
        return HI_ERR_PVR_PLAY_INVALID_STATE;
    }

    memset(&pChnAttr->stLastStatus, 0, sizeof(HI_UNF_PVR_PLAY_STATUS_S));

    PVR_Index_ResetPlayAttr(pChnAttr->IndexHandle);

    PVR_Index_GetRecIdxInfo(pChnAttr->IndexHandle);

    if (PVR_REC_INDEX_MAGIC_WORD != pChnAttr->IndexHandle->stRecIdxInfo.u32MagicWord)
    {
        PVR_Index_GetIdxInfo(pChnAttr->IndexHandle);
    }

    s32Ret = HI_UNF_AVPLAY_RegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS, PVRPlayProcessEos);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("registe avplay EOS event fail! ret=%#x\n",s32Ret);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    s32Ret = HI_UNF_AVPLAY_RegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME, PVRPlayProcessNewVidFrame);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("registe avplay EOS event fail! ret=%#x\n",s32Ret);
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }
#endif

    s32Ret = HI_MPI_AVPLAY_SetWindowStepMode(pChnAttr->hAvplay, HI_FALSE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("avplay set step mode failed! ret %x\n", s32Ret);
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
#endif
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, HI_UNF_VCODEC_MODE_NORMAL);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("set vdec normal mode error!\n");
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
#endif        
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    /*===============================avplay start the audio================================*/
    /*attentain: if audio start failed, should not return ,continue to try start the vedio ,
    because PVR support playback pure vedio without audio*/
    s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID, &u32Pid);
    if ((HI_SUCCESS != s32Ret) || (0x1fff == u32Pid))
    {
        HI_ERR_PVR("No audio stream! ret=%#x pid=%d\n", s32Ret, u32Pid);
    }
    else
    {
        s32Ret = HI_UNF_AVPLAY_Start(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, NULL);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Can't start audio, error:%#x!\n", s32Ret);
        }
        else
        {
            HI_INFO_PVR("HI_UNF_AVPLAY_start audio ok!\n");
            bAudExist = HI_TRUE;
        }
    }

    /*===============================avplay start the vedio================================*/
    s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID, &u32Pid);
    if ((HI_SUCCESS != s32Ret) || (0x1fff == u32Pid))
    {
        pChnAttr->bRecordedVideoExist = HI_FALSE;
        HI_ERR_PVR("No video stream! ret=%#x pid=%d\n", s32Ret, u32Pid);
    }
    else
    {
        pChnAttr->bRecordedVideoExist = HI_TRUE;
        s32Ret = HI_UNF_AVPLAY_Start(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
        if (HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("Can't start video, error:%#x!\n", s32Ret);
            (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
            (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
#endif            
            if (HI_TRUE == bAudExist)
            {              
                (HI_VOID)HI_UNF_AVPLAY_Stop(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD , NULL);
            }
            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
            return s32Ret;
        }
        HI_INFO_PVR("HI_UNF_AVPLAY_start video ok!\n");
    }

    if (pChnAttr->enLastSyncState != HI_UNF_AVPLAY_SYNC_REF_BUTT)
    {
        HI_UNF_SYNC_ATTR_S stSyncAttr = {0};
        s32Ret = HI_UNF_AVPLAY_GetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
        if (s32Ret == HI_SUCCESS)
        {
            stSyncAttr.enSyncRef = pChnAttr->enLastSyncState;
            s32Ret = HI_UNF_AVPLAY_SetAttr(pChnAttr->hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &stSyncAttr);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_PVR("Set sync attr failed! ret %x\n", s32Ret);
            }
        }
        else
        {
            HI_ERR_PVR("Get sync attr failed! ret %x\n", s32Ret);
        }
    }

    PVRPlaySeektoStartFrame(pChnAttr);

    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_PLAY;
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_NORMAL;
    pChnAttr->bPlayMainThreadStop = HI_FALSE;
    pChnAttr->bEndOfFile = HI_FALSE;
    pChnAttr->bEofEvent = HI_FALSE;
    pChnAttr->bNotAvailableTsBuff = HI_FALSE;
    pChnAttr->bIndexBeyondTs = HI_FALSE;

    s32Ret = PVRPlaySmoothInit(&pChnAttr);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PVR("PVRPlaySmoothInit failed,ret = 0x%x\n",s32Ret);
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
#endif        
        (HI_VOID)HI_UNF_AVPLAY_Stop(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
        pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INIT ;
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    if (pthread_create(&pChnAttr->stPlayMainThread, NULL, PVRPlayMainRoutine, pChnAttr))
    {
        HI_ERR_PVR("create play thread failed!\n");
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
        (HI_VOID)HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
#endif        
        (void)HI_UNF_AVPLAY_Stop(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, NULL);
        /* and also reset play state to init                                      */
        pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_INIT ;
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    PVRPlayCreateTimer(&pChnAttr);

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return HI_SUCCESS;
}


/*****************************************************************************
 Prototype       : HI_PVR_PlayStopChn
 Description     : stop play channel
 Input           : u32Chn  **channel number
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayStopChn(HI_U32 u32Chn, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt)
{
    HI_S32 ret;
    PVR_PLAY_CHN_S  *pChnAttr;
    HI_UNF_AVPLAY_STATUS_INFO_S stAvplayStatus;

    PVR_PLAY_CHECK_CHN(u32Chn);

    memset(&stAvplayStatus, 0, sizeof(HI_UNF_AVPLAY_STATUS_INFO_S));
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    if ((HI_UNF_PVR_PLAY_STATE_STOP == pChnAttr->enState)
        || (HI_UNF_PVR_PLAY_STATE_INIT == pChnAttr->enState))
    {
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        HI_ERR_PVR("Play channel is stopped already!\n");
        return HI_ERR_PVR_ALREADY;
    }

    HI_INFO_PVR("wait Play thread.\n");
    pChnAttr->bPlayMainThreadStop = HI_TRUE;
    
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    (HI_VOID)pthread_join(pChnAttr->stPlayMainThread, NULL);
    HI_INFO_PVR("wait Play thread OK.\n");

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    
    if (pChnAttr->bRecordedVideoExist == HI_TRUE)
    {
        ret = PVRPlaySetFrmRateAutoDetect(pChnAttr);
        if (HI_SUCCESS != ret)  
        {
            HI_ERR_PVR("set frame to VO fail.\n");
            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
            return HI_FAILURE;
        }

        if (HI_SUCCESS == HI_UNF_AVPLAY_GetStatusInfo(pChnAttr->hAvplay, &stAvplayStatus))
        {
            if (HI_UNF_AVPLAY_STATUS_STOP != stAvplayStatus.enRunStatus)
            {
                ret = PVRPlaySetDecoderSpeed(pChnAttr, HI_UNF_PVR_PLAY_SPEED_NORMAL);
                if (HI_SUCCESS != ret)
                {
                    HI_ERR_PVR("Resume Avplay trick mode to normal fail.\n");
                    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
                    return HI_FAILURE;
                }
            }
        }
    }

    ret = HI_MPI_AVPLAY_SetWindowRepeat(pChnAttr->hAvplay, 1);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("AVPLAY set window repeat failed!\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    ret = HI_MPI_AVPLAY_SetWindowStepMode(pChnAttr->hAvplay, HI_FALSE);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("avplay set step mode failed! ret %x\n", ret);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    ret = HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, HI_UNF_VCODEC_MODE_NORMAL);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("set vdec normal mode error! ret %x\n", ret);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    ret = HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_EOS);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("unregiste avplay EOS event fail! ret=%#x\n",ret);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }
#ifdef HI_PVR_SOFT_INDEX_SUPPORT
    ret = HI_UNF_AVPLAY_UnRegisterEvent(pChnAttr->hAvplay, HI_UNF_AVPLAY_EVENT_NEW_VID_FRAME);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("unregiste avplay EOS event fail! ret=%#x\n",ret);
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }
#endif

    //lint -e655
    ret = HI_UNF_AVPLAY_Stop(pChnAttr->hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD | HI_UNF_AVPLAY_MEDIA_CHAN_VID, pstStopOpt);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("HI_UNF_AVPLAY_Stop failed:%#x, force PVR stop!\n", ret);
    }
    //lint +e655

    ret = HI_UNF_DMX_ResetTSBuffer(pChnAttr->hTsBuffer);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("ts buffer reset failed!\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_FAILURE;
    }

    pChnAttr->enState = HI_UNF_PVR_PLAY_STATE_STOP;
    pChnAttr->enSpeed = HI_UNF_PVR_PLAY_SPEED_BUTT;
    pChnAttr->bPlayMainThreadStop = HI_FALSE;
    pChnAttr->bEndOfFile = HI_FALSE;
    pChnAttr->bEofEvent = HI_FALSE;
    pChnAttr->bRecordedVideoExist = HI_FALSE;
    pChnAttr->stStartSeekInfo.bSeek = HI_FALSE;
    pChnAttr->stStartSeekInfo.s64Offset = 0;
    pChnAttr->stStartSeekInfo.s32Whence = 0;
    memset(&pChnAttr->stLastStatus, 0, sizeof(HI_UNF_PVR_PLAY_STATUS_S));

    PVR_Index_ResetPlayAttr(pChnAttr->IndexHandle);

    PVRPlayDestroyTimer();

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayStartTimeShift
 Description     : start TimeShift
 Input           : pu32PlayChnID   **
                   u32DemuxID   **
                   u32RecChnID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayStartTimeShift(HI_U32 *pu32PlayChnID, HI_U32 u32RecChnID, HI_HANDLE hAvplay, HI_HANDLE hTsBuffer)
{
    HI_S32 ret;
    HI_U32 u32PlayChnID = 0;
    HI_UNF_PVR_REC_ATTR_S RecAttr;
    HI_UNF_PVR_PLAY_ATTR_S PlayAttr;

    PVR_CHECK_POINTER(pu32PlayChnID);

    /* get record channel attribute */
    memset(&RecAttr, 0x00, sizeof(HI_UNF_PVR_REC_ATTR_S));
    ret = HI_PVR_RecGetChn(u32RecChnID, &RecAttr);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    memset(&PlayAttr, 0x00, sizeof(HI_UNF_PVR_PLAY_ATTR_S));
    /* configure play channel with record channel attributes */
    PlayAttr.enStreamType = RecAttr.enStreamType;
    PlayAttr.u32FileNameLen = RecAttr.u32FileNameLen;
    PlayAttr.bIsClearStream = RecAttr.bIsClearStream;
    memset(PlayAttr.szFileName, 0, sizeof(PlayAttr.szFileName));
    strncpy(PlayAttr.szFileName, RecAttr.szFileName, strlen(RecAttr.szFileName));
    PlayAttr.stDecryptCfg.bDoCipher = RecAttr.stEncryptCfg.bDoCipher;
    PlayAttr.stDecryptCfg.enType = RecAttr.stEncryptCfg.enType;
    PlayAttr.stDecryptCfg.u32KeyLen = RecAttr.stEncryptCfg.u32KeyLen;
    memcpy(PlayAttr.stDecryptCfg.au8Key, RecAttr.stEncryptCfg.au8Key, PVR_MAX_CIPHER_KEY_LEN);

    /* apply a new play channel for timeshift playing */
    ret = HI_PVR_PlayCreateChn(&u32PlayChnID, &PlayAttr, hAvplay, hTsBuffer);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /* start timeshift playing  */
    ret = HI_PVR_PlayStartChn(u32PlayChnID);
    if (HI_SUCCESS != ret)
    {
        (HI_VOID)HI_PVR_PlayDestroyChn(u32PlayChnID);
        return ret;
    }

    *pu32PlayChnID = u32PlayChnID;
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayStopTimeShift
 Description     : stop TimeShift
 Input           : u32PlayChnID  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/10
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayStopTimeShift(HI_U32 u32PlayChnID, const HI_UNF_AVPLAY_STOP_OPT_S *pstStopOpt)
{
    HI_S32 ret;

    PVR_PLAY_CHN_S  *pChnAttr;
    PVR_PLAY_CHECK_CHN(u32PlayChnID);
    pChnAttr = &g_stPvrPlayChns[u32PlayChnID];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);

    /* stop timeshift play channel */
    ret = HI_PVR_PlayStopChn(u32PlayChnID, pstStopOpt);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("stop play chn failed:%#x!\n", ret);
        return ret;
    }

    ret = HI_PVR_PlayDestroyChn(u32PlayChnID);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_PVR("destroy play chn failed:%#x!\n", ret);
        return ret;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayPauseChn
 Description     : pause play channel
 Input           : u32Chn  ** channel number
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayPauseChn(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PVR_PLAY_CHN_S  *pChnAttr;

    /* when u32Chn is record channel, mean to pause the live stream */
    if (PVRRecCheckChnRecording(u32Chn))
    {
        return PVRRecMarkPausePos(u32Chn);
    }

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    s32Ret = PVRPlaySetPauseMode(pChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Set pause mode fail! ret=%#x\n", s32Ret);
    }

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayResumeChn
 Description     : resume the play channel
 Input           : u32Chn  **channel number
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayResumeChn(HI_U32 u32Chn)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32Chn);

    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    s32Ret = PVRPlaySetNormalMode(pChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Set normal mode fail! ret=%#x\n", s32Ret);
    }

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayTrickMode
 Description     : set the play mode of play channel
 Input           : u32Chn         **channel number
                   pTrickMode  **play mode, trick mode
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayTrickMode(HI_U32 u32Chn, const HI_UNF_PVR_PLAY_MODE_S *pTrickMode)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_CHECK_POINTER(pTrickMode);
    PVR_PLAY_CHECK_CHN(u32Chn);

    pChnAttr = &g_stPvrPlayChns[u32Chn];

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    /*recorded file without index, not support trick play mode*/
    if (HI_TRUE == pChnAttr->bPlayingTsNoIdx)
    {
        HI_ERR_PVR("No index file, NOT support trick mode.\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_ERR_PVR_NOT_SUPPORT;
    }

    /* audio index file or no audio pid, not support Tplay*/
    if (PVR_INDEX_IS_TYPE_AUDIO(pChnAttr->IndexHandle) || (HI_FALSE == pChnAttr->bRecordedVideoExist))
    {
        HI_ERR_PVR("audio indexed stream, or no audio pid stream, NOT support trick mode.\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_ERR_PVR_NOT_SUPPORT;
    }

    /* scramble stream not support Tplay */
    if (!pChnAttr->stUserCfg.bIsClearStream)
    {
        HI_ERR_PVR("scrambed stream NOT support trick mode !\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_ERR_PVR_NOT_SUPPORT;
    }

    s32Ret = PVRPlaySetTrickMode(pChnAttr, pTrickMode->enSpeed);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Set trick play mode fail! ret=%#x\n", s32Ret);
    }

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlaySeek
 Description     : seek to play
 Input           : u32Chn        **channel number
                   pPosition  **the position to play
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlaySeek(HI_U32 u32Chn, const HI_UNF_PVR_PLAY_POSITION_S *pPosition)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U32 u32CurPlayTime = 0;
    HI_U32 u32SeekCurTime = 0;
    PVR_PLAY_CHN_S  *pChnAttr;
    PVR_INDEX_ENTRY_S stReadFrame;

    PVR_CHECK_POINTER(pPosition);
    PVR_PLAY_CHECK_CHN(u32Chn);

    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);
    s32Ret = PVRPlayCheckSeekCondition(pChnAttr, pPosition);
    if ( s32Ret != HI_SUCCESS )
    {
        HI_ERR_PVR("PVRPlayCheckSeekCondition failed, ret=0x%x\n", s32Ret);
        return s32Ret;  
    }

    if (HI_UNF_PVR_PLAY_POS_TYPE_TIME == pPosition->enPositionType)
    {
        u32CurPlayTime = PVRPlayGetCurPlayTime(pChnAttr);        
    }

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);
    
    if (HI_UNF_PVR_PLAY_STATE_INIT == pChnAttr->enState)
    {
        s32Ret = PVRPlaySeekBeforeStart(pChnAttr, pPosition);
        if(HI_SUCCESS != s32Ret)
        {
            HI_ERR_PVR("seek failed, ret = 0x%x\n", s32Ret);
        }
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return s32Ret;
    }
    
    if ((HI_UNF_PVR_PLAY_STATE_PLAY != pChnAttr->enState)
        && (HI_UNF_PVR_PLAY_STATE_PAUSE != pChnAttr->enState))
    {
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        HI_ERR_PVR("Current state(%d) don't support seek, resume first!\n", pChnAttr->enState);
        return HI_ERR_PVR_NOT_SUPPORT;
    }
    s32Ret = PVRPlayResetToCurFrame(pChnAttr);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("reset to current frame failed!\n");
    }

    switch ( pPosition->enPositionType )
    {
        case HI_UNF_PVR_PLAY_POS_TYPE_SIZE:
        case HI_UNF_PVR_PLAY_POS_TYPE_FRAME:
            s32Ret = HI_ERR_PVR_NOT_SUPPORT;
            break;
        case HI_UNF_PVR_PLAY_POS_TYPE_TIME: 
            if (HI_TRUE == pChnAttr->bTimeShiftStartFlg)
            {
                u32SeekCurTime = pChnAttr->u32CurPlayTimeMs;
            }
            else
            {
                u32SeekCurTime = u32CurPlayTime;
            }

            s32Ret = PVR_Index_SeekByTime(pChnAttr->IndexHandle, pPosition->s64Offset, pPosition->s32Whence, u32SeekCurTime);
            if (s32Ret == HI_SUCCESS)
            {
                memset(&stReadFrame, 0 ,sizeof(PVR_INDEX_ENTRY_S));

                s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stReadFrame, pChnAttr->IndexHandle->u32ReadFrame);
                if (HI_SUCCESS != s32Ret)
                {
                    if (HI_ERR_PVR_FILE_TILL_END == s32Ret)
                    {
                        s32Ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stReadFrame, pChnAttr->IndexHandle->u32ReadFrame-1);
                        if (HI_SUCCESS != s32Ret)
                        {
                            PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
                            HI_ERR_PVR("Can't get EndFrame:%d\n", pChnAttr->IndexHandle->u32ReadFrame);
                            return s32Ret;
                        }
                    }
                    else
                    {
                        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
                        HI_ERR_PVR("Can't get EndFrame:%d\n", pChnAttr->IndexHandle->u32ReadFrame);
                        return s32Ret;
                    }
                }

                pChnAttr->u32CurPlayTimeMs = stReadFrame.u32DisplayTimeMs;
            }
            break;
        default:
            s32Ret = HI_ERR_PVR_INVALID_PARA;
    }
    if (HI_SUCCESS == s32Ret)
    {
        pChnAttr->stLastStatus.u32CurPlayTimeInMs = pChnAttr->u32CurPlayTimeMs;
        (HI_VOID)HI_SYS_GetTimeStampMs(&(pChnAttr->u32SeekSysTime));
    }
    pChnAttr->bEndOfFile = HI_FALSE;

    /*Only in palyback mode, seek to end, set decode mode to I frame only, to avoid other frames in the last GOP cause decode error
        set decode mode back to normal in 2 case, 
        in PVRPlayProcessEos, after all frame flush out, 
        in PVRPlayResetToCurFrame, before play state change or seek*/
    if ((SEEK_END == pPosition->s32Whence) && (!pChnAttr->IndexHandle->bIsRec))
    {
//        (HI_VOID)HI_UNF_AVPLAY_SetDecodeMode(pChnAttr->hAvplay, HI_UNF_VCODEC_MODE_I);
        pChnAttr->bSeekEnd = HI_TRUE;
    }

    HI_INFO_PVR("SEEK OK!\n");
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return s32Ret;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayStep
 Description     : play by step frame
 Input           : u32Chn        **channel number
                   direction  ** direction:forward or backward. presently, just only support backward.
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayStep(HI_U32 u32Chn, HI_S32 direction)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PVR_PLAY_CHN_S  *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    /* audio type index file, which not support step forward play */
    if (PVR_INDEX_IS_TYPE_AUDIO(pChnAttr->IndexHandle))
    {
        HI_ERR_PVR("audio stream NOT support step play!\n");
        PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
        return HI_ERR_PVR_NOT_SUPPORT;
    }

    s32Ret = PVRPlaySetStepMode(pChnAttr, direction);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_PVR("Set step play mode fail! ret=%#x\n", s32Ret);
    }

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return s32Ret;
}

HI_S32 HI_PVR_PlayRegisterReadCallBack(HI_U32 u32Chn, ExtraCallBack readCallBack)
{
    PVR_PLAY_CHN_S              *pChnAttr;
    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    pChnAttr->readCallBack = readCallBack;
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return HI_SUCCESS;
}

HI_S32 HI_PVR_PlayUnRegisterReadCallBack(HI_U32 u32Chn)
{
    PVR_PLAY_CHN_S *pChnAttr;

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    pChnAttr->readCallBack = NULL;

    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));
    return HI_SUCCESS;
}

/*****************************************************************************
 Prototype       : HI_PVR_PlayGetStatus
 Description     : get the status of play channel
 Input           : u32Chn      **channel number
 Output          : pStatus  **the status of channel
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/22
    Author       : quyaxin 46153
    Modification : Created function

*****************************************************************************/
HI_S32 HI_PVR_PlayGetStatus(HI_U32 u32Chn, HI_UNF_PVR_PLAY_STATUS_S *pStatus)
{
    HI_S32 ret;
    HI_U32 u32PtsPos;
    HI_U32 u32CurFrmTimeMs = 0;
    HI_U32 u32CurSysTime = 0;
    PVR_INDEX_ENTRY_S stCurPlayFrame = {0};   /* the current displaying frame info  */
    PVR_PLAY_CHN_S  *pChnAttr = HI_NULL;

    PVR_PLAY_CHECK_CHN(u32Chn);

    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);
    u32CurFrmTimeMs = PVRPlayGetCurPlayTime(pChnAttr);

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    if ((pChnAttr->enState == HI_UNF_PVR_PLAY_STATE_STEPF) ||
        (pChnAttr->enState == HI_UNF_PVR_PLAY_STATE_STEPB) ||
        (abs(pChnAttr->enSpeed)/HI_UNF_PVR_PLAY_SPEED_NORMAL > 8))
    {
        pStatus->u32CurPlayTimeInMs = u32CurFrmTimeMs;
    }
    else
    {
        pStatus->u32CurPlayTimeInMs = pChnAttr->u32CurPlayTimeMs;
    }

    if (HI_TRUE == pChnAttr->bTimeShiftStartFlg)
    {
        ret = PVR_Index_QueryFrameByTime(pChnAttr->IndexHandle, pChnAttr->u32CurPlayTimeMs, &stCurPlayFrame, &u32PtsPos);
    }
    else
    {
        ret = PVR_Index_QueryFrameByTime(pChnAttr->IndexHandle, u32CurFrmTimeMs, &stCurPlayFrame, &u32PtsPos);
    }
    
    if (HI_SUCCESS == ret)
    {
        pStatus->u32CurPlayTimeInMs = stCurPlayFrame.u32DisplayTimeMs;
        pStatus->u32CurPlayFrame = u32PtsPos;
        pStatus->u64CurPlayPos =  stCurPlayFrame.u64Offset;
    }
    else
    {
        memcpy(pStatus, &pChnAttr->stLastStatus, sizeof(HI_UNF_PVR_PLAY_STATUS_S));
        pStatus->enState = pChnAttr->enState;
        pStatus->enSpeed = pChnAttr->enSpeed;
    }

    /* for rewind timeshift, seek to start, the current play time may be less than start frame time (1~3S) issue */
    if ((pChnAttr->IndexHandle->stCycMgr.u32StartFrame >= pChnAttr->IndexHandle->stCycMgr.u32EndFrame) && 
        pChnAttr->IndexHandle->bIsRec)
    {
        PVR_INDEX_ENTRY_S stStartEntry = {0};

        ret = PVR_Index_GetFrameByNum(pChnAttr->IndexHandle, &stStartEntry, pChnAttr->IndexHandle->stCycMgr.u32StartFrame);
        if (HI_SUCCESS == ret)
        {
            if (pStatus->u32CurPlayTimeInMs < stStartEntry.u32DisplayTimeMs)
            {
                pStatus->u32CurPlayTimeInMs = stStartEntry.u32DisplayTimeMs;
                pChnAttr->u32CurPlayTimeMs = stStartEntry.u32DisplayTimeMs;
            }
        }
        else
        {
            HI_ERR_PVR("Can't get StartFrame:%d\n", pChnAttr->IndexHandle->stCycMgr.u32StartFrame);
        }
    }

    pStatus->enState = pChnAttr->enState;
    pStatus->enSpeed = pChnAttr->enSpeed;
	
    if (((HI_UNF_PVR_PLAY_STATE_FF == pChnAttr->enState)
            || (HI_UNF_PVR_PLAY_STATE_PLAY == pChnAttr->enState))
          &&(pChnAttr->stLastStatus.u32CurPlayTimeInMs > pStatus->u32CurPlayTimeInMs))
    {
        pStatus->u32CurPlayTimeInMs = pChnAttr->stLastStatus.u32CurPlayTimeInMs;
    }

    if ((HI_UNF_PVR_PLAY_STATE_FB == pChnAttr->enState)
        && (pChnAttr->stLastStatus.u32CurPlayTimeInMs < pStatus->u32CurPlayTimeInMs))
    {
        pStatus->u32CurPlayTimeInMs = pChnAttr->stLastStatus.u32CurPlayTimeInMs;
    }
    if (0 != pChnAttr->u32SeekSysTime)
    {
        (HI_VOID)HI_SYS_GetTimeStampMs(&u32CurSysTime);
        if (u32CurSysTime > pChnAttr->u32SeekSysTime + 1000)
        {
            pChnAttr->u32SeekSysTime = 0;
            memcpy(&pChnAttr->stLastStatus, pStatus, sizeof(HI_UNF_PVR_PLAY_STATUS_S));
        }
    }
    else
    {
        memcpy(&pChnAttr->stLastStatus, pStatus, sizeof(HI_UNF_PVR_PLAY_STATUS_S));
    }

    HI_WARN_PVR("========cur time:%d!\n", pStatus->u32CurPlayTimeInMs);
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return HI_SUCCESS;
}

HI_S32 HI_PVR_PlayGetFileAttr(HI_U32 u32Chn, HI_UNF_PVR_FILE_ATTR_S *pAttr)
{
    HI_S32 ret;
    PVR_PLAY_CHN_S *pChnAttr;

    PVR_CHECK_POINTER(pAttr);
    PVR_PLAY_CHECK_INIT(&g_stPlayInit);

    PVR_PLAY_CHECK_CHN(u32Chn);
    pChnAttr = &g_stPvrPlayChns[u32Chn];
    PVR_PLAY_CHECK_CHN_INIT(pChnAttr->enState);
    PVR_CHECK_POINTER(pChnAttr->IndexHandle);
    if(pChnAttr->IndexHandle->bIsRec)
    {
        PVR_Index_FlushIdxWriteCache(pChnAttr->IndexHandle);
    }

    PVR_LOCK(&(pChnAttr->stPlayChnMutex));
    PVR_PLAY_CHECK_CHN_INIT_UNLOCK(pChnAttr);

    ret = PVR_Index_PlayGetFileAttrByFileName(pChnAttr->stUserCfg.szFileName, pChnAttr->IndexHandle, pAttr);
    PVR_UNLOCK(&(pChnAttr->stPlayChnMutex));

    return ret;
}

HI_S32 HI_PVR_GetFileAttrByFileName(const HI_CHAR *pFileName, HI_UNF_PVR_FILE_ATTR_S *pAttr)
{
    HI_S32 ret;
    PVR_REC_CHN_S*  pstRecChnAttr = (PVR_REC_CHN_S *)NULL;
    PVR_PLAY_CHN_S* pstPlayChnAttr = (PVR_PLAY_CHN_S *)NULL;

    if (PVRRecCheckFileRecording(pFileName))
    {
        pstRecChnAttr = PVRRecGetChnAttrByName(pFileName);

        if (pstRecChnAttr != NULL)
        {
             PVR_Index_FlushIdxWriteCache(pstRecChnAttr->IndexHandle);
             ret = PVR_Index_PlayGetFileAttrByFileName(pFileName, pstRecChnAttr->IndexHandle, pAttr);
        }
        else
        {
            ret = PVR_Index_PlayGetFileAttrByFileName(pFileName, HI_NULL, pAttr);
        }
    }
    else
    {
        pstPlayChnAttr = (PVR_PLAY_CHN_S *)PVRPlayGetChnAttrByName(pFileName);

        if ((PVR_PLAY_CHN_S *)NULL != pstPlayChnAttr)
        {
            ret = PVR_Index_PlayGetFileAttrByFileName(pFileName, pstPlayChnAttr->IndexHandle, pAttr);
        }
        else
        {
            ret = PVR_Index_PlayGetFileAttrByFileName(pFileName, HI_NULL, pAttr);
        }
    }

    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

