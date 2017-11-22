#include <string.h>
#include <stdarg.h>
#include "mapi_inner.h"


/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ClOCKS_PER_SECOND (1000)
#define SECONDS_ONE_DAY   (86400)           /* Restraints number of seconds to last day (86400s in 1 day) */

static MBT_S32 mlmv_s32PrintSem = -1;

/*
*˵������ȡ��ǰOSʱ�䡣
*���������
*�ޡ�
*�������: 
*�ޡ�
*���أ�
*��ǰϵͳ���еĺ�����(�������������0��ʼ�ؼ�)��
*/
MBT_U32 MLMF_SYS_GetMS(MBT_VOID)
{
	struct timeval   tv;
	clock_t          Clk = 1;	    /* Default value */
//	int              USecPerClk;    /* Micro seconds per clock (typically 64) */

//	USecPerClk = 1000000/ClOCKS_PER_SECOND;

	if (gettimeofday(&tv, NULL) == 0)
	{

		Clk = ((tv.tv_sec*1000000 + tv.tv_usec)/1000);
		/* Restraints number of seconds */
		/* This restriction is due to unsigned int maximum value which may be overflowed if no constraint was introduced */
	}
	return (Clk);
}


MBT_VOID MLMF_SYS_DcacheFlush(MBT_VOID *p_addr, MBT_U32 len)
{
	
}


MBT_VOID MLMF_SYS_DcachInvalid(MBT_VOID *p_addr, MBT_U32 len)
{
	
}


MBT_VOID *MLMF_SYS_CacheAddr2NCach(MBT_VOID *p_addr)
{
	
}


MBT_BOOL MLMF_Get_ChipID(MBT_U8 * pbyChipID,MBT_U8 * pbyChipIDLen)
{
	
}


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

MBT_VOID MLMF_SYS_WatchDog(MBT_U32 u32TimeOut)
{
	
}


MMT_STB_ErrorCode_E MLMF_SYS_ResetSecModule(MBT_VOID)
{
    return MM_ERROR_FEATURE_NOT_SUPPORTED;
}



/*
*˵����ϵͳ��ӡ������
*���������
*��ӡ������
*�������: 
*�ޡ�
*���أ�
*�ޡ�
*/
MBT_VOID MLMF_Print(const MBT_CHAR* fmt, ...)
{
	
}



/*
*˵�����������
*/

MBT_VOID MLMF_SYS_EnterStandby(MM_STANDBY_MODE eMode,MBT_U32 u32WakeupTime,MBT_U32 u32Powerkey)
{
	
}


MBT_VOID MLMF_SYS_FakeWakeup(MBT_VOID)
{
	
}


MMT_STB_ErrorCode_E MLF_SysInit(MBT_VOID)
{
	
}


/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */


