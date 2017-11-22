/*****************************************************************************
* Copyright (c) 2005,SULDVB
* All rights reserved.
* 
* FileName ��ErrorCode.h
* Description ��Global define for error code base
* 
* Version ��Version ��1.0.0
* Author ��Jason   Reviewer : 
* Date ��2005-01-12
* Record : Change it into standard mode
*
* History :
* (1) 	Version ��1.0.0
* 		Author ��Jason   Reviewer : 
* 		Date ��2005-01-12
*		Record : Create File
****************************************************************************/
#include "string.h"
#include "mapi_inner.h"

#include "hi_adp_boardcfg.h"
#include "hi_unf_ecs.h"
#include "hi_unf_frontend.h"



/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define m_LockWaitTime                   1000
#define NIM_NOTIFY_TASK_STK_SIZE         (1024*30)

#define m_TunerTaskIdle                   0
#define m_TunerTaskChecking                   1

#if 0
#define MLTUNER_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLTUNER_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLTUNER_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLTUNER_DEBUG(fmt, args...) {} //printf("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#endif


typedef MBT_VOID (*MLMF_pLockCallBack_t)(MMT_TUNER_TunerStatus_E eTunerState,MBT_U32 u32TunerID,MBT_U32 u32Frenq,MBT_U32 u32Sym, MMT_TUNER_QamType_E eQamType);

typedef struct _m_tuner_msg_t
{
    MLMF_pLockCallBack_t  tuner_Callback;
    MBT_U32 u32TunerState;
    MBT_U32 u32Frenq;
    MBT_U32 u32Sym;
    MBT_U32 u32Qam;
} MST_TUNER_MSG;

static HI_UNF_TUNER_LOCK_STATUS_E gTunerStatus = HI_UNF_TUNER_SIGNAL_BUTT;
static HI_UNF_TUNER_SIG_TYPE_E g_enSigType[HI_TUNER_NUMBER];

static MBT_U8   mv_Tuner_u8Initalised = 0;
static MLMF_pLockCallBack_t  mf_Tuner_pLockCall = NULL;
static MBT_U32   mv_Tuner_u32SymbolRate = 6875;
static MBT_U32   mv_Tuner_u32TransFrenq = 306;
static MMT_TUNER_QamType_E mv_Tuner_eQamType = MM_TUNER_QAM_64;

static HI_S32 _MLMF_Tuner_GetConfig(HI_U32 TunerId, HI_UNF_TUNER_ATTR_S *TunerAttr)
{

    switch (TunerId)
    {
        case 0:
        {
            TunerAttr->enSigType        = HI_TUNER_SIGNAL_TYPE;
            TunerAttr->enTunerDevType   = HI_TUNER_TYPE;
            TunerAttr->u32TunerAddr     = HI_TUNER_DEV_ADDR;
            TunerAttr->enDemodDevType   = HI_DEMOD_TYPE;
            TunerAttr->u32DemodAddr     = HI_DEMOD_DEV_ADDR;
            TunerAttr->enOutputMode     = HI_DEMOD_TS_MODE;
            TunerAttr->u32ResetGpioNo   = HI_DEMOD_RESET_GPIO;
			TunerAttr->unTunerAttr.stTer.u32ResetGpioNo = HI_DEMOD_RESET_GPIO;
			TunerAttr->unTunerAttr.stSat.u32ResetGpioNo = HI_DEMOD_RESET_GPIO;
            TunerAttr->enI2cChannel     = HI_DEMOD_I2C_CHANNEL;

#ifdef GET_MULTIMODE_DEMOD_CONFIG
            {
                GET_MULTIMODE_DEMOD_CONFIG(0,TunerAttr->stPortAttr);
            }
#endif

            break;
        }
#if ((HI_DEMOD_TYPE == 270) && (HI_TUNER_NUMBER == 2))
        case 1:
        {
            TunerAttr->enSigType        = HI_TUNER1_SIGNAL_TYPE;
            TunerAttr->enTunerDevType   = HI_TUNER1_TYPE;
            TunerAttr->u32TunerAddr     = HI_TUNER1_DEV_ADDR;
            TunerAttr->enDemodDevType   = HI_DEMOD1_TYPE;
            TunerAttr->u32DemodAddr     = HI_DEMOD1_DEV_ADDR;
            TunerAttr->enOutputMode     = HI_DEMOD1_TS_MODE;
            TunerAttr->u32ResetGpioNo   = HI_DEMOD1_RESET_GPIO;
            TunerAttr->enI2cChannel     = HI_DEMOD1_I2C_CHANNEL;

#ifdef GET_MULTIMODE_DEMOD_CONFIG
            {
                GET_MULTIMODE_DEMOD_CONFIG(1,TunerAttr->stPortAttr);
            }
#endif
            break;
        }
#endif        
        default:
        {
            MLTUNER_DEBUG("[%s] tunerid %u error\n", __FUNCTION__, TunerId);
            return HI_FAILURE;
        }
    }


    MLTUNER_DEBUG("[%s] TunerId=%u, TunerType=%u, TunerAddr=0x%x, I2cChannel=%u, DemodAddr=0x%x, DemodType=%u, OutputMode=%d\n",
        __FUNCTION__, TunerId, TunerAttr->enTunerDevType, TunerAttr->u32TunerAddr,
        TunerAttr->enI2cChannel, TunerAttr->u32DemodAddr, TunerAttr->enDemodDevType, TunerAttr->enOutputMode);

    return HI_SUCCESS;
}


