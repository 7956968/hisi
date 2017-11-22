/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_task.h
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

#ifndef	__MLM_TASK_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_TASK_H__


/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define M_MIN_USER_PRIORITY 0
#define M_MAX_USER_PRIORITY 31

typedef MBT_U32 MET_Task_T;

/*
*˵��������һ������
*���������
*Func:Ҫ�����������������Դ�������
*Param:�������Ĳ�����
*Stacksize:�����ջ��С��
*Priority:��������ȼ���С��0-15����
*name:��������ƣ�����ƽ̨֧�ֵ��ַ�������һ����ʹ�õ�ʱ����ò�Ҫ����10���ַ�����
*�������:
*taskID�����ش�����������������ɹ����ط�0ֵ��ʧ�ܷ���MM_INVALID_HANDLE��
*���أ�
*MM_NO_ERROR:�ɹ���
*����Ĵ���ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Task_Create(
                                    MBT_VOID (*func)(MBT_VOID *),
					MBT_VOID *param,
					MBT_U32 stacksize,
					MBT_U32 priority,
					MET_Task_T *taskID,
					const char *name) ;


/*
*˵��������һ�����񣬲�����ϵͳ��Դ��
*���������
*			taskID��Ҫ���ٵ�����ID��
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����Ĵ���ţ�ʧ�ܡ�
*
*/
extern MMT_STB_ErrorCode_E MLMF_Task_Destroy(MET_Task_T taskID);


extern MBT_VOID MLMF_Task_Exit(MBT_VOID);

/*
*˵������������ms���롣
*���������
*Ms:����Ҫ���ߵĺ�����������MM_SYS_TIME_INIFIE����Ϊ��������
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����Ĵ���ţ�ʧ�ܡ�
*/
extern MBT_VOID MLMF_Task_Sleep(MBT_U32 u32MilliSeconds);

extern MBT_VOID MLMF_Task_Lock(MBT_VOID);

extern MBT_VOID MLMF_Task_Unlock(MBT_VOID);

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_TASK_H__ */

/*----------------------End of mlm_task.h-------------------------*/

