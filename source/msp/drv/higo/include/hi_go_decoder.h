/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_go_decoder.h
Version             : Initial Draft
Author              : 
Created             : 2014/06/20
Description         : The user will use this api to realize some function
Function List   : 

                                          
History         :
Date                            Author                  Modification
2014/06/20                  y00181162               Created file        
******************************************************************************/

#ifndef __HI_GO_DECODE_H__
#define __HI_GO_DECODE_H__


/*********************************add include here******************************/
#include "hi_go_comm.h"
#include "hi_go_surface.h"


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

    /** @} */  /** <!-- ==== Macro Definition End ==== */

    /*************************** Enum Definition **********************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/
	/*type of decoder *//**CNcomment:����ʵ���Ľ������� */
	typedef enum 
	{
	    HIGO_DEC_IMGTYPE_JPEG = HIGO_IMGTYPE_JPEG, /*for jpeg *//*<CNcomment:JPEG��ʽͼƬ*/
	    HIGO_DEC_IMGTYPE_GIF  = HIGO_IMGTYPE_GIF,  /*for gif *//*<CNcomment:GIF��ʽͼƬ*/
	    HIGO_DEC_IMGTYPE_BMP  = HIGO_IMGTYPE_BMP,  /*for bmp *//*<CNcomment:BMP��ʽͼƬ */
	    HIGO_DEC_IMGTYPE_PNG  = HIGO_IMGTYPE_PNG,  /*for png *//*<CNcomment:PNG��ʽͼƬ */
	    HIGO_DEC_IMGTPYE_BUTT
	} HIGO_DEC_IMGTYPE_E;
	typedef enum 
	{
	    HIGO_DEC_POSITION_SET = 0,   /*start position*//*CNcomment:��ʼλ��*/
	    HIGO_DEC_POSITION_CUR,       /*current position *//*CNcomment:�ӵ�ǰλ��*/
	    HIGO_DEC_POSITION_END,       /*end position *//*CNcomment:�ӽ���λ��*/
	    HIGO_DEC_POSITION_BUTT
	}HIGO_DEC_POSITION_E;

	/*format type of extend data*//*CNcomment: ��չ���ݸ�ʽ���� */
	typedef enum 
	{
		HIGO_DEC_EXTEND_EXIF = 0,		  /*extend data for exif format*//*<CNcomment:EXIF��ʽ����չ���� */
		HIGO_DEC_EXTEND_EXIF_BUTT
	} HIGO_DEC_EXTENDTYPE_E;
		
	/** @} */  /** <!-- ==== Enum Definition End ==== */

    /*************************** Structure Definition ****************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/
	
	/*stream source information *//*CNcomment:������Դ����ϸ��Ϣ*/
	typedef union 
	{
		/*about memery stream source *//*CNcomment:����ԴΪ�ڴ��ʱ��Ҫ����Ϣ*/
		struct
		{
			HI_CHAR* pAddr; 	/*pointer of memery address*//*<CNcomment:�ڴ�ָ���ַ*/
			HI_U32 Length;		 /*length *//*<CNcomment:����*/
		} MemInfo;
	} HIGO_DEC_SRCINFO_U;
	/*the parameter for create decoder*//*CNcomment: ��������ʵ����Ҫ�Ĳ��� */
	typedef struct 
	{
		HIGO_DEC_SRCINFO_U SrcInfo; /*information of input steam*//*<CNcomment:����Դ��ϸ��Ϣ*/
	} HIGO_DEC_ATTR_S;
	
	/*information after deocded*//*CNcomment: ���������Ϣ */
	typedef struct 
	{
		HI_U32			   Count;			/*number of pictrue*//*<CNcomment:ͼƬ�ĸ��� */
		HIGO_DEC_IMGTYPE_E ImgType; 		/*type of input steam*//*<CNcomment:����Դ���� */
		HI_U32			   ScrWidth;	   /*width of picture that is consider as picture screen, 0 is invalid*//*<CNcomment:��Ļ��ȣ��˴���Ļ��ʾͼƬ�ļ���Ļ��0��ʾ��Ч */
		HI_U32			   ScrHeight;		/*width of picture that is consider as picture screen, 0 is invalid*//*<CNcomment:��Ļ�߶ȣ��˴���Ļ��ʾͼƬ�ļ���Ļ��0��ʾ��Ч */
		HI_BOOL 		   IsHaveBGColor;	/*whether had background *//*<CNcomment:�Ƿ��б���ɫ */
		HI_COLOR		   BGColor; 		/*background color *//*<CNcomment:������ɫ */
	} HIGO_DEC_PRIMARYINFO_S;
	
	typedef struct 
	{
		HI_U32	  OffSetX;				  /*X offset of screen that is consider as picture screen*//*<CNcomment:����Ļ�ϵ�Xƫ�������˴���Ļ��ʾͼƬ�ļ���Ļ����ͬ */
		HI_U32	  OffSetY;				  /*Y offset of screen that is consider as picture screen*//*<CNcomment:����Ļ�ϵ�Yƫ����*/
		HI_U32	  Width;				  /*width of picture*//*<CNcomment:ԭʼͼƬ�ĸ߶� */
		HI_U32	  Height;				  /*height of picture*//*<CNcomment:ԭʼͼƬ�Ŀ�� */
		HI_U8	  Alpha;				  /*alpha that is use splice*//*<CNcomment:����ͼƬ���е��ӵ�ͨ��alpha��Ϣ����Χ��0-255 */
		HI_BOOL   IsHaveKey;			  /*whether has key*//*<CNcomment:�Ƿ���KEY�� HI_FALSE: KEY��ʹ�ܣ�HI_TRUE: KEYʹ�� */
		HI_COLOR  Key;					  /*colorkey information for splice*//*<CNcomment:����ͼƬ���е��ӵ�colorkey��Ϣ */
		HIGO_PF_E Format;				  /*format of source picture *//*<CNcomment:ͼƬԴ���ظ�ʽ */
		HI_U32	  DelayTime;			  /**<Time interval between this picture and the previous one (10 ms). The value 0 indicates that the parameter is invalid.*//**<CNcomment:����һ��ͼƬ��ʱ����(10ms)��0��ʾ�ò�����Ч */
		HI_U32	  DisposalMethod;		  /**<Processing method of the current frame. This application programming interface (API) is valid for only GIF pictures. 0: customized processing method; 1: no processing method; 2: use the background color.*//**<CNcomment:��ǰ֡���������˽ӿ�ֻ��GIF��Ч��0:�û���������ʽ��1:��������2:ʹ�ñ���ɫ��*/	
	} HIGO_DEC_IMGINFO_S;
	
	/**Picture attributes after decoding*/
	/**CNcomment:�����ͼƬ���� */
	typedef struct 
	{
		HI_U32	  Width;				  /*want width after deocded*//*<CNcomment:����������� */
		HI_U32	  Height;				  /*want height after deocded*//*<CNcomment:���������߶�*/
		HIGO_PF_E Format;				  /*want format after deocded*//*<CNcomment:������������ظ�ʽ*/
	} HIGO_DEC_IMGATTR_S;
	
	/*base information of picture*//*CNcomment:ͼƬ������Ϣ */
	typedef struct 
	{
		HI_U32	  Width;				/*width of picture*//*<CNcomment:ͼƬ�ĸ߶� */
		HI_U32	  Height;				/*height of picture*//*<CNcomment:ͼƬ�Ŀ�� */
		HIGO_PF_E PixelFormat;			/*format of pixel*//*<CNcomment:���ظ�ʽ */
		HI_U32	  VirAddr[3];			/*virtual address*//*<CNcomment:�����ַ�������RGB��ʽ��ֻ�е�һ��VirAddr[0]Ϊ��ʼ��ַ��
													VirAddr[1]��VirAddr[2]��Ч�������YC��ʽ��
													VirAddr[0]��ʾY������ʼ��ַ��VirAddr[1]��ʾC������ʼ��ַ��
													VirAddr[2]��Ч */
		HI_U32	  PhyAddr[3];			/*physics address*//*<CNcomment:�����ַ��ʹ��ԭ��ͬ�� */
		HI_U32	  Pitch[3]; 			/*line distance*//*<CNcomment:�м�࣬ʹ��ԭ��ͬ�� */
		HI_U32	  Palate[256];		  /*pallette*//*<CNcomment:��ɫ�� */
	} HIGO_DEC_IMGDATA_S;
    /** @} */  /** <!-- ==== Structure Definition End ==== */

    /********************** Global Variable declaration **************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

    /** @} */  /** <!-- ==== Global Var Definition End ==== */

    /******************************* API declaration *****************************/
	/** \addtogroup 		 HIGO */
	/** @{ */		/** <!--[HIGO]*/

	
	/** 
	 \brief create decoder handle. CNcomment:����������
	 \attention \n
	 Currently, the formats of .bmp, .png, .jpeg, and .gif are supported. The picture input source can be the memory, files, and streams. The picture input source of streams is only applicable to the .jpeg format.
	 CNcomment:Ŀǰ֧��BMP��PNG��JPEG��GIF��ʽ��ͼƬ����Դֻ����Ϊ�ڴ桢�ļ��Լ���ʽ����ʽĿǰֻ��JPEG��ʽ��Ч��
	 \param[in] pSrcDesc: input stream information. CNcomment:����Դ��Ϣ
	 \param[out] pDecoder:handle of deocde instance. CNcomment:����ʵ�����ָ��
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_NULLPTR
	 \retval ::HIGO_ERR_NOTINIT
	 \retval ::HIGO_ERR_NOMEM
	 \retval ::HIGO_ERR_INVHANDLE
	 \retval ::HIGO_ERR_INVFILE
	 \retval ::HIGO_ERR_INVSRCTYPE
	 \retval ::HIGO_ERR_INVIMAGETYPE
	 \retval ::HIGO_ERR_INVIMGDATA
	
	\see \n
	::HI_GO_DestroyDecoder
	*/
	HI_S32 HI_GO_CreateDecoder(const HIGO_DEC_ATTR_S* pSrcDesc, HI_HANDLE* pDecoder);
	
	 /** 
	 \brief destory decoder. CNcomment:���ٽ�����
	 \attention \n
	 N/A
	 \param[in] Decoder : handle of deocde instance. CNcomment:����ʵ�����
	 
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_INVHANDLE
	
	\see \n
	::HI_GO_CreateDecoder
	*/
	HI_S32 HI_GO_DestroyDecoder(HI_HANDLE Decoder);
	 
	/** 
	 \brief reset decoder when want reset decoder. CNcomment:��λ������������Ҫ��ֹ�����ʱ��ͽ��н�������λ
	 \attention \n
	 N/A
	 \param[in] Decoder: handle of deocde instance. CNcomment:����ʵ�����
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_INVHANDLE
	
	\see \n
	::HI_GO_CreateDecoder \n
	::HI_GO_DestroyDecoder
	*/
	HI_S32 HI_GO_ResetDecoder(HI_HANDLE Decoder);
	
	 /** 
	 \brief get main information of picture. CNcomment:��ȡͼƬ�ļ�����Ҫ��Ϣ 
	 \attention
	 \param[in] Decoder:handle of decoder instance. CNcomment:����ʵ�����
	 \param[out] pPrimaryInfo : pointer of picture information. CNcomment:����ͼƬ����Ϣָ��
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_INVHANDLE
	 \retval ::HIGO_ERR_NULLPTR
	
	\see \n
	::HI_GO_DecImgInfo
	*/
	HI_S32 HI_GO_DecCommInfo(HI_HANDLE Decoder, HIGO_DEC_PRIMARYINFO_S *pPrimaryInfo);
	
	/** 
	 \brief  get the information of picture that can set the format by user
	 CNcomment:��ȡָ��ͼƬ����Ϣ����ͬʱ�û���������������ͼ���ʽ
	 \attention \n
	 none
	 \param[in] Decoder: the instance of decoder. CNcomment:����ʵ�����
	 \param[in] Index: index of picture.
								CNcomment:ͼƬ�����š�JPEG��ʽͼƬ����ͼ����������0������ͼ��������1��ʼ��
								GIF��ʽͼƬ�������ļ��еĴ洢˳����������0��ʼ�ݼ�
	 \param[out] pImgInfo: information of picture. CNcomment:ͼƬ��Ϣ
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_NULLPTR
	 \retval ::HIGO_ERR_INVHANDLE
	
	\see \n
	::HI_GO_DecCommInfo
	*/
	HI_S32 HI_GO_DecImgInfo(HI_HANDLE Decoder, HI_U32 Index, HIGO_DEC_IMGINFO_S *pImgInfo);
	
	 /** 
	 \brief get single picture data. CNcomment:��ȡ����ͼƬ������
	 \attention \n
	 The format in the destination picture attributes does not support the macro block format.
	 CNcomment:Ŀ��ͼƬ�����е�ͼ���ʽ��֧�ֺ���ʽ
	 \param[in] Decoder : handle of decoder's instance.  CNcomment:����ʵ�����
	 \param[in] Index :picture index that is start from 0. CNcomment:ͼƬ�����ţ������Ŵ�0��ʼ
	 \param[in] pImgAttr: attribute of targeg picture. CNcomment:Ŀ��ͼƬ���ԣ�Ϊ�ձ�ʾ����ԭͼƬ����
	 \param[out] pSurface: surface handle. CNcomment:surface���
	 \retval ::HI_SUCCESS
	 \retval ::HIGO_ERR_NULLPTR
	 \retval ::HIGO_ERR_INVHANDLE
	
	\see \n
	::HI_GO_DecCommInfo
	*/
	HI_S32 HI_GO_DecImgData(HI_HANDLE Decoder, HI_U32 Index, const HIGO_DEC_IMGATTR_S *pImgAttr, HI_HANDLE *pSurface);
	
	HI_S32 HI_GO_DecImgToSurface(HI_HANDLE Decoder, HI_U32 Index, HI_HANDLE Surface);
	/** @} */  /** <!-- ==== API Declaration End ==== */

    /****************************************************************************/



#ifdef __cplusplus

#if __cplusplus

}
#endif
#endif /* __cplusplus */

#endif /* __HI_GO_DECODE_H__*/
