/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_smartcard.h
* Description ��Global define for error code base
* 
* Version ��Version ��1.0.0
* Author ��wangyafeng : 
* Date ��2010-08-11
* Record : Change it into standard mode
*
* History :
* (1) 	Version ��1.0.0
* 		Author ��wangyafeng  : 
* 		Date ��2010-08-11
*		Record : Create File 
****************************************************************************/
#ifndef	__MLM_SMARTCARD_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_SMARTCARD_H__


/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/
/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
typedef enum _m_stcd_status_e
{
    MM_SMARTCARD_IN,
    MM_SMARTCARD_OUT,
    MM_SMARTCARD_ERROR
} MMT_STCD_Status_E;

typedef enum _m_stcd_standard_e
{
    MM_SMARTCARD_STANDARD_ISO7816,
    MM_SMARTCARD_STANDARD_NDS,
    MM_SMARTCARD_STANDARD_EMV96,
    MM_SMARTCARD_STANDARD_EMV2000,
    MM_SMARTCARD_STANDARD_ECHOCHAR_T,
    MM_SMARTCARD_STANDARD_UNDEFINE
} MMT_STCD_Standard_E;
	
typedef enum _m_stcd_protocol_e
{
    MM_SMARTCARD_PROTOCOL_T0,
    MM_SMARTCARD_PROTOCOL_T1,
    MM_SMARTCARD_PROTOCOL_T14,
    MM_SMARTCARD_PROTOCOL_UNDEFINE
} MMT_STCD_Protocol_E;

/*-----------------------Module Variables-------------------------*/

/*-----------------------Module Functions-------------------------*/
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
extern MMT_STB_ErrorCode_E  MLMF_SmartCard_SetNotify (MBT_S32 cardno, 
                                                                                                                                         MBT_VOID (*pSCStatusNotify)(
                                                                                                                                                    MBT_S32 cardno, 
                                                                                                                                                    MMT_STCD_Status_E status, 
                                                                                                                                                    MMT_STCD_Standard_E standard, 
                                                                                                                                                    MMT_STCD_Protocol_E protocol));


                                                                                                                                
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
extern MMT_STB_ErrorCode_E  MLMF_SmartCard_Reset(MBT_S32 cardno, MBT_U8 *msgATR,MBT_U32 *msgATRLen);

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
extern MMT_STB_ErrorCode_E  MLMF_SmartCard_GetStatus(MBT_S32 cardno, MMT_STCD_Status_E*status);

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
extern MMT_STB_ErrorCode_E  MLMF_SmartCard_TransferData(MBT_S32 cardno, 
                                                                    MBT_U8 *command,
                                                                    MBT_U32 commandLen, 
                                                                    MBT_U8 *recvBuffer, 
                                                                    MBT_U32 *recvLen, 
                                                                    MBT_U16 *statusWord);

/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_SMARTCARD_H__ */

/*----------------------End of mlm_smartcard.h-------------------------*/

