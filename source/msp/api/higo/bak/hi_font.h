/**
 \file
 \brief Describes the header file of the font module. CNcomment:HiFont ����ӿ�ͷ�ļ�CNend
 \author Shenzhen Hisilicon Co., Ltd.
 \date 2011-2018
 \version 1.0
 \date 2011-8-25
 */
 
#ifndef _HI_FONT_H
#define _HI_FONT_H

#include "hi_type.h"
#ifndef WIN32

#ifdef __cplusplus
extern "C" {
#endif


/*************************** Structure Definition ****************************/
/** \addtogroup      font */
/** @{ */  /** <!--[font]*/

/** Common err code of font module */
/** CNcomment:HiFont ���������� CNend*/
typedef enum
{
    HI_FONT_ERRNOTINIT = 0,   /**< The module is not initialized *//**<CNcomment:δ��ʼ��CNend*/
    HI_FONT_ERRINITFAILED,	  /**< The module fails to be initialized *//**<CNcomment:��ʼ��ʧ��CNend*/
    HI_FONT_ERRDEINITFAILED,  /**< The module fails to be deinitialized *//**<CNcomment:ȥ��ʼ��ʧ��CNend*/
    HI_FONT_ERRNULLPTR,		  /**< The input pointer is null *//**<CNcomment:��ָ�����CNend*/
    HI_FONT_ERRINVHANDLE,	  /**< The input handle is invalid *//**<CNcomment:��Ч�������CNend*/
    HI_FONT_ERRNOMEM,		  /**< The memory is insufficient *//**<CNcomment:�ڴ治��CNend*/
    HI_FONT_ERRINTERNAL,
    HI_FONT_ERRINVFILE,       /**< The file operation fails because the file is invalid *//**<CNcomment:��Ч�ļ�·��CNend*/
    HI_FONT_ERRINVPARAM,	  /**< The parameter is invalid *//**<CNcomment:��Ч����CNend*/		
    HI_FONT_ERRUNSUPPORTED,   /**< The operation is invalid *//**<CNcomment:��֧�ֲ���CNend*/
    HI_FONT_ERRMMAP,		  /**< Mem-mapping fails *//**<CNcomment:�ڴ�ӳ��ʧ��CNend*/
    HI_FONT_ERRGETCHARINDEX,  /**< Obtaining char index fails *//**<CNcomment:��ȡ�ַ�����ʧ��CNend*/
    HI_FONT_ERRUNKNOWCHARSET, /**< Unknown charset *//**<CNcomment:δ֪�ַ���CNend*/
    HI_FONT_ERRBUTT
} HI_FONT_ERRE;




typedef struct tagGlyphMetrics {
	HI_U8 boxWidth;	  /**< Bitmap width *//**<CNcomment:λͼ��ռ�Ŀ��CNend*/
	HI_U8 boxHeight;  /**< Bitmap height *//**<CNcomment:λͼ��ռ�ĸ߶�CNend*/
	HI_S8 posX;  	  /**< Horizon offset between character and bitmap *//**<CNcomment:��������ռ��λ�õ�λͼ�� X�����ϵ�ƫ��CNend*/
	HI_S8 posY;  	  /**< Vertical offset between character and bitmap *//**<CNcomment:��������ռ��λ�õ�λͼ�� Y �����ϵ�ƫ��CNend*/
	HI_U8 incX; 	  /**< Horizon offset after drawing bitmap *//**<CNcomment:�����λͼ֮�� X �����ϵ�ƫ��CNend*/
	HI_U8 incY; 	  /**< Vertical offset after drawing bitmap *//**<CNcomment:�����λͼ֮�� Y �����ϵ�ƫ��CNend*/
} GlyphMetrics;


/** @} */  /** <!-- ==== Structure Definition End ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      font */
/** @{ */  /** <!--[font] */

/** 
\brief Initialize font beforing calling other font API.  
CNcomment:�����ļ���ʼ��, ��ʹ���ֿ�ӿ�ǰ��ʼ���ֿ�.CNend
\attention \n
ASCII charset file and GB2312 charset file cannot be empty.
GB2312 charset doesn't contain ascii character.
CNcomment:ASCII �ֿ��ļ�·����GB2312�ֿ��ļ�·��������Ϊ�գ�
GB2312�ֿ��в�����ascii �ַ���CNend

\param[in] pAsciiFontFile    ASCII charset file path. CNcomment:ASCII�ֿ��ļ�·��   CNend
\param[in] pGb2312FontFile  GB2312 charset file path. CNcomment:GB2312�ֿ��ļ�·��CNend

\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVFILE
\retval ::HI_FONT_ERRINITFAILED
\see \n
N/A
*/
HI_S32 HI_BitmapFont_Init( HI_CHAR*  pAsciiFontFile,  HI_CHAR* pGb2312FontFile);



/** 
\brief Denitialize font to release resources after calling other font API. 
CNcomment:����ȥ��ʼ��,��ʹ�����ֿ�ӿں�ر��ļ�,�ͷ���Դ.CNend
\attention \n
\param[in] 
\retval ::HI_SUCCESS 
\see \n
N/A
*/
HI_S32 HI_BitmapFont_DeInit();


/*********************************************************************************************/

/** 
\brief Obtains character model info of queried character. 
CNcomment:��ȡ����ѯ�ַ�����ģ��Ϣ.CNend
\attention \n
str contains only one queried character. The first character will take effect if there are
more than one character.
CNcomment:str һ��ֻ����һ������ѯ�ַ�.����һ���ַ�ֻȡ��һ����CNend
\paam[in]  str Queried character. CNcomment: ����ѯ���ַ�CNend
\param[out]  pData Pointer to character's address in character lib. CNcomment:��ָ����ָ���ֵΪ���ַ����ֿ��е��׵�ַCNend
\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVPARAM
\retval ::HI_FONT_ERRUNKNOWCHARSET
\see \n
N/A
*/
HI_S32 HI_BitmapFont_GetGlyphData(HI_CHAR * str, HI_U8** pData);

/** 
\brief Obtains width and height info of queried character.  
CNcomment:��ȡ����ѯ�ַ�����ģ�Ŀ����ϢCNend
\attention \n
str contains only one queried character. The first character will take effect if there are
more than one character.
CNcomment:str һ��ֻ����һ������ѯ�ַ�.����һ���ַ�ֻȡ��һ����CNend
\paam[in]  str Queried character. CNcomment:����ѯ���ַ�CNend
\param[out]  pWidth widht. CNcomment:��ģ���CNend
\param[out]  pHeight height. CNcomment:��ģ�߶�CNend

\retval ::HI_SUCCESS 
\retval ::HI_FONT_ERRINVPARAM
\retval ::HI_FONT_ERRUNKNOWCHARSET
\see \n
N/A
*/
HI_S32 HI_BitmapFont_GetGlyphSize(HI_CHAR * str, HI_S32 *pWidth, HI_S32 *pHeight);


/*********************************************************************************************/

/** @} */  /** <!-- ==== API Declaration End ==== */


#ifdef __cplusplus
}
#endif

#endif //

#endif


