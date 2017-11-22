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
#include "inttypes.h"
#include <sys/stat.h>
#include <mqueue.h>
#include "mapi_inner.h"


#if 0
#define MLMSG_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLMSG_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLMSG_ERR(fmt, args...)  {}
#define MLMSG_DEBUG(fmt, args...) {}
#endif

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)



/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct msg_s
{
	mqd_t mqd;
	char msgname[20];
} msg_t;

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
MMT_STB_ErrorCode_E MLMF_Msg_Create(MBT_U32 msgSize,MBT_U32 msgCount,MET_Msg_T *msgID)
{
	static unsigned int s_count=0;
	char _mq_name[14]={0};
	struct mq_attr _attr={0};
	int _oflag=0;
	msg_t *_pmq = NULL;

	_pmq = (msg_t*)MLMF_Malloc(sizeof(msg_t));
	
	if(NULL == _pmq)
	{
		return MM_ERROR_NO_MEMORY;
	}

	sprintf(_mq_name,"/linuxmq%ld",s_count++);

	*msgID = MM_INVALID_HANDLE;
	_oflag = O_CREAT | O_RDWR;
	_attr.mq_maxmsg=msgCount;
	_attr.mq_msgsize=msgSize;
	
	_pmq->mqd = mq_open(_mq_name,_oflag,FILE_MODE,&_attr);
	
	if(-1 == _pmq->mqd)
	{
		MLMF_Free(_pmq);
		return MM_ERROR_UNKNOW;
	}
	strcpy(_pmq->msgname,_mq_name);
	*msgID = _pmq;
	
	return MM_NO_ERROR;
}


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
MMT_STB_ErrorCode_E MLMF_Msg_Destroy(MET_Msg_T msgID)
{
	msg_t *_pmq = NULL;

	if (MM_INVALID_HANDLE == msgID||NULL == msgID)
	{
           return MM_ERROR_BAD_PARAMETER;
	}

	_pmq = (msg_t *)msgID;
	mq_close(_pmq->mqd);
	mq_unlink(_pmq->msgname);

	return MM_NO_ERROR;
}


/*
*˵�����ȴ�һ����Ϣ�������Ϣ����������Ϣ����ֱ��ȡ����Ϣ������ȴ�waitTime
*��ʱ�䣬ֱ����ʱ��������Ϣ���
*���������
*				msgID����Ϣ���е�ID��
*u32BufLen����Ŵ���Ϣ��buf���ȡ�
*waitTime����Ϣ�ȴ���ʱ��,��λms��waitTime = MM_SYS_TIME_INIFIE��ʾ���õȴ�,waitTime = MM_SYS_TIME_IMEDIEA��ʾ�������ء�
*�������:
*buf��������Ϣ�Ļ��档������Ҫ��֤����Ĵ�С�ܷŵ���һ����Ϣ��
*			���أ�
*				MM_NO_ERROR:�ɹ���
*				����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Msg_WaitMsg(MET_Msg_T msgID,MBT_VOID *buf,MBT_U32 msgSize,MBT_U32 waitTime)
{
	struct timespec _ts;
	struct mq_attr _attr;
	clock_t _total_msec=0;
	int _err=0;
	msg_t *_pmq = NULL;

	unsigned int _sec=0,_msec=0;

	if (MM_INVALID_HANDLE == msgID||NULL == msgID)
	{
           return MM_ERROR_BAD_PARAMETER;
	}

	if (NULL == buf)
	{
		return MM_ERROR_BAD_PARAMETER;
	}

	_pmq = (msg_t *)msgID;
	
	if(MM_SYS_TIME_INIFIE == waitTime)
	{
		mq_getattr(_pmq->mqd, &_attr);
		mq_receive(_pmq->mqd, buf, _attr.mq_msgsize, 0);

		return MM_NO_ERROR;
	}

	if(MM_SYS_TIME_IMEDIEA == waitTime)
	{
		_total_msec = 0;
	}
	else
	{
		_total_msec = waitTime;
	}

	if(clock_gettime(CLOCK_REALTIME, &_ts) == -1)
	{
		return MM_ERROR_TIMEOUT;
	}

	_msec =	_total_msec;
	_sec=_msec/1000; //get second count;
	_msec = _msec%1000;

	_ts.tv_sec +=  _sec;
	_ts.tv_nsec += (_msec*1000*1000);
	_ts.tv_sec += _ts.tv_nsec/(1000 * 1000 * 1000);
	_ts.tv_nsec %= 1000 * 1000 * 1000;
    
	mq_getattr(_pmq->mqd, &_attr);
	
	_err = mq_timedreceive(_pmq->mqd, buf, _attr.mq_msgsize,0, &_ts);
	
	if(-1 == _err)
	{
		return MM_ERROR_TIMEOUT;
	}
	
	return MM_NO_ERROR;
}



/*
*˵��������һ����Ϣ����Ϣ�����У����������û�п������Ϣ���ã�
*��һֱ�ȴ���ֱ���������뵽�µ���Ϣ�ٷ��͡�
*���������
*			msgID�������������Ϣ���е�ID��
*			buf�����������������Ϣ�Ļ��档
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR���ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_Msg_SendMsg(MET_Msg_T msgID,MBT_VOID * buf,MBT_U32 msgSize,MBT_U32 waitTime)
{
	int err = 0;
	struct mq_attr _attr;
	clock_t _total_msec=0;
	unsigned int _sec=0,_msec=0;
	struct timespec _ts;
	msg_t *_pmq = NULL;
	
	if (MM_INVALID_HANDLE == msgID||NULL == msgID)
	{
       	return MM_ERROR_BAD_PARAMETER;
	}

	if (NULL == buf)
	{
		return MM_ERROR_BAD_PARAMETER;
	}
	
	_pmq = (msg_t *)msgID;
	if (MM_SYS_TIME_INIFIE == waitTime)
	{
		mq_getattr(_pmq->mqd, &_attr);
		mq_send(_pmq->mqd, buf, _attr.mq_msgsize, 0);
		return MM_NO_ERROR;
	}
	if (MM_SYS_TIME_IMEDIEA == waitTime)
	{
		_total_msec = 0;
	}
	else
	{
		_total_msec = waitTime;
	}

	if (clock_gettime(CLOCK_REALTIME, &_ts) == -1)
	{
		return MM_ERROR_TIMEOUT;
	}

	_msec =	_total_msec;
	_sec=_msec/1000; //get second count;
	_msec = _msec%1000;
	
	_ts.tv_sec +=  _sec;
	_ts.tv_nsec += (_msec*1000*1000);
	_ts.tv_sec += _ts.tv_nsec/(1000 * 1000 * 1000);
	_ts.tv_nsec %= 1000 * 1000 * 1000;
	mq_getattr(_pmq->mqd, &_attr);
	err = mq_timedsend(_pmq->mqd, buf,
     						_attr.mq_msgsize, 0, &_ts);

       if (-1 == err)
       {
            return MM_ERROR_TIMEOUT;
     	}

	return MM_NO_ERROR;
}




/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

/*----------------------End of ErrorCode.h-------------------------*/

