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
#include <stdio.h>
#include <stdlib.h>

#include "mapi_inner.h"


#define BLMALLOC_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)



/*
*˵��������ָ����С���ڴ档
*���������
*			Size�����������Ҫ����Ĵ�С��       
*�������:
*�ޡ�
*		���أ�
*			����ɹ��ڴ����ʼ��ַ�����߷���ʧ��MM_NULL��
*/
MBT_VOID * MLMF_Malloc(MBT_U32 size)
{
	return malloc(size);
}


/*
*˵�����ͷ����뵽���ڴ档
*���������
*			Addr������������ͷŵ��ڴ�ָ�롣
*�������:
*�ޡ�
*		���أ�
*			�ޡ�
*/
MBT_VOID MLMF_Free(MBT_VOID *addr)
{
	if (NULL != addr)
	{
		free(addr);
	}
}




MBT_VOID MLF_ShowMemInfo(MBT_VOID)
{
	
}



/*----------------------End of ErrorCode.h-------------------------*/

