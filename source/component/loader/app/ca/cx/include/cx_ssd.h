/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************/

#ifndef __CX_SSD_H__
#define __CX_SSD_H__

#include "../../../../api/src/cx_ssd.h"

/**
\brief authenticate the data CNcomment:�����ݽ���У��
\param[in]  StartVirAddr start virtual address of data CNcomment:StartVirAddr ������ʼ�����ַ
\param[out] CodeAddr  real Code address CNcomment:length ��������������ĵ�ַ
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_Authenticate(HI_U8* pu8StartVirAddr,HI_U32* pu32CodeOffset);

/**
\brief authenticate the data CNcomment:���� flash ���������������ݵĽṹ�峤�ȣ����ر��������ݳ���
\param[in]  u32StructureLen length of flash data area structure CNcomment:u32StructureLen flash �������ݽṹ�峤��
\param[out] EncryptedImageLen  encrypted image length:EncryptedImage���������ݳ���
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_GetEncryptedImageLenByStructureLen(HI_U32 u32StructureLen, HI_U32* pu32EncryptedImageLen);

/**
\brief encrypt the data  CNcomment:����ԭʼ������
\param[in]  StartVirAddr start address of data CNcomment:StartVirAddr ������ʼ��ַ
\param[int] length  data  length CNcomment:length ���ݳ���
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_DecryptUpgradeImage(HI_U8* pu8StartVirAddr,HI_U32 u32Length, HI_U8 au8M2MEncryptedSWPK[16]);

/**
\brief authenticate the data CNcomment:�����ݽ���У��
\param[in]  StartVirAddr start virtual address of data CNcomment:StartVirAddr ������ʼ�����ַ
\param[in]  Addrlength  data  length CNcomment:Addrlength ���ݳ���
\decryption:Signature data use the structure of HI_CASignImageTail_S.
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_CommonVerify_Authenticate(HI_U8* pu8StartVirAddr,HI_U32 u32Addrlength);

/**
\brief authenticate the data CNcomment:�����ݽ���У��
\param[in]  StartVirAddr start virtual address of data CNcomment:StartVirAddr ������ʼ�����ַ
\param[in]  length  data  length CNcomment:length ���ݳ���
\decryption:Signature data use the structure of HI_CASignImageTail_S.
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_CommonVerify_AuthenticateEx(HI_U8* pu8StartVirAddr,HI_U32 u32Addrlength, HI_CASignImageTail_S *pstSignImageInfo);

/**
\brief authenticate the data, no matter the authenticate type is common or special CNcomment:�����ݽ���У�飬��������Common��Special
\param[in]  StartVirAddr start virtual address of data CNcomment:StartVirAddr ������ʼ�����ַ
\param[in]  Addrlength  data  length CNcomment:Addrlength ���ݳ���
\decryption:Signature data use the structure of HI_CASignImageTail_S.
\retval ::HI_SUCCESS  success CNcomment:�ɹ�
\retval ::HI_FAILURE failure CNcomment:ʧ��
\see \n
*/
HI_S32 CA_Verify_Authenticate(HI_U8* pu8StartVirAddr, HI_U32 u32Addrlength);

#endif

