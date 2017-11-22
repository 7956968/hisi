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
    
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include <unistd.h>
#include <pthread.h>

#include "mapi_inner.h"


#define ClOCKS_PER_SECOND (1000)


static int ClkPerSec = 0;


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
MMT_STB_ErrorCode_E MLMF_Task_Create(MBT_VOID (*func)(MBT_VOID *),
                                            MBT_VOID *param,
                                            MBT_U32 stacksize,
                                            MBT_U32 priority,
                                            MET_Task_T *taskID,
                                            const MBT_CHAR *name)
{
	int _err=0;
	pthread_t _TsThd;
	pthread_attr_t _attr;
	struct sched_param  _sparam;

	*taskID = MM_INVALID_HANDLE;

	if(NULL == name)
	{
		return MM_ERROR_BAD_PARAMETER; 
	}
	
	pthread_attr_init(&_attr);
	pthread_attr_setschedpolicy(&_attr,SCHED_RR);

	//min = 1 ; max = 99
	if (priority < sched_get_priority_min(SCHED_RR))
		priority = sched_get_priority_min(SCHED_RR);

	if (priority > sched_get_priority_max(SCHED_RR))
		priority = sched_get_priority_max(SCHED_RR);

	_sparam.sched_priority = priority;
	pthread_attr_setschedparam(&_attr,&_sparam);

	_err = pthread_create(&_TsThd,&_attr,(void*)func,(void *)param);
	
	if(_err != 0)
	{
		return MM_ERROR_BAD_PARAMETER;
	}
	
	pthread_detach(_TsThd);//set thread auto free mem
	*taskID = _TsThd;
	
	return MM_NO_ERROR;
}


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
MMT_STB_ErrorCode_E MLMF_Task_Destroy(MET_Task_T taskID)
{
	if (MM_INVALID_HANDLE != taskID)
	{
		pthread_kill((pthread_t)taskID,0);
	}
	return MM_NO_ERROR;
}

MBT_VOID MLMF_Task_Exit(MBT_VOID)
{
    return;
}


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
MBT_VOID MLMF_Task_Sleep(MBT_U32 u32MilliSeconds)
{
	struct timespec x;
	if (ClkPerSec == 0)
		ClkPerSec = ClOCKS_PER_SECOND;
	/* if < 2ms */
	if (u32MilliSeconds <= (2 * ClkPerSec / 1000 ))
	{
		x.tv_sec  = 0;
		x.tv_nsec = 2000001;
	}
	else
	{
		x.tv_sec  = u32MilliSeconds / ClkPerSec;
		x.tv_nsec = (((u32MilliSeconds % ClkPerSec) * 1000000) / ClkPerSec) * 1000;
	}
	nanosleep (&x, 0);
	return ;
}

MBT_VOID MLMF_Task_Lock(MBT_VOID)
{
}

MBT_VOID MLMF_Task_Unlock(MBT_VOID)
{
}
/*----------------------End of ErrorCode.h-------------------------*/