static HI_S32 _MLMF_Tuner_Init(HI_VOID)
{
    HI_S32   Ret;
    HI_UNF_TUNER_ATTR_S          TunerAttr;
    HI_U32 i = 0;


    /* Initialize Tuner*/
    Ret = HI_UNF_TUNER_Init();
    if (HI_SUCCESS != Ret)
    {
        MLTUNER_DEBUG("call HI_UNF_TUNER_Init failed.\n");
        return Ret;
    }

    for (i = 0; i < HI_TUNER_NUMBER; i++)
    {
        /* open Tuner*/
        Ret = HI_UNF_TUNER_Open(TUNER_USE);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_Open failed.\n");
            HI_UNF_TUNER_DeInit();
            return Ret;
        }

        /* set attr */
        /*get default attribute in order to set attribute next*/
        Ret = HI_UNF_TUNER_GetDeftAttr(i, &TunerAttr);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_GetDeftAttr failed.\n");
            HI_UNF_TUNER_Close(TUNER_USE);
            HI_UNF_TUNER_DeInit();
            return Ret;
        }

        Ret = _MLMF_Tuner_GetConfig(i, &TunerAttr);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("[%s] HIADP_Tuner_GetConfig failed 0x%x\n", __FUNCTION__, Ret);
            break;
        }
        
        MLTUNER_DEBUG("****************************************************************************\n");
        MLTUNER_DEBUG("TunerId=%u, TunerType=%u, TunerAddr=0x%x, I2cChannel=%u, DemodAddr=0x%x, \nDemodType=%u,SigType:%u,OutputMode:%u,ResetGpioNo:%u\n",
             i, TunerAttr.enTunerDevType, TunerAttr.u32TunerAddr,
            TunerAttr.enI2cChannel, TunerAttr.u32DemodAddr, TunerAttr.enDemodDevType,
            TunerAttr.enSigType,TunerAttr.enOutputMode,TunerAttr.u32ResetGpioNo);
        MLTUNER_DEBUG("****************************************************************************\n");
        
        Ret = HI_UNF_TUNER_SetAttr(i, &TunerAttr);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_SetAttr failed.\n");
            HI_UNF_TUNER_Close(TUNER_USE);
            HI_UNF_TUNER_DeInit();
            return Ret;
        }

        g_enSigType[i] = TunerAttr.enSigType;
    }

    /* set 0 spec attr */
