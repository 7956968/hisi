/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_surface.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : The user will use this api to realize some function
Function List   : 

                                          
History         :
Date                            Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/

#ifndef __HI_GO_SURFACE_H__
#define __HI_GO_SURFACE_H__


/*********************************add include here******************************/

#include "hi_go_comm.h"


/*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
   extern "C"
{
#endif
#endif /* __cplusplus */


     /***************************** Macro Definition ******************************/
	 /** \addtogroup          HIGO */
     /** @{ */      /** <!--[HIGO]*/

	/*max data of heft*//* CNcomment: ������ݷ����� */
	#define MAX_PARTPIXELDATA               3

    /** @} */  /** <!-- ==== Macro Definition End ==== */

    /*************************** Enum Definition **********************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

	
	typedef enum
	{
		HIGO_PF_CLUT8 = 0,	/*pallette*//*<CNcomment: ��ɫ�� */
		HIGO_PF_CLUT1,
		HIGO_PF_CLUT4,
		HIGO_PF_4444,		/**<Each pixel occupies 16 bits, and the A/R/G/B components each occupies 4 bits. They are sorted by address in descending order.*//*<CNcomment: һ������ռ16bit��ARGBÿ����ռ4bit������ַ�ɸ��������� */
		HIGO_PF_0444,		/**<Each pixel occupies 16 bits, and the A/R/G/B components each occupies 4 bits. They are sorted by address in descending order. The A component does not take effect.*//*<CNcomment: һ������ռ16bit��ARGBÿ����ռ4bit������ַ�ɸ���������, A������������ */
	
		HIGO_PF_1555,		/**<Each pixel occupies 16 bits, the R/G/B components each occupies 5 bits, and the A component occupies 1 bit. They are sorted by address in descending order.*//*<CNcomment: һ������ռ16bit��RGBÿ����ռ5bit��A����1bit������ַ�ɸ��������� */
		HIGO_PF_0555,		/**<Each pixel occupies 16 bits, the R/G/B components each occupies 5 bits, and the A component occupies 1 bit. They are sorted by address in descending order. The A component does not take effect.*//*<CNcomment: һ������ռ16bit��RGBÿ����ռ5bit��A����1bit������ַ�ɸ���������, A������������ */
		HIGO_PF_565,		/**<Each pixel occupies 16 bits, and the R/G/B components each occupies 5 bits, 6 bits, and 5 bits respectively. They are sorted by address in descending order.*//*<CNcomment: һ������ռ16bit��RGBÿ�����ֱ�ռ5bit��6bit��5bit������ַ�ɸ��������� */
		HIGO_PF_8565,		/**<Each pixel occupies 24 bits, and the A/R/G/B components each occupies 8 bits, 5 bits, 6 bits, and 5 bits respectively. They are sorted by address in descending order.*//*<CNcomment: һ������ռ24bit��ARGBÿ�����ֱ�ռ8bit, 5bit��6bit��5bit������ַ�ɸ��������� */
		HIGO_PF_8888,		/**<Each pixel occupies 32 bits, and the A/R/G/B components each occupies 8 bits. They are sorted by address in descending order.*//*<CNcomment: һ������ռ32bit��ARGBÿ����ռ8bit������ַ�ɸ��������� */
		HIGO_PF_0888,		/**<Each pixel occupies 24 bits, and the A/R/G/B components each occupies 8 bits. They are sorted by address in descending order. The A component does not take effect.*//*<CNcomment: һ������ռ24bit��ARGBÿ����ռ8bit������ַ�ɸ��������У�A������������ */
	
		HIGO_PF_YUV400, 	/**<Semi-planar YUV 400 format defined by HiSilicon*//*<CNcomment: ��˼�����semi-planar YUV 400��ʽ */    
		HIGO_PF_YUV420, 	/**<Semi-planar YUV 420 format defined by HiSilicon*//*<CNcomment: ��˼�����semi-planar YUV 420��ʽ */
		HIGO_PF_YUV422, 	/**<Semi-planar YUV 422 format and horizontal sampling format defined by HiSilicon*//*<CNcomment: ��˼�����semi-planar YUV 422��ʽ  ˮƽ������ʽ*/
		HIGO_PF_YUV422_V,	/**<Semi-planar YUV 422 format and vertical sampling format defined by HiSilicon*//*<CNcomment: ��˼�����semi-planar YUV 422��ʽ  ��ֱ������ʽ*/	 
		HIGO_PF_YUV444, 	/**<Semi-planar YUV 444 format defined by HiSilicon*//*<CNcomment: ��˼�����semi-planar YUV 444��ʽ */
	
		HIGO_PF_A1, 
		HIGO_PF_A8,
		HIGO_PF_BUTT
	} HIGO_PF_E;
	
	
	/*type of heft*//*<CNcomment: �������� */
	typedef enum
	{
		HIGO_PDFORMAT_RGB = 0,
		HIGO_PDFORMAT_RGB_PM,	/**< pre-multiplied */
		HIGO_PDFORMAT_Y,
		HIGO_PDFORMAT_UV,
		HIGO_PDFORMAT_UV_LE,	
	
		HIGO_PDFORMAT_BUTT
	} HIGO_PDFORMAT_E;
	
	typedef struct
	{
		HIGO_PDFORMAT_E Format; 	/*type of heft*//*<CNcomment: �������� */
		HI_VOID*		pData;		/*virtual address of heft*//*<CNcomment: ���������ַָ�� */
		HI_VOID*		pPhyData;	/*physics address of heft*//*<CNcomment: ���������ַָ�� */	
		HI_U32			Pitch;		/*line width of heft*//*<CNcomment: �����п� */
		HI_U32			Bpp;		/**< bytes per pixel */
		HI_U32			Offset;
	} HIGO_PIXELDATA_S;
	
	typedef enum 
	{
		HIGO_SYNC_MODE_CPU = 0x01, /*synchronization next step need operation cpu*//* CNcomment:ͬ������һ����Ҫ����CPU����*/
		HIGO_SYNC_MODE_TDE = 0x02, /*synchronization next step need 2d accelerate*//* CNcomment:ͬ������һ����Ҫ����2D���ٲ���*/
		HIGO_SYNC_MODE_BUTT,
	} HIGO_SYNC_MODE_E;
	

	/** @} */  /** <!-- ==== Enum Definition End ==== */

    /*************************** Structure Definition ****************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

	typedef struct
	{
	    HI_S32 Width;                  /*width of surface*//*<CNcomment: surface��� */
	    HI_S32 Height;                 /*height of surface*//*<CNcomment: surface�߶� */
	    HIGO_PF_E PixelFormat;         /*piexl format of surface*//*<CNcomment: surface���ظ�ʽ*/
	    HI_U32   Pitch[MAX_PARTPIXELDATA];              /*line distance  of surface*//*<CNcomment: surface�м����*/
	    HI_CHAR* pPhyAddr[MAX_PARTPIXELDATA];           /*physics address of surface*//*<CNcomment: surface�����ַ*/
	    HI_CHAR* pVirAddr[MAX_PARTPIXELDATA];          /*virtual address of surface*//*<CNcomment: surface�����ַ*/
	}HIGO_SURINFO_S;

	typedef HIGO_PIXELDATA_S HI_PIXELDATA[MAX_PARTPIXELDATA];

    /** @} */  /** <!-- ==== Structure Definition End ==== */

    /********************** Global Variable declaration **************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

    /** @} */  /** <!-- ==== Global Var Definition End ==== */

    /******************************* API declaration *****************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

	
	/**
	\brief lock surface. CNcomment:������surface
	\attention \n 
	After accessing a surface, you need to call the API to unlock it.
	CNcomment:��surface���ݷ��ʽ�����Ҫ��ʱ���øýӿڽ�����surface
	\param[in] Surface: Surface handle. CNcomment:Surface���
	
	\retval ::HI_SUCCESS
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_NOTLOCKED
	
	\see \n
	::HI_GO_LockSurface
	*/
	HI_S32 HI_GO_UnlockSurface(HI_HANDLE Surface);
	
	/** 
	\brief get size of surface. CNcomment:��ȡsurface�ߴ�
	\attention \n
	pWidth and pHeight cannot be empty concurrently.
	CNcomment:pWidth��pHeight����ͬʱΪ��
	\param[in]	Surface: Surface Handle. CNcomment:Surface���
	\param[out]  pWidth: point to width of Surface. CNcomment:Surface��������ַ
	\param[out]  pHeight: point to eight of Surface. CNcomment:Surface�߶������ַ
	
	\retval ::HI_SUCCESS 
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_NULLPTR 
	
	\see \n
	N/A
	*/
	HI_S32 HI_GO_GetSurfaceSize(HI_HANDLE Surface, HI_S32* pWidth, HI_S32* pHeight);
	
	/** 
	\brief get piexl format of surface. CNcomment:��ȡsurface���ظ�ʽ
	\attention \n
	none
	\param[in]	Surface: Surface Handle. CNcomment:Surface���
	\param[out] pPixelFormat: export address for pixel format. CNcomment:���ظ�ʽ�����ַ������Ϊ��
	
	\retval ::HI_SUCCESS 
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_NULLPTR
	
	\see \n
	N/A
	*/
	HI_S32 HI_GO_GetSurfacePixelFormat(HI_HANDLE Surface, HIGO_PF_E* pPixelFormat);
	
	/** 
	\brief memery turn	to surface. CNcomment:���û����ڴ��װ��surface
	\attention \n
	pSurInfo all attribute must right, and the format must is RGB16 ��32 bit for each piexl and YUV semi-planner
	CNcomment:pSurInfo�������Զ�����������ȷ,֧����������ظ�ʽΪRGB16��32λ���ظ�ʽ���Լ�YUV semi-planner��ʽ��
	\param[in]	pSurInfo :point to user memery info. CNcomment:�û��ڴ���Ϣ
	\param[out] handle infor. CNcomment:�����Ϣ
	
	\retval ::HI_SUCCESS 
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_NULLPTR
	\retval ::HIGO_ERR_INVSURFACESIZE
	\retval ::HIGO_ERR_INVSURFACEPF
	\retval ::HIGO_ERR_INVPARAM
	\retval ::HIGO_ERR_NOTINIT
	\retval ::HIGO_ERR_NOMEM
	
	\see \n
	��
	*/
	HI_S32 HI_GO_CreateSurfaceFromMem(const HIGO_SURINFO_S *pSurInfo, HI_HANDLE * pSurface);

	/** 
	\brief create memery for surface. CNcomment:�����ڴ�surface
	\attention \n
	cann't create surface for ::HIGO_PF_CLUT1��::HIGO_PF_CLUT4��::HIGO_PF_YUV420��::HIGO_PF_YUV422
	CNcomment:���ܴ���::HIGO_PF_CLUT1��::HIGO_PF_CLUT4��::HIGO_PF_YUV420��::HIGO_PF_YUV422��ʽ��surface
	\param[in] Width :Surface width. CNcomment:Surface���
	\param[in] Height :Surface height. CNcomment:Surface�߶�
	\param[in] PixelFormat: piexl format of Surface. CNcomment:Surface���ظ�ʽ
	\param[out] pSurface :Surface handle. CNcomment:Surface���
	
	\retval ::HI_SUCCESS 
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_NOTINIT
	\retval ::HIGO_ERR_NULLPTR
	\retval ::HIGO_ERR_INVSURFACESIZE
	\retval ::HIGO_ERR_INVSURFACEPF
	\retval ::HIGO_ERR_NOMEM
	
	\see \n
	::HI_GO_FreeSurface
	*/
	HI_S32 HI_GO_CreateSurface(HI_S32 Width, HI_S32 Height, HIGO_PF_E PixelFormat, HI_HANDLE* pSurface);
	
	/** 
	\param[in] Surface : surface handle. CNcomment:Surface���
	\param[out] pData HI_PIXELDATA struct. CNcomment:HI_PIXELDATA�ṹ��
	
	\retval ::HI_SUCCESS
	\retval ::HI_FAILURE
	
	\see \n
	::HI_GO_CreateImgDecoder
	*/
	HI_S32 HI_GO_LockSurface(HI_HANDLE Surface, HI_PIXELDATA pData);
	
	/** 
	\brief destory surface. CNcomment:����surface
	\attention \n
	N/A
	\param[in] Surface:surface handle. CNcomment:Surface���
	
	\retval ::HI_SUCCESS
	\retval ::HIGO_ERR_NOTINIT
	\retval ::HIGO_ERR_INVHANDLE
	\retval ::HIGO_ERR_INUSE
	
	\see \n
	::HI_GO_CreateSurface \n
	::HI_GO_DecImgData
	*/
	HI_S32 HI_GO_FreeSurface(HI_HANDLE Surface);
	

	/** @} */  /** <!-- ==== API Declaration End ==== */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HI_GO_SURFACE_H__*/
