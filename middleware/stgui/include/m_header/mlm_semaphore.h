/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_semaphore.h
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
#ifndef	__MLM_SEMAPHORE_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_SEMAPHORE_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef MBT_U32 MET_Sem_T;

/*
*˵��������һ���ź�����
*���������
*		    Count������ʼ��������
*semID: ���ź��������ɹ����ش�������ź���ID����ʧ���򷵻�MM_INVALID_HANDLE��
*�������:
*		    �ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Sem_Create(MET_Sem_T *semID, MBT_U32 count);

/*
*˵��������һ���ź�����
*���������
*			semID��Ҫ���ٵ��ź��������
*�������:
*		    �ޡ�
*		���أ�
*		    MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Sem_Destroy(MET_Sem_T semID);

/*
*˵�����ȴ�һ���ź������ȴ��Ĺ���������������������涨��ʱ���ڻ�û�еȵ����ź������򷵻�MM_ERROR_TIMEOUT��ʱ�˳���
*���������
*			semID���Ϸ����ź���ID��
*waitTime���ź����ȴ���ʱ��ʱ�䣬��λ��ms��waitTime = MM_SYS_TIME_INIFIE��ʾ���õȴ���
*�������:
*		    �ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Sem_Wait(MET_Sem_T semID,MBT_U32 waitTime);

/*
*˵�����ͷ�һ���ź�����
*���������
*			semID��Ҫ�ͷŵ��ź���ID��
*�������:
*		    �ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Sem_Release(MET_Sem_T semID);
/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_SEMAPHORE_H__ */

/*----------------------End of mlm_semaphore.h-------------------------*/

