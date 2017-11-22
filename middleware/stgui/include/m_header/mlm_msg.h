/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_msg.h
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
#ifndef	__MLM_MSG_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_MSG_H__
/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef MBT_U32 MET_Msg_T;

/*
*˵��������һ����Ϣ���С�
*���������
*			msgSize��ÿ����Ϣ�Ĵ�С����λ�ֽڣ�byte����
*			msgCount����Ϣ������Ϣ����������
*�������:
*msgID����Ϣ���д����ɹ����ش������ID������ʧ�ܷ���MM_INVALID_HANDLE��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Msg_Create(MBT_U32 msgSize,MBT_U32 msgCount,MET_Msg_T *msgID);

/*
*˵��������һ����Ϣ����
*���������
*			msgID��Ҫ���ٵ���Ϣ���е�ID��
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR���ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Msg_Destroy(MET_Msg_T msgID);

/*
*˵�����ȴ�һ����Ϣ�������Ϣ����������Ϣ����ֱ��ȡ����Ϣ������ȴ�waitTime
*��ʱ�䣬ֱ����ʱ��������Ϣ���
*���������
*				msgID����Ϣ���е�ID��
*u32BufLen����Ŵ���Ϣ��buf���ȡ�
*waitTime����Ϣ�ȴ���ʱ��,��λms��waitTime = MM_SYS_TIME_INIFIE��ʾ���õȴ���
*�������:
*buf��������Ϣ�Ļ��档������Ҫ��֤����Ĵ�С�ܷŵ���һ����Ϣ��
*			���أ�
*				MM_NO_ERROR:�ɹ���
*				����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Msg_WaitMsg(MET_Msg_T msgID,MBT_VOID * buf,MBT_U32 msgSize,MBT_U32 waitTime);


/*
*˵��������һ����Ϣ����Ϣ�����У����������û�п������Ϣ���ã���һֱ�ȴ���ֱ���������뵽�µ���Ϣ�ٷ��͡�
*���������
*			msgID�������������Ϣ���е�ID��
*			buf�����������������Ϣ�Ļ��档
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR���ɹ���
*			����ţ�ʧ�ܡ�
*MM_SYS_TIME_IMEDIEA  0 
*MM_SYS_TIME_INIFIE  (0xFFFFFFFF)
*/
extern MMT_STB_ErrorCode_E MLMF_Msg_SendMsg(MET_Msg_T msgID,MBT_VOID * buf,MBT_U32 msgSize,MBT_U32 waitTime);

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_MSG_H__ */

/*----------------------End of mlm_msg.h-------------------------*/

