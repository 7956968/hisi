/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_blast.h
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
#ifndef	__MLM_BLAST_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_BLAST_H__
/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef enum _M_IrStatus_e
{
	MM_BLAST_PRESS = 0,
	MM_BLAST_REPEAT,
	MM_BLAST_RELEASE
}MMT_BLAST_Status_E;


/*------------------------Module Macors---------------------------*/
/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
/*-----------------------Module Variables-------------------------*/

/*-----------------------Module Functions-------------------------*/
/*
*˵����ע�ᰴ���ص��������ڽ��յ�ң��������ʱ���˻ص�����������û����¼�300msû�зſ�����ʼ�ظ�������һ��ֵ��֮��ÿ�յ�һ��repeate�㷢��һ�Σ�ֱ���û�ֹͣ������
*��Ϊң���������в��죬������ʵ�ִ˹���ʱ��һ��Ҫ��֤���Խ���108ms���²���10%�ĺ����źţ�ͬʱҲҪ�Ѵ���λ���������32λ��ֵ�����ص�������
*���������
*pBCallback: ע��Ļص�����ָ�롣�ص������ĸ�ʽ��MBT_VOID BlastCallback(MBT_U32 key)������key���ǰ���ֵ��
*�������:
*    �ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MBT_VOID MLMF_Blast_RegisterCallback(MBT_VOID(* pBCallback)(MBT_U16 u16key,MMT_BLAST_Status_E eStatus));


extern MBT_VOID MLMF_Blast_SetSystemCode(MBT_U16 u16SystemCode);


/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_BLAST_H__ */

/*----------------------End of mlm_blast.h-------------------------*/

