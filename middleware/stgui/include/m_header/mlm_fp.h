/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_pio.h
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
#ifndef	__MLM_PIO_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_PIO_H__


/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*------------------------Module Macors---------------------------*/
typedef enum _m_fp_key_e
{
    MM_FRONTKEY_NULL = 0x00,			
    MM_FRONTKEY_OK,			
    MM_FRONTKEY_MENU,
    MM_FRONTKEY_UP,			
    MM_FRONTKEY_DOWN,
    MM_FRONTKEY_LEFT,
    MM_FRONTKEY_RIGHT,
    MM_FRONTKEY_EXIT,
    MM_FRONTKEY_VOLUMEUP,
    MM_FRONTKEY_VOLUMEDOWN,
    MM_FRONTKEY_POWER
}MMT_FP_KEY_E;


/*------------------------Module Constants---------------------------*/

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
extern MMT_STB_ErrorCode_E MLMF_FP_Display (MBT_CHAR *pString,MBT_U32 u32StrLen,MBT_BOOL bDotStatus);

/*
*˵����ˢ��ǰ��壬�˺����ᱻ�ϲ�ڸ�6ms����һ�Σ��Ա�֤ǰ���LED��������ʾ��
*���ǰ������м���оƬ����CPUˢ�£����԰Ѵ˺����ſա�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MBT_VOID MLMF_FP_LedFresh (MBT_VOID);

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
extern MMT_STB_ErrorCode_E MLMF_FP_SetLock (MBT_U8 u8Light);
extern MMT_STB_ErrorCode_E MLMF_FP_SetLedMode(MBT_U8 u8Light);
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
extern MMT_STB_ErrorCode_E MLMF_FP_SetStandby(MBT_U8 u8Light);

/*
*˵������ȡǰ��尴�����ϲ�ڸ�200ms����һ������ɨ��ǰ��尴����
*���������
*�ޡ�
*�������:
*    �ޡ�
*		����
*		ǰ��尴�����͡�
*/
extern MMT_FP_KEY_E MLMF_FP_ReadKey(MBT_VOID);



/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_USB_H__ */

/*----------------------End of mlm_pio.h-------------------------*/