#ifdef GET_SAT_TUNER_CONFIG
    {
    	HI_UNF_TUNER_SAT_ATTR_S   stSatTunerAttr;
		
        GET_SAT_TUNER_CONFIG(stSatTunerAttr);
        Ret = HI_UNF_TUNER_SetSatAttr(TUNER_USE, &stSatTunerAttr);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_SetAttr failed.\n");
            return Ret;
        }
    }
#endif

#ifdef GET_TER_TUNER_CONFIG
    {
        HI_UNF_TUNER_TER_ATTR_S     stTerTunerAttr;

        GET_TER_TUNER_CONFIG(stTerTunerAttr);
        Ret = HI_UNF_TUNER_SetTerAttr(TUNER_USE, &stTerTunerAttr);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_SetTerAttr failed.\n");
            return Ret;
        }
    }
#endif

#ifdef GET_CAB_TUNER_CONFIG
    {
        if (TunerAttr.enDemodDevType == HI_UNF_DEMOD_DEV_TYPE_3138)
        {
            HI_UNF_TUNER_CAB_ATTR_S stCabTunerAttr;

            GET_CAB_TUNER_CONFIG(stCabTunerAttr);        
            Ret = HI_UNF_TUNER_SetCabAttr(TUNER_USE, &stCabTunerAttr);
            if (HI_SUCCESS != Ret)
            {
                MLTUNER_DEBUG("call HI_UNF_TUNER_SetCabAttr failed.\n");
                return Ret;
            }
        }
    }
#endif

    if(HI_TUNER_NUMBER == 2)
    {
    /* set 1 spec attr */
#if (HI_DEMOD1_TYPE == 270)
#ifdef GET_SAT_TUNER1_CONFIG
    {
    	HI_UNF_TUNER_SAT_ATTR_S   stSatTunerAttr1;
		
        GET_SAT_TUNER1_CONFIG(stSatTunerAttr1);
        Ret = HI_UNF_TUNER_SetSatAttr(TUNER1_USE, &stSatTunerAttr1);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_SetAttr failed.\n");
            return Ret;
        }
    }
#endif

#ifdef GET_TER_TUNER1_CONFIG
    {
        HI_UNF_TUNER_TER_ATTR_S     stTerTunerAttr1;

        GET_TER_TUNER1_CONFIG(stTerTunerAttr1);
        Ret = HI_UNF_TUNER_SetTerAttr(TUNER1_USE, &stTerTunerAttr1);
        if (HI_SUCCESS != Ret)
        {
            MLTUNER_DEBUG("call HI_UNF_TUNER_SetTerAttr failed.\n");
            return Ret;
        }
    }
#endif

#ifdef GET_CAB_TUNER1_CONFIG
    {
        if (TunerAttr.enDemodDevType == HI_UNF_DEMOD_DEV_TYPE_3138)
        {
            HI_UNF_TUNER_CAB_ATTR_S     stCabTunerAttr1;

            GET_CAB_TUNER1_CONFIG(stCabTunerAttr1);        
            Ret = HI_UNF_TUNER_SetCabAttr(TUNER1_USE, &stCabTunerAttr1);
            if (HI_SUCCESS != Ret)
            {
                MLTUNER_DEBUG("call HI_UNF_TUNER_SetCabAttr failed.\n");
                return Ret;
            }
        }
    }
#endif
#endif
    }

    return HI_SUCCESS;
}


static HI_S32 _MLMF_Tuner_Connect(HI_U32 TunerID,HI_U32 Freq,HI_U32 SymbolRate,HI_U32 QamType)
{
    HI_UNF_TUNER_CONNECT_PARA_S  ConnectPara;

    ConnectPara.enSigType = g_enSigType[TunerID];
    ConnectPara.unConnectPara.stCab.bReverse = 0;
    ConnectPara.unConnectPara.stCab.u32Freq = Freq * 1000;
    ConnectPara.unConnectPara.stCab.u32SymbolRate = SymbolRate * 1000;
    switch (QamType)
    {
        case 16 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_16;
            break;
        case 32 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_32;
            break;
        case 64 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
            break;
        case 128 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_128;
            break;
        case 256 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_256;
            break;
        case 512 :
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_512;
            break;            
        default:
            ConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64; 
    }

    /* connect Tuner*/
    return HI_UNF_TUNER_Connect(TUNER_USE, &ConnectPara, 500);
}

