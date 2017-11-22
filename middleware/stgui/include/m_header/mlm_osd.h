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

#ifndef	__MLM_OSD_H__ /* ��ֹgraphics.h���ظ����� */
#define	__MLM_OSD_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Types---------------------------*/
typedef  enum _m_osd_color_mode_e
{
    MM_OSD_COLOR_MODE_ARGB8888 = 0,
    MM_OSD_COLOR_MODE_ARGB4444,
    MM_OSD_COLOR_MODE_ARGB1555,
    MM_OSD_COLOR_MODE_RGB565,
    MM_OSD_COLOR_MODE_ALUT88
}MMT_OSD_ColorMode_E;


typedef struct _m_osd_imageinfo_t
{ 
    MBT_S32 Width; /* ͼ��� */
    MBT_S32 Height; /* ͼ��� */
    MBT_U32 Size; /* ͼ�����ݴ�С */
} MST_OSD_ImageInfo_T;

/* ͼ����ʾ��Ч */
typedef enum _m_osd_drawStyle_e
{
    MM_OSD_STYLE_NONE, /* û����Ч */
    MM_OSD_STYLE_RANDOM, /* �����Ч */
    MM_OSD_STYLE_SEQUENTIAL, /* ����˳����ʾ������Ч */
    MM_OSD_STYLE_01 /* ������ */
}MMT_OSD_DrawStyle_E;

/*
* ͼ������ʶ
* bit 0 :�Ƿ񱣳ֱ���
* bit 1-2 :��ת�Ƕ�
* bit 3 :δ����
* bit 4-5 :��ֱ������뷽ʽ
* bit 6-7 :ˮƽ������뷽ʽ
*/
typedef enum _m_osd_image_decodeflag_e
{
    /*���ֱ���*/
    MM_OSD_IMAGE_DECODE_KEEP_ASPECT_RATIO = 0x00000001,
    /*˳ʱ�뷽����ת0��*/
    MM_OSD_IMAGE_DECODE_ROTATE_0 = 0x00000000,
    /*˳ʱ�뷽����ת90��*/
    MM_OSD_IMAGE_DECODE_ROTATE_90 = 0x00000002,
    /*˳ʱ�뷽����ת180��*/
    MM_OSD_IMAGE_DECODE_ROTATE_180 = 0x00000004,
    /*˳ʱ�뷽����ת270��*/
    MM_OSD_IMAGE_DECODE_ROTATE_270 = 0x00000006,
    /*��ֱ��������*/
    MM_OSD_IMAGE_DECODE_VSTRETCH = 0x00000000,
    /*��ֱ�����϶���(����ԭʼ��С)*/
    MM_OSD_IMAGE_DECODE_UP = 0x00000010,
    /*��ֱ�����¶���(����ԭʼ��С) */
    MM_OSD_IMAGE_DECODE_DOWN = 0x00000020,
    /*��ֱ�������Ķ���(����ԭʼ��С)*/
    MM_OSD_IMAGE_DECODE_VCENTER = 0x00000030,
    /*ˮƽ��������*/
    MM_OSD_IMAGE_DECODE_HSTRETCH = 0x00000000,
    /*ˮƽ������(����ԭʼ��С)*/
    MM_OSD_IMAGE_DECODE_LEFT = 0x00000040,
    /*ˮƽ�����Ҷ��� (����ԭʼ��С)*/
    MM_OSD_IMAGE_DECODE_RIGHT = 0x00000080,
    /*ˮƽ���Ķ���(����ԭʼ��С)*/
    MM_OSD_IMAGE_DECODE_HCENTER = 0x000000c0
} MMT_OSD_ImageDecodeFlag_E;

/* ͼ����ʾ����*/
typedef struct _m_osd_imageEx_param_t
{
    MBT_CHAR *FileName; /*Դͼ���ļ��� */
    MMT_OSD_DrawStyle_E Style; /*��Ч,Ŀǰ��Ҫ��֧���κ���Ч*/
    MMT_OSD_ImageDecodeFlag_E ImageDecodeFlag; /*ͼ������ʶ */
    MBT_S32 DstX; /*Ŀ����ʾ����x ������ */
    MBT_S32 DstY; /*Ŀ����ʾ����y ������ */
    MBT_S32 DstWidth; /*Ŀ����ʾ����� */
    MBT_S32 DstHeight; /*Ŀ����ʾ����� */
    MBT_S32 SrcX; /*Դͼ�����ʾ����x ������ */
    MBT_S32 SrcY; /*Դͼ�����ʾ����y ������ */
    MBT_S32 SrcWidth; /*Դͼ�����ʾ����� */
    MBT_S32 SrcHeight; /*Դͼ�����ʾ����� */
}MST_OSD_DrawImageExParam_T;


/*-----------------------Module Functions-------------------------*/
/*------------------------Module Macors---------------------------*/
/*
*˵��������ͼ�β����ɫ���͡��ڵ����κ�ͼģ�麯��ǰ�����ȵ��ô˺���������ϵͳ����ɫ���ͣ�
*һ���趨��ɣ�֮���������ɫ�����˺���������ֵ���в�����
*���������
*			stColorMode����ɫ����ֵ��
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_OSD_SetOSDColorMode(MMT_OSD_ColorMode_E stColorMode);
/*
*MMT_STB_ErrorCode_E MLMF_OSD_SetOSDAlpha(MBT_U8 value)
*		˵��������ͼ�β��alphaֵ��0~255��ֻ��OSD��ʹ�á�
*���������
*			Value��alphaֵ��0~255��
*�������:
*�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_OSD_SetOSDAlpha(MBT_U8 value);

/*
*˵�����ϲ�ͨ���˺�������ȡһ������д��˵�ͼ�����ݵ�buffer��
*�˵��ĳ�����buffer����ֱ�����Դ��bufferҲ�����ǵײ㴴����һ��
*��ʾ�ľ���buffer����buffer�ĳ�����������ֵ�и������ĳ���һ����
*���������
*			�ޡ�
*�������:
*pu32Width��OSD�Ŀ�
*pu32Height��OSD�ĸߡ�
*		���أ�
*			�������ַ��
*/
extern MBT_U8 * MLMF_OSD_GetInfo( MBT_U32 *pu32Width,MBT_U32 *pu32Height);


/*
*˵�������ϲ�׼����һ���������Ժ󣬱����ô˺���ˢ��ȫ����
*���������
*�ޡ�
*���������
*�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_OSD_Flush (MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h);
extern MMT_STB_ErrorCode_E MLMF_OSD_Mix (MBT_U8 *pu8Dst,MBT_U8 *pu8Src1,MBT_U8 *pu8Src2,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h);
extern MMT_STB_ErrorCode_E MLMF_OSD_Disp (MBT_U8 *pu8Src,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h);
extern MMT_STB_ErrorCode_E MLMF_OSD_DispExt (MBT_U8 *pu8Src,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h);


/*
*˵�������벢��ʾͼƬ(jpg\bmp\png\gif\tif)���˺�����Ҫ���ڵײ������ͼƬ���빦�ܵ�ƽ̨�����ƽ̨��������ع��ܣ����԰Ѵ˺����ſա�
*���������
*			pParam ָ��ͼ����ʾ������ָ�롣
*���������
*                       �ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_OSD_DrawImageEx (MST_OSD_DrawImageExParam_T *pParam);
/*
*˵������ȡͼ����Ϣ���˺�����Ҫ���ڵײ������ͼƬ���빦�ܵ�ƽ̨�����ƽ̨��������ع��ܣ����԰Ѵ˺����ſա�
*���������
*			FileName��Դͼ���ļ���(��������·��)��
*�������:
*pImageInfo��ָ��ͼ����Ϣ��ָ�롣
*		���أ�
*        		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/

extern MMT_STB_ErrorCode_E MLMF_OSD_GetImageInfo (MBT_CHAR *FileName,MST_OSD_ImageInfo_T *pImageInfo);


/*��������ͼƬ*/
extern MMT_STB_ErrorCode_E MLMF_OSD_Picture_Start(MBT_CHAR *pu8FileName,MMT_OSD_ColorMode_E stOutputColorMode);

extern MBT_U8 *MLMF_OSD_GetPictureInfo(MBT_U32 *pu32Width,MBT_U32 *pu32Height);

extern MMT_STB_ErrorCode_E MLMF_OSD_ShowPicture(MBT_U32 x,MBT_U32 y,MBT_U32 u32Width,MBT_U32 u32Height);

/*ֹͣ����ͼƬ*/
extern MMT_STB_ErrorCode_E MLMF_OSD_Picture_Stop(MBT_VOID);


/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__ERRORCODE_H__ */

/*----------------------End of ErrorCode.h-------------------------*/

