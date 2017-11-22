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
#include "mlm_type.h"
#include "string.h"
#include "mapi_inner.h"


/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0
#define MLSMC_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLSMC_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLSMC_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLSMC_DEBUG(fmt, args...) {}
#endif


static MMT_STCD_Status_E mv_eSmartCardStatus = MM_SMARTCARD_OUT;
static MBT_VOID (*mlf_smartcard_SCStatusNotify)(MBT_S32 cardno,MMT_STCD_Status_E status, MMT_STCD_Standard_E standard, MMT_STCD_Protocol_E protocol) = NULL;
static MBT_BOOL mv_eSmartCardActived = MM_FALSE;

/*------------------------------------------------------------*/

/*
*˵��:�������ܿ�״̬�ı�֪ͨ�ص�������
*���������
*cardno�����ܿ���š�
*M_SC_STATUS_NOTIFY�����ܿ�״̬�ı�ص������������ܿ������γ�ʱ���ô˺�����֪ͨ�ϲ㡣
*�������:
*			�ޡ�
*���أ�
*	   MM_NO_ERROR:�ɹ���
*	   ����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_SmartCard_SetNotify (MBT_S32 cardno, 
                                                            MBT_VOID (*pSCStatusNotify)(
                                                                                    MBT_S32 cardno, 
                                                                                    MMT_STCD_Status_E status, 
                                                                                    MMT_STCD_Standard_E standard, 
                                                                                    MMT_STCD_Protocol_E protocol))
{
    mlf_smartcard_SCStatusNotify = pSCStatusNotify;  
    return MM_NO_ERROR;
}

                                                                                                                                
/*
*˵�������ܿ���λ��
*���������
*cardno�����ܿ���š�
*msgATR����λӦ���ַ�����
*msgATRLen��ATRӦ��ĳ��ȡ�
*�������:
*			�ޡ�
*���أ�
*	MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_SmartCard_Reset(MBT_S32 cardno, MBT_U8 *msgATR,MBT_U32 *msgATRLen)
{
	
}


/*
*˵��:��ȡ���ܿ����״̬��
*���������
*cardno�����ܿ���š�
*status�����ܿ�״̬��
*�������:
*			�ޡ�
*���أ�
*	MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_SmartCard_GetStatus(MBT_S32 cardno, MMT_STCD_Status_E *status)
{
    *status = mv_eSmartCardStatus;
    return MM_NO_ERROR;
}

/*
*˵��:���ܿ�����ͨѶ��
*���������
*cardno�����ܿ���š�
*command�����͵����ܿ�����档
*commandLen�����͵����ܿ�����ȡ�
*recvBuffer�����ص����ݻ��档
*recvLen��recvBuffer�ĳ��ȡ�
*statusWord��״̬�֡�
*�������:
*			�ޡ�
*���أ�
*	MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E  MLMF_SmartCard_TransferData(MBT_S32 cardno, 
                                                                    MBT_U8 *command,
                                                                    MBT_U32 commandLen, 
                                                                    MBT_U8 *recvBuffer, 
                                                                    MBT_U32 *recvLen, 
                                                                    MBT_U16 *statusWord)
{
	
}




MMT_STB_ErrorCode_E MLF_SmartCardInit(MBT_VOID)
{
	
}


MMT_STB_ErrorCode_E MLF_SmartCardTerm(MBT_VOID)
{
	
}


/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */



