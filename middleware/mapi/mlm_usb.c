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
#include "mlm_usb.h"



#if 0
#define MLUSB_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLUSB_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLUSB_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLUSB_DEBUG(fmt, args...) {}
#endif

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef void (*USBDevCallback)(MMT_USB_DeviceStatus_E type, MBT_CHAR *pstringName);



/*
*˵����ע���豸�ص�������
*���������
*USBDevCallback: ���������ע��Ļص�����ָ�룬����USB�豸�����γ�ʱ���ô˺���֪ͨ�ϲ㡣����typeΪ�豸��״̬��������߰γ���pstringNameΪ�豸���֡�
*�������:
*			�ޡ�
*		���أ�
*		         MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MBT_VOID MLMF_USB_RigesterCallback(MBT_VOID (* USBDevCallback)(MMT_USB_DeviceStatus_E type,MBT_CHAR*pstringName))
{
    //gpfusbCB = USBDevCallback;
}

/*
*˵������ȡ�豸���߼��豸�����е��豸�Ữ�ֳ��߼��豸��ÿ���߼��豸����һ���豸�š�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		USB�豸������������ʱֻ���Ĵ洢�豸��
*/
MBT_U32 MLMF_USB_GetDevVolNum(MBT_VOID)
{
	
}
/*
*˵������ȡUSB�豸��Ϣ��
*���������
*			pstUsbDevInfo: �豸��Ϣ��buffer��
*			u32DevNumber: �豸��Ϣbuffer�ĳ��ȡ�
*�������:
*			pstUsbDevInfo���豸��Ϣ��
*		���أ�
*		         MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_USB_ReadDevInfo(MST_USB_DeviceVInfo_T *pstUsbDevInfo,MBT_U32 u32DevNumber,MBT_U32 *pu32ReadNumber)
{
	
}

/*************************************************************
Function: USBAPI_UsbVfsInfo
Description: Get the information of the usb
Input: RTT_U8 *pu8Root
Output: USBAPIT_UsbVfsInfo *pstVfsInfo
return: ST_ErrorCode_t
*************************************************************/
MMT_STB_ErrorCode_E MLMF_USB_VfsInfo(MBT_CHAR *pstrMountName, MST_USB_VfsInfo_T *pstVfsInfo)
{
	
}

MMT_STB_ErrorCode_E MLMF_USB_Format(MBT_U8 *MountPath, MBT_U8 *DevicePath)
{
	
}

MMT_STB_ErrorCode_E MLF_UsbInit(MBT_VOID)
{
	
}

MMT_STB_ErrorCode_E MLF_UsbTerm(MBT_VOID)
{
	
}

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */


