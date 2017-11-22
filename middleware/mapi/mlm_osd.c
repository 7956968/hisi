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
#include "mlm_type.h"
#include "mlm_osd.h"
#include "mlm_demux.h"
#include "mlm_avctr.h"
#include "hi_unf_disp.h"
#include "hi_go.h"


/*----------------------Standard Include-------------------------*/
/*---------------------User-defined Include----------------------*/
/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0
#define MLOSD_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLOSD_DEBUG(fmt, args...) MLMF_Print("\033[32m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#else
#define MLOSD_ERR(fmt, args...)   MLMF_Print("\033[31m[%s:%d] "#fmt" \033[0m\r\n", __func__, __LINE__, ##args)
#define MLOSD_DEBUG(fmt, args...) {}
#endif

#define MLM_OSD_SUPPORT_EX 0

#define MEM_ALIGN( unit, val ) (1 + (((val) - 1) | (unit - 1)))


#define MAIN_BITMAP_DISPLAY_WIDTH 1920
#define MAIN_BITMAP_DISPLAY_HEIGHT 1080

#define m_OsdPhyWidth    1280
#define m_OsdPhyHeight   720 
#define m_OsdColorDepth   4 
#define HD_LAYER 0
#define SD_LAYER 1



static HI_HANDLE ghMainSurface;
static HI_HANDLE ghMainLayer    = HIGO_INVALID_HANDLE;

#if MLM_OSD_SUPPORT_EX
static HI_HANDLE ghExSurface;
static HI_HANDLE ghExLayer    = HIGO_INVALID_HANDLE;
#endif

static unsigned int ghOsd_u32Width = m_OsdPhyWidth;
static unsigned int ghOsd_u32Height = m_OsdPhyHeight;
static MMT_AV_HDResolution_E mlmv_OSD_stVideoMode = MM_AV_RESOLUTION_AUTO;


static MET_Mutex_T  gOsdProtector;


void MLF_OSD_Lock(void)
{
	MLMF_Mutex_Lock(gOsdProtector);
}

void MLF_OSD_UnLock(void)
{
	MLMF_Mutex_UnLock(gOsdProtector);
}

MBT_VOID MLF_OSD_ResizeOSD(MBT_VOID)
{
	
}


MBT_VOID MLF_OSD_GetOSDSize(MBT_U32 *pu32Width,MBT_U32 *pu32Height)
{
	if(MM_NULL != pu32Width)
	{
		*pu32Width = ghOsd_u32Width;
	}
	if(MM_NULL != pu32Height)
	{
		*pu32Height = ghOsd_u32Height;
	}
}



/********************** Local  Function ******************************/



/********************** End  Local  Function******************************/





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
MMT_STB_ErrorCode_E MLMF_OSD_SetOSDColorMode(MMT_OSD_ColorMode_E stColorMode)
{
    // not use
    return MM_NO_ERROR;
}

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
MMT_STB_ErrorCode_E MLMF_OSD_SetOSDAlpha(MBT_U8 value)
{
	HI_GO_SetLayerAlpha(ghMainLayer, value);
#if MLM_OSD_SUPPORT_EX
	HI_GO_SetLayerAlpha(ghExLayer, value);
#endif

	return MM_NO_ERROR;
}


