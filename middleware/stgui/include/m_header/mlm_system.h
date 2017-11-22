/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_system.h
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

#ifndef	__MLM_SYSTEM_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_SYSTEM_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum _MM_STANDBY_MODE_
{
    MM_STANDBY_MODE_NORMAL,
    MM_STANDBY_MODE_FAST,
    MM_STANDBY_MODE_FAKE,
    MM_STANDBY_MODE_MAX
}MM_STANDBY_MODE;


/*
*˵������ȡ��ǰOSʱ�䡣
*���������
*�ޡ�
*�������: 
*�ޡ�
*���أ�
*��ǰϵͳ���еĺ�����(�������������0��ʼ�ؼ�)��
*/
extern MBT_U32 MLMF_SYS_GetMS(MBT_VOID);

extern MBT_VOID MLMF_SYS_DcacheFlush(MBT_VOID *p_addr, MBT_U32 len);
extern MBT_VOID MLMF_SYS_DcachInvalid(MBT_VOID *p_addr, MBT_U32 len);
extern MBT_VOID *MLMF_SYS_CacheAddr2NCach(MBT_VOID *p_addr);

extern MBT_BOOL MLMF_Get_ChipID(MBT_U8 * pbyChipID,MBT_U8 * pbyChipIDLen);

/*
*˵�������ô˺���ι�����Ź����ڲ���������ʱ���������ι�����Զ�������
*�������ΪMM_SYS_TIME_INIFIE���ʾ�رտ��Ź������ΪMM_SYS_TIME_IMEDIEA���ʾ����������
*���������
*u32TimeOut��ι����ʱ�䣬��λΪms��
*�������: 
*�ޡ�
*���أ�
*�ޡ�
*/
extern MBT_VOID MLMF_SYS_WatchDog(MBT_U32 u32TimeOut);

extern MMT_STB_ErrorCode_E MLMF_SYS_ResetSecModule(MBT_VOID);

/*
*˵����ϵͳ��ӡ������
*���������
*��ӡ������
*�������: 
*�ޡ�
*���أ�
*�ޡ�
*/
extern MBT_VOID MLMF_Print(const MBT_CHAR* fmt, ...);



/*
*˵��������������
*���������
*��
*�������: 
*�ޡ�
*���أ�
*�ޡ�
*/
extern MBT_VOID MLMF_SYS_EnterStandby(MM_STANDBY_MODE eMode,MBT_U32 u32WakeupTime,MBT_U32 u32Powerkey);

extern MBT_VOID MLMF_SYS_FakeWakeup(MBT_VOID);

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_SYSTEM_H__ */

/*----------------------End of mlm_system.h-------------------------*/

