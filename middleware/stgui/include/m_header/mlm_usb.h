/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_usb.h
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
#ifndef	__MLM_USB_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_USB_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/
/*------------------------Module Constants---------------------------*/
#define mm_usb_nameLen 32
/*------------------------Module Types---------------------------*/
typedef enum _m_usb_devicestatus_e
{
    MM_DEVICE_INSERT,
    MM_DEVICE_REMOVE,
    MM_DEVICE_UNKNOW
}MMT_USB_DeviceStatus_E;

typedef enum _m_usb_devicedrvType_e
{
    MM_DEVICE_DRIVE_TYPE_REMOVEABLE,//�豸���Ƴ���
    MM_DEVICE_DRIVE_TYPE_FIXED,//�豸�����Ƴ�
    MM_DEVICE_DRIVE_TYPE_RAMDISK,//�ڴ���
    MM_DEVICE_DRIVE_TYPE_UNKNOW
}MMT_USB_DeviceDrvType_E;

typedef struct _m_usb_devicevinfo_t
{
    MMT_USB_DeviceDrvType_E stType;
    MBT_CHAR strMountName[mm_usb_nameLen];
    MBT_CHAR strDeviceName[mm_usb_nameLen];
	MBT_VOID *devId;
}MST_USB_DeviceVInfo_T;	

typedef struct _m_usb_Vfsinfo_t
{
    MBT_CHAR ms8FSType[8];
    MBT_U32 mu32Totalsize;
    MBT_U32 mu32freesize;
    MBT_U32 mu32Usedsize;	
}MST_USB_VfsInfo_T;

/*-----------------------Module Variables-------------------------*/

/*-----------------------Module Functions-------------------------*/
/*
*˵����ע���豸�ص�������
*���������
*USBDevCallback: ���������ע��Ļص�����ָ�룬����USB�豸�����γ�ʱ���ô˺���֪ͨ�ϲ㡣����typeΪ�豸��״̬��������߰γ���pu32IndexΪ�豸�����š�
*�������:
*			�ޡ�
*		���أ�
 *		        �ޡ�
*/
extern MBT_VOID MLMF_USB_RigesterCallback(MBT_VOID (* USBDevCallback)(MMT_USB_DeviceStatus_E type,MBT_CHAR*pstringName));

/*
*˵������ȡ�豸���߼��豸�����е��豸�Ữ�ֳ��߼��豸��ÿ���߼��豸����һ���豸�š�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		USB�豸������������ʱֻ���Ĵ洢�豸��
*/
extern MBT_U32 MLMF_USB_GetDevVolNum(MBT_VOID);


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
extern MMT_STB_ErrorCode_E MLMF_USB_ReadDevInfo(MST_USB_DeviceVInfo_T *pstUsbDevInfo,MBT_U32 u32DevNumber,MBT_U32 *pu32ReadNumber);


/*
*˵������ȡ�洢������Ϣ��
*���������
*			pstrMountName:Ŀ¼���֡�
*�������:
*			pstVfsInfo���豸��Ϣ��
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_USB_VfsInfo(MBT_CHAR *pstrMountName, MST_USB_VfsInfo_T *pstVfsInfo);

extern MMT_STB_ErrorCode_E MLMF_USB_Format(MBT_U8 *MountPath, MBT_U8 *DevicePath);


/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_USB_H__ */

/*----------------------End of mlm_usb.h-------------------------*/