static MBT_VOID _MLMF_Tuner_ServiceProcess(void)
{
	MMT_TUNER_TunerStatus_E _eTunerState = MM_TUNER_UNLOCKED;
	HI_UNF_TUNER_STATUS_S tunerStatus = {0};
	HI_U32 i = 0, j = 0;
	
	while (mv_Tuner_u8Initalised)
	{
		MLMF_Task_Sleep(500);
		HI_UNF_TUNER_GetStatus(TUNER_USE, &tunerStatus);

		if(tunerStatus.enLockStatus == HI_UNF_TUNER_SIGNAL_DROPPED)
		{
			for(j = 0; j < 2; j++)
			{
				MLMF_Task_Sleep(500);
				HI_UNF_TUNER_GetStatus(TUNER_USE, &tunerStatus);
			}
		}

		if (HI_UNF_TUNER_SIGNAL_LOCKED == tunerStatus.enLockStatus)
		{
			_eTunerState = MM_TUNER_LOCKED;
		}
		else
		{
			_eTunerState = MM_TUNER_UNLOCKED;
		}

		if(MM_TUNER_LOCKED == _eTunerState)
		{
			MLMF_FP_SetLock(1);
			MLTUNER_DEBUG("Tuner locked,Freq = %d",mv_Tuner_u32TransFrenq);
		}
		else
		{
			MLMF_FP_SetLock(0);
			MLTUNER_DEBUG("Tuner Unlocked, Freq = %d",mv_Tuner_u32TransFrenq);
		}
		
		if(NULL != mf_Tuner_pLockCall)
		{
			if (gTunerStatus != _eTunerState)
			{
				mf_Tuner_pLockCall(_eTunerState, 0, mv_Tuner_u32TransFrenq, mv_Tuner_u32SymbolRate, mv_Tuner_eQamType);
				gTunerStatus = _eTunerState;
			}
		}
	}
}


/*
*˵������һ��Ƶ�㣬Ϊ������ʽ���ã��˺���Ӧ�ڱ�֤�µ�Ƶ����������tuner�����뱣֤�˺��������󲻻��ٽ��յ���һ��Ƶ����κ�����������ǰ���·��أ����صȴ���Ƶ�������Ƶ���Ӧ��ͨ�����ûص���������֪�ϲ㡣
*���������
*u32TunerID: ���������tuner��ID �����ڶ��tuner����������ֻ��һ·tuner����0��
*u32Frenq�������������Ƶ�ʣ���λΪKHz��
*u32Sym������������Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875��
*stQam������������ǵ��Ʒ�ʽ��
*MBT_VOID (*TUNER_ScanCallback_T)(
*MMT_TUNER_TunerStatus_E stTunerState, 
*MBT_U32 u32TunerID��
*MBT_U32 iFrenq, 
*MBT_U32 iSym, 
*MMT_TUNER_QAMMode_E stQam)��Ϊ״̬���ص�����������������һ�ֳ����Ժ���ô˺������ص�ǰ״̬��ƽʱ����Ҫ��tuner״̬���м�أ�ÿ�������ӿ���һ�Σ��������ǰ״̬Ϊ����״̬�����κβ����������ǰ״̬Ϊδ����״̬����ô˺�����֪�ϲ�ͬʱ��������������Ƶ���˻ص�������ÿ�ε�����Ƶ����MLMF_Tuner_Lockʱ���п��ܸ�����������������£�
*u32TunerID: �������������MLMF_Tuner_Lock����ĵ�ID��
*u32Frenq�������������Ƶ�ʣ���λΪKHz������MLMF_Tuner_Lock����ĵ�Ƶ�ʡ�
*u32Sym������������Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875������MLMF_Tuner_Lock����ĵķ����ʡ�
*stQam������������ǵ��Ʒ�ʽ������MLMF_Tuner_Lock����ĵ��Ʒ�ʽ��
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Tuner_Lock(MBT_U32 u32TunerID, 
                                                MBT_U32 u32Frenq, 
                                                MBT_U32 u32Sym, 
                                                MMT_TUNER_QamType_E eQamType,
                                                MBT_VOID (*TUNER_ScanCallback_T)(
                                                                                    MMT_TUNER_TunerStatus_E stTunerState, 
                                                                                    MBT_U32 u32TunerID,
                                                                                    MBT_U32 u32Frenq, 
                                                                                    MBT_U32 u32Sym, 
                                                                                    MMT_TUNER_QamType_E eQamType)
                                                    )
{
	HI_U32 _QamType = 64;
	
	if((mv_Tuner_eQamType != eQamType)
		||(mv_Tuner_u32SymbolRate != u32Sym)
		||(mv_Tuner_u32TransFrenq != u32Frenq))
	{
		switch(eQamType)
		{
			case MM_TUNER_QAM_16:
			_QamType = 16;
			break;
			case MM_TUNER_QAM_32:
			_QamType = 32;
			break;
			case MM_TUNER_QAM_64:
			_QamType = 64;
			break;
			case MM_TUNER_QAM_128:
			_QamType = 128;
			break;
			case MM_TUNER_QAM_256:
			_QamType = 256;
			break;
			case MM_TUNER_QAM_MAX:
			_QamType = 64;
			break;
		}

		_MLMF_Tuner_Connect(TUNER_USE,u32Frenq,u32Sym,_QamType);

	}

	mv_Tuner_u32SymbolRate = u32Sym;
	mv_Tuner_u32TransFrenq = u32Frenq;
	mv_Tuner_eQamType = eQamType,
	mf_Tuner_pLockCall  = TUNER_ScanCallback_T;
	gTunerStatus = HI_UNF_TUNER_SIGNAL_BUTT;
	return MM_NO_ERROR;
}

/*
*˵������ȡtuner��״̬���������Ҫȡ��صĲ�����ֱ�Ӵ�MBT_NULL��
*���������
*			u32TunerID�����������tuner��ID��
*�������:
*			�ޡ�			
*		���أ�
*			����״̬��
*/                                                                                                                                            
MMT_TUNER_TunerStatus_E MLMF_Tuner_GetLockStatus(MBT_U32 u32TunerID)
{
	HI_S32 ret = HI_SUCCESS;
	MMT_TUNER_TunerStatus_E eTunerState = MM_TUNER_UNLOCKED;
	HI_UNF_TUNER_STATUS_S tunerStatus = {0};
	
	ret = HI_UNF_TUNER_GetStatus(TUNER_USE, &tunerStatus);
	
	if(HI_SUCCESS == ret && HI_UNF_TUNER_SIGNAL_LOCKED == tunerStatus.enLockStatus)
	{
		eTunerState = MM_TUNER_LOCKED;
	}
	
	return eTunerState;
}

/*
*˵������ȡtuner��״̬���������Ҫȡ��صĲ�����ֱ�Ӵ�MBT_NULL��
*���������
*			u32TunerID�����������tuner��ID��
*�������:
*			signal_ber����������� Ƶ���������, Bit error rate (x 10000000)��Ϊһ����������Ԫ�ص��������顣
*			Signal_berָ��ǰTUNER �����ʵ�ָ�롣��ָ��ָ��һ����������Ԫ�ص����飬����Ԫ�غ�������:
*				 signal_ber[0]:�����ʵ�������������
*				 signal_ber[1]:�����ʵ�����С�����ֳ���1000
*				 signal_ber[2]:������ָ������ȡ����ֵ(���ֵΪ����������Ϊ�����ʲ����ܴ���1) ����:������Ϊ2.156E-7��
*				 ��ô����Ԫ�ص�ȡֵ�ֱ�Ϊ 2��156��7��
*			signal_quality������������ź����� , Carrier to noise ratio (dB x 10)��
*			signal_strength������������ź�ǿ��, Power of the RF signal (dBm x 10)��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Tuner_GetStatus(MBT_U32 u32TunerID, MBT_U32 *signal_strength,MBT_U32 *signal_quality, MBT_U32 *signal_ber)
{
	HI_S32 ret = HI_SUCCESS;
	HI_U32 ber[3] = {0};
	HI_U32 snr = 0;
	HI_U32 strength = 0;
	HI_UNF_TUNER_STATUS_S tunerStatus = {0};

	*signal_quality = 0;
        *signal_strength = 0;
        *signal_ber = 0;
	
	ret = HI_UNF_TUNER_GetStatus(TUNER_USE, &tunerStatus);
	
	if(HI_SUCCESS == ret && HI_UNF_TUNER_SIGNAL_LOCKED == tunerStatus.enLockStatus)
	{
		if (NULL != signal_ber)
		{
			HI_UNF_TUNER_GetBER(TUNER_USE, ber);//������
			signal_ber[0] = ber[0];
			signal_ber[1] = ber[1];
			signal_ber[2] = ber[2];
		}

		if (NULL != signal_quality)
		{
			HI_UNF_TUNER_GetSNR(TUNER_USE, &snr);//�����
			*signal_quality = snr;
		}

		if (NULL != signal_strength)
		{
			HI_UNF_TUNER_GetSignalStrength(TUNER_USE, &strength);//ǿ��
			*signal_strength = strength;
		}
	}
	
	return MM_NO_ERROR;
}

/*
*˵������ȡ��ǰƵ����Ϣ��
*���������
*u32TunerID: ���������tuner��ID �����ڶ��tuner����������ֻ��һ·tuner����0��
*�������:
*u32Frenq����Ƶ�ʣ���λΪKHz��
*u32Sym���Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875��
*pu32Qam���ǵ��Ʒ�ʽ��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Tuner_CurTrans(MBT_U32 u32TunerID, MBT_U32 * pu32Frenq, MBT_U32 * pu32Sym, MMT_TUNER_QamType_E* peQamType)
{
	if(NULL != pu32Frenq)
	{
	    *pu32Frenq = mv_Tuner_u32TransFrenq;
	    MLTUNER_DEBUG("----> Freq = %d, mv_Tuner_u32TransFrenq = %d",*pu32Frenq, mv_Tuner_u32TransFrenq);
	}

	if(NULL != pu32Sym)
	{
	    *pu32Sym = mv_Tuner_u32SymbolRate;
	}

	if(NULL != peQamType)
	{
	    *peQamType = mv_Tuner_eQamType;
	}
	return MM_NO_ERROR;
}

/*
u8Mute == 1  �ص����� 
u8Mute == 0  �򿪻��� 
����Ĭ���ǹص��ģ�����û������˴򿪣���Ҫ�Գ�ʼ��ʱ��
*/
MBT_VOID MLMF_Tuner_SetLoopOutMode(MBT_U8 u8Mute)
{
	
}

MMT_STB_ErrorCode_E MLF_TunerInit(MBT_VOID)
{
	MET_Task_T _TunerTaskID;
	_MLMF_Tuner_Init();

	mv_Tuner_u8Initalised = 1;
	MLMF_Task_Create(_MLMF_Tuner_ServiceProcess,
                                            NULL,
                                            NIM_NOTIFY_TASK_STK_SIZE,
                                            7,
						  &_TunerTaskID,
                                            "TUNER_MON_TASK");
	return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E MLF_TunerTerm(MBT_VOID)
{
	HI_UNF_TUNER_Close(TUNER_USE);
	HI_UNF_TUNER_DeInit();
	return MM_NO_ERROR;
}

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */


