/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_malloc.h
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
#ifndef	__MLM_MOLLOC_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_MOLLOC_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
*˵��������ָ����С���ڴ档
*���������
*			Size�����������Ҫ����Ĵ�С��       
*�������:
*�ޡ�
*		���أ�
*			����ɹ��ڴ����ʼ��ַ�����߷���ʧ��MBT_ NULL��
*/
extern MBT_VOID * MLMF_Malloc(MBT_U32 size);

/*
*˵�����ͷ����뵽���ڴ档
*���������
*			Addr������������ͷŵ��ڴ�ָ�롣
*�������:
*�ޡ�
*		���أ�
*			�ޡ�
*/
extern MBT_VOID MLMF_Free(MBT_VOID *addr);

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_MOLLOC_H__ */

/*----------------------End of mlm_malloc.h-------------------------*/

