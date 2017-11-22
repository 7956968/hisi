/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_uart.h
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
* 		Date ��2010-08-14
*		Record : Create File 
****************************************************************************/
#ifndef	__MLM_UART_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_UART_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/
/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/

/*-----------------------Module Variables-------------------------*/

/*-----------------------Module Functions-------------------------*/
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
extern MMT_STB_ErrorCode_E MLMF_Uart_SetParam (MBT_U32 Baud, MBT_U32 Pairty, MBT_U32 Sp, MBT_U32 Db);

/*
*˵����������д���ݡ�
*���������
*			Buf: ���ݻ���ָ�롣
*			Len: ���ݳ��ȣ��ֽڡ�
*			Time_ms: ���ó�ʱʱ�䣬���룬time_ms=0��ʾ���õȴ���
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Uart_Write(MBT_U8 * Buf,MBT_U32 len,MBT_U32 * pu32WrtieLen,MBT_U32 time_ms);

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
extern MMT_STB_ErrorCode_E MLMF_Uart_Read(MBT_U8 *data, MBT_U32 Wlen, MBT_U32 *Alen, MBT_U32 tm);

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
extern MMT_STB_ErrorCode_E MLMF_Uart_Flush(MBT_VOID);
/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_UART_H__ */

/*----------------------End of mlm_uart.h-------------------------*/

