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
#include "mapi_inner.h"
 

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static MBT_U8 mlmv_u8FpIsFD650 = 0; //1-fd650, 0-sar

static MBT_U8 s_u8StandbyMode = 0x0; /* 0 - normal, 1 - standby */
MMT_STB_ErrorCode_E MLF_FPInit(MBT_VOID)
{
	
}


MMT_STB_ErrorCode_E MLF_FPTerm(MBT_VOID)
{
	
}


/*
*˵����ˢ��ǰ��壬�˺����ᱻ�ϲ�ڸ�6ms����һ�Σ��Ա�֤ǰ���LED��������ʾ��
*���ǰ������м���оƬ����CPUˢ�£����԰Ѵ˺����ſա�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*		����ţ�ʧ�ܡ�
*/
MBT_VOID MLMF_FP_LedFresh (MBT_VOID)
{
    //no use
    return;
}

/*
*˵����������ر������ơ�
*���������
*			u8Light: u8LightΪ1��ʾ������Ϊ0��ʾϨ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_FP_SetLock (MBT_U8 u8Light)
{
	
}


/*
*˵�������ô�����ģʽ
*���������
*			u8Light: u8LightΪ1��ʾ����ģʽ��Ϊ0��ʾ�Ǵ���ģʽ��
*�������:
*		�ޡ�
*		���أ�
*		�ޡ�
*/
MMT_STB_ErrorCode_E MLMF_FP_SetLedMode(MBT_U8 u8Light)
{
	s_u8StandbyMode = (u8Light > 0) ? 1 : 0;

    return MM_NO_ERROR;
}

/*
*˵����������رմ����ơ�
*���������
*			u8Light: u8LightΪ1��ʾ������Ϊ0��ʾϨ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_FP_SetStandby(MBT_U8 u8Light)
{
	
}




/*
*˵������ȡǰ��尴�����ϲ�ڸ�200ms����һ������ɨ��ǰ��尴����
*���������
*�ޡ�
*�������:
*    �ޡ�
*		����
*		ǰ��尴�����͡�
*/
MMT_FP_KEY_E MLMF_FP_ReadKey(MBT_VOID)
{
	
}


/*
*˵������ǰ���LED����ʾһ���ַ���������ʾ��λ����ʾ��λ��ĿǰֻҪ����ʾӢ����ĸ�����֡�
*���������
*			pString: Ҫ��ʾ���ַ�����
*			u32StrLen: Ҫ����ʾ�ĳ��ȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/

MMT_STB_ErrorCode_E MLMF_FP_Display (MBT_CHAR *pString,MBT_U32 u32StrLen, MBT_BOOL bDotStatus)
{
    //MDrv_FD650_Display(pString,bDotStatus);
    return MM_NO_ERROR;
}

MBT_U8 MLMF_FP_GetFPMode(MBT_VOID)
{
    return mlmv_u8FpIsFD650;
}

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