/*
*˵�����ϲ�ͨ���˺�������ȡһ������д��˵�ͼ�����ݵ�buffer��
*�˵��ĳ�����buffer����ֱ�����Դ��bufferҲ�����ǵײ㴴����һ��
*��ʾ�ľ���buffer����buffer�ĳ�����������ֵ�и������ĳ���һ����
*�˺������ܲ��ظ����á�
*���������
*			�ޡ�
*�������:
*pu32Width��OSD�Ŀ�
*pu32Height��OSD�ĸߡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MBT_U8 * MLMF_OSD_GetInfo( MBT_U32 *pu32Width,MBT_U32 *pu32Height)
{
    *pu32Width = m_OsdPhyWidth;
    *pu32Height = m_OsdPhyHeight;
    return (MBT_U8 *)0x11111111;
}

MMT_STB_ErrorCode_E MLMF_OSD_Mix (MBT_U8 *pu8Dst,MBT_U8 *pu8Src1,MBT_U8 *pu8Src2,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h)
{
	HI_RECT SrcRect, DstRect;
	HIGO_BLTOPT_S BlitOpt;
	HI_HANDLE _psurface;
	HI_PIXELDATA _DecSurfaceInfo;
	HIGO_PF_E PixelFormat;
	unsigned char *_pTmp = NULL;
	HI_S32 _rel = 0;

	memset(&BlitOpt, 0, sizeof(HIGO_BLTOPT_S));
	BlitOpt.EnablePixelAlpha = 1;
	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_SRCOVER;
	BlitOpt.EnableScale = 1;
	
	SrcRect.x = x;
	SrcRect.y = 0;
	SrcRect.w = w;
	SrcRect.h = h;

	DstRect.x = x;
	DstRect.y = y;
	DstRect.w = w;
	DstRect.h = h;

	return MM_NO_ERROR;
	PixelFormat = HIGO_PF_8888;

	MLF_OSD_Lock();
	_rel = HI_GO_CreateSurface(u32PicW, h, PixelFormat, &_psurface);

	if(HI_SUCCESS != _rel)
	{
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_rel = HI_GO_LockSurface(_psurface, _DecSurfaceInfo,HI_TRUE);

	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_pTmp = _DecSurfaceInfo[0].pData;
	memset(_pTmp,0x0,h*u32PicW*4);
	memcpy(_pTmp,(pu8Src1 + (u32PicW*4*y)),h*u32PicW*4);
	//memcpy(_pTmp,pu8Src1,m_OsdPhyWidth*m_OsdPhyHeight*4);
	HI_GO_UnlockSurface(_psurface);

 	_rel = HI_GO_Blit(_psurface, &SrcRect, ghMainSurface, &DstRect, &BlitOpt);

	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_rel = HI_GO_LockSurface(_psurface, _DecSurfaceInfo,HI_TRUE);

	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_pTmp = _DecSurfaceInfo[0].pData;
	memset(_pTmp,0x0,h*u32PicW*4);
	memcpy(_pTmp,(pu8Src2 + (u32PicW*4*y)),h*u32PicW*4);
	//memcpy(_pTmp,pu8Src2,m_OsdPhyWidth*m_OsdPhyHeight*4);
	HI_GO_UnlockSurface(_psurface);

 	_rel = HI_GO_Blit(_psurface, &SrcRect, ghMainSurface, &DstRect, &BlitOpt);
	
	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	HI_GO_FreeSurface(_psurface);
	MLF_OSD_UnLock();

	return MM_NO_ERROR;
}


MMT_STB_ErrorCode_E MLMF_OSD_Disp (MBT_U8 *pu8Src,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h)
{
	HI_PIXELDATA _DecSurfaceInfo;
	HI_RECT SrcRect, DstRect;
	HIGO_BLTOPT_S BlitOpt;
	HI_HANDLE _psurface;
	HIGO_PF_E PixelFormat;
	unsigned char *_pTmp = NULL;
	HI_S32 _rel = 0;

	memset(&BlitOpt, 0, sizeof(HIGO_BLTOPT_S));
	#if 1
	BlitOpt.EnablePixelAlpha = 0;
	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_NONE;
	BlitOpt.EnableScale = 0;
	#else
	BlitOpt.EnablePixelAlpha = 1;
	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_SRCOVER;
	BlitOpt.EnableScale = 1;

	#endif
	SrcRect.x = x;
	SrcRect.y = 0;
	SrcRect.w = w;
	SrcRect.h = h;

	DstRect.x = x;
	DstRect.y = y;
	DstRect.w = w;
	DstRect.h = h;

	PixelFormat = HIGO_PF_8888;

	MLF_OSD_Lock();
	
	_rel = HI_GO_CreateSurface(u32PicW, h, PixelFormat, &_psurface);

	if(HI_SUCCESS != _rel)
	{
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_rel = HI_GO_LockSurface(_psurface, _DecSurfaceInfo,HI_TRUE);
	
	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}
	_pTmp = _DecSurfaceInfo[0].pData;
	memset(_pTmp,0x0,h*u32PicW*4);
	memcpy(_pTmp,(pu8Src + (u32PicW*4*y)),h*u32PicW*4);
	//memcpy(_pTmp,pu8Src,m_OsdPhyWidth*m_OsdPhyHeight*4);
	HI_GO_UnlockSurface(_psurface);

 	_rel = HI_GO_Blit(_psurface, &SrcRect, ghMainSurface, &DstRect, &BlitOpt);
	
	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	HI_GO_FreeSurface(_psurface);
	MLF_OSD_UnLock();

	return MM_NO_ERROR;
}


MMT_STB_ErrorCode_E MLMF_OSD_DispExt(MBT_U8 *pu8Src,MBT_U32 u32PicW,MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h)
{

#if MLM_OSD_SUPPORT_EX
	HI_RECT SrcRect, DstRect;
	HIGO_BLTOPT_S BlitOpt;
	HI_HANDLE _psurface;
	HIGO_PF_E PixelFormat;
	unsigned char *_pTmp = NULL;
	HI_S32 _rel = 0;

	memset(&BlitOpt, 0, sizeof(HIGO_BLTOPT_S));
	BlitOpt.EnablePixelAlpha = 1;
	BlitOpt.PixelAlphaComp = HIGO_COMPOPT_SRCOVER;
	BlitOpt.EnableScale = 1;
	
	SrcRect.x = 0;
	SrcRect.y = 0;
	SrcRect.w = m_OsdPhyWidth;
	SrcRect.h = m_OsdPhyHeight;

	DstRect.x = 0;
	DstRect.y = 0;
	DstRect.w = ghOsd_u32Width;
	DstRect.h = ghOsd_u32Height;

	PixelFormat = HIGO_PF_8888;

	MLF_OSD_Lock();
	
	_rel = HI_GO_CreateSurface(m_OsdPhyWidth, m_OsdPhyHeight, PixelFormat, &_psurface));

	if(HI_SUCCESS != _rel)
	{
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	_rel = HI_GO_LockSurface(_psurface, _pTmp,HI_TRUE);

	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	memcpy(_pTmp,(pu8Src + (u32PicW*4*y)),h*u32PicW*4);
	HI_GO_UnlockSurface(_psurface);

 	_rel = HI_GO_Blit(_psurface, &SrcRect, ghExSurface, &DstRect, &BlitOpt);
	
	if(HI_SUCCESS != _rel)
	{
		HI_GO_FreeSurface(_psurface);
		MLF_OSD_UnLock();
		return MM_ERROR_UNKNOW;
	}

	HI_GO_FreeSurface(_psurface);
	MLF_OSD_UnLock();
#endif
	return MM_NO_ERROR;
}



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
MMT_STB_ErrorCode_E MLMF_OSD_Flush (MBT_U32 x,MBT_U32 y,MBT_U32 w,MBT_U32 h)
{
	HI_RECT _rect;
	
	MLF_OSD_Lock();

	if (0 != ghMainLayer)
	{
		_rect.x = 0;
		_rect.y = 0;
		_rect.w = ghOsd_u32Width;
		_rect.h = ghOsd_u32Height;
		HI_GO_RefreshLayer(ghMainLayer, &_rect);
	}
	MLF_OSD_UnLock();	
    	return MM_NO_ERROR;
}


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
MMT_STB_ErrorCode_E MLMF_OSD_DrawImageEx (MST_OSD_DrawImageExParam_T *pParam)
{
    //not use
    return MM_ERROR_FEATURE_NOT_SUPPORTED;
}

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
MMT_STB_ErrorCode_E MLMF_OSD_GetImageInfo (MBT_CHAR *FileName,MST_OSD_ImageInfo_T *pImageInfo)
{
    // not use
    return MM_NO_ERROR;
}

MMT_STB_ErrorCode_E MLF_OSDInit(MBT_VOID)
{
	HI_S32 s32Ret   = 0;
	HIGO_LAYER_INFO_S stLayerInfo = {0};
	HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;

	MLMF_Mutex_Create(&gOsdProtector);
	HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_HD0, HI_UNF_DISP_LAYER_OSD_0, HI_LAYER_ZORDER_MOVETOP);
	s32Ret = HI_GO_Init();
	
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}

	//������ͼ�β�
	s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo); 

	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}

	stLayerInfo.CanvasWidth = m_OsdPhyWidth;
	stLayerInfo.CanvasHeight = m_OsdPhyHeight;
	stLayerInfo.DisplayWidth = stLayerInfo.CanvasWidth;
	stLayerInfo.DisplayHeight = stLayerInfo.CanvasHeight;

	stLayerInfo.ScreenWidth = MAIN_BITMAP_DISPLAY_WIDTH;
	stLayerInfo.ScreenHeight = MAIN_BITMAP_DISPLAY_HEIGHT;

	stLayerInfo.AntiLevel = HIGO_LAYER_DEFLICKER_NONE;
	stLayerInfo.PixelFormat = HIGO_PF_8888;
	stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_DOUBBUFER;
	s32Ret = HI_GO_CreateLayer(&stLayerInfo, &ghMainLayer);
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}

	s32Ret = HI_GO_GetLayerSurface(ghMainLayer, &ghMainSurface);
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}

#if MLM_OSD_SUPPORT_EX
	    //����������Ļͼ��
	s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_AD_0, &stLayerInfo); 
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}

	stLayerInfo.CanvasWidth = m_OsdPhyWidth;
	stLayerInfo.CanvasHeight = m_OsdPhyHeight;
	stLayerInfo.DisplayWidth = stLayerInfo.CanvasWidth;
	stLayerInfo.DisplayHeight = stLayerInfo.CanvasHeight;

	stLayerInfo.ScreenWidth = MAIN_BITMAP_DISPLAY_WIDTH;
	stLayerInfo.ScreenHeight = MAIN_BITMAP_DISPLAY_HEIGHT;
	stLayerInfo.PixelFormat = HIGO_PF_8888;
	stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;      
	s32Ret = HI_GO_CreateLayer(&stLayerInfo, &ghExLayer);
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;
	}


	s32Ret = HI_GO_GetLayerSurface(ghExLayer, &ghExSurface);
	if(HI_SUCCESS != s32Ret)
	{
		return MM_ERROR_UNKNOW;   
	}
#endif

	return MM_NO_ERROR;
}


MMT_STB_ErrorCode_E MLF_OSDTerm(MBT_VOID)
{
	return MM_NO_ERROR;
}


MMT_AV_HDResolution_E MLF_OSD_GetCurVideoMode(MBT_VOID)
{
    return mlmv_OSD_stVideoMode;
}

MBT_VOID MLF_OSD_SetCurVideoMode(MMT_AV_HDResolution_E u8VideoMode)
{
    mlmv_OSD_stVideoMode = u8VideoMode;    
}

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */


