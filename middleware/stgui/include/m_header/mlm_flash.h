/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_flash.h
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
#ifndef	__MLM_FLASH_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_FLASH_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*
*˵������ָ����FLASH��ַ�������ݣ������롣
*���������
*			flash_addr: FLASH��ַ��
*			buf: ���ݻ���ָ�롣
*			len: ���ݳ��ȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Flash_Read(MBT_U32 flash_addr, MBT_VOID * buf, MBT_U32 len);

/*
*˵������ָ����FLASH��ַд�����ݣ������롣
*���������
*			flash_addr: FLASH��ַ��
*			buf: ���ݻ���ָ�롣
*			len: ���ݳ��ȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Flash_Write(MBT_U32 flash_addr, MBT_U8 * buf,MBT_U32 len);

/*
*˵��������һ�����������򣬲���һ��BLOCK�����������BLOCK��
*���������
*			flash_addr: FLASH��ַ
*			size: Ҫ������flash����
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Flash_EraseBlock(MBT_U32 flash_addr);

/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_FLASH_H__ */

/*----------------------End of mlm_flash.h-------------------------*/

