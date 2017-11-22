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



/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */




/*
*˵�������ô��ڲ����������ʣ�У��λ��ֹͣλ������λ��
*���������
*			Baud: �����ʡ�
*			Pairty: У��λ��
*			Sp: ֹͣλ��
*			Db: ����λ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Uart_SetParam (MBT_U32 Baud, MBT_U32 Pairty, MBT_U32 Sp, MBT_U32 Db)
{
	
}

/*
*˵����������д���ݡ�
*���������
*			Buf: ���ݻ���ָ�롣
*			Len: ���ݳ��ȣ��ֽڡ�
*			Time_ms: ���ó�ʱʱ�䣬���룬time_ms=MM_SYS_TIME_INIFIE��ʾ���õȴ���
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Uart_Write(MBT_U8* Buf, MBT_U32 len, MBT_U32 *pu32WrtieLen,MBT_U32 time_ms)
{
	
}

/*
*˵�����Ӵ��ڶ�ȡ���ݡ�
*���������
*			Data: �����������Ŷ�ȡ���ݵĻ����ַ��
*			Wlen: �����������Ҫ��ȡ���ֽ�����
*			Alen: ���������ʵ�ʶ�ȡ���ֽ�����
*			Tm: �����������ʱʱ�䣬ms , Tm = MM_SYS_TIME_INIFIE��ʾ���õȴ���
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Uart_Read(MBT_U8 *data, MBT_U32 Wlen, MBT_U32 *Alen, MBT_U32 tm)
{
	
}



/*
*˵����ǿ�����UART�Ļ������ݡ�
*���������
*			Data: �����������Ŷ�ȡ���ݵĻ����ַ��
*			Wlen: �����������Ҫ��ȡ���ֽ�����
*			Alen: ���������ʵ�ʶ�ȡ���ֽ�����
*			Tm: �����������ʱʱ�䣬ms , Tm = MM_SYS_TIME_INIFIE��ʾ���õȴ���
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Uart_Flush(MBT_VOID)
{
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E MLMF_Uart_Init(MBT_VOID)
{
	return MM_NO_ERROR;
}

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

/*----------------------End of ErrorCode.h-------------------------*/

