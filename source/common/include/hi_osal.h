/******************************************************************************
Copyright (C), 2013-2023, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_osal.h
Version       : V1.0 Initial Draft
Author        : l00185424
Created       : 2013/7/25
Last Modified :
Description   : OS Abstract Layer.
Function List : None.
History       :
******************************************************************************/
#ifndef __HI_OSAL_H__
#define __HI_OSAL_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <stdarg.h>
#else
#include <linux/kernel.h>
#endif
#include "hi_type.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/******************************* API Declaration *****************************/
/** \addtogroup      COMMON*/
/** @{*/  /** <!-- -COMMON=*/

typedef va_list HI_VA_LIST_S;

/**-----Standard Functions--------*/

/**
\brief String copy. CNcomment: �ַ�������CNend
\attention \n
This function is the abstact of strncpy.CNcomment:����ӿڽ����Ƕ�strncpy�ļ򵥷�װCNend
\param[in] pszDest  the point of buffer that the string copy to.CNcomment:�����ַ���Ŀ�ĵ�ַCNend
\param[in] pszSrc the point of buffer that the string copy from.CNcomment:�����ַ���Դ��ַCNend
\param[in] ulLen  the max length of the string to copy.CNcomment:�����ַ�����󳤶� CNend
\retval ::the point of buffer that the string copy to(pszDest) if  Success CNcomment: �ɹ�����Ŀ���ַ�����ַ CNend
\retval ::NULL if FAILURE. CNcomment: APIϵͳ����ʧ�ܷ���NULL CNend
\see \n
N/A CNcomment: �� CNend
*/
HI_CHAR* HI_OSAL_Strncpy(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);

/**
\brief String compare. CNcomment: �ַ����Ƚ�CNend
\attention \n
This function is the abstact of strncpy.CNcomment:���ӿڽ����Ƕ�strncmp�ļ򵥷�װCNend
\param[in] pszStr1  the point of buffer that the first string.CNcomment:�ַ���Ŀ1�ĵ�ַCNend
\param[in] pszStr2 the point of buffer that the second string.CNcomment:�ַ���Ŀ2�ĵ�ַCNend
\param[in] ulLen  the max length of the string to copy.CNcomment:�Ƚ��ַ�����󳤶� CNend
\retval ::equal return 0,unequal return 1 or -1 CNcomment:��ͬ����0������ͬ���ط�0 CNend
\see \n
N/A CNcomment: �� CNend
*/
HI_S32 HI_OSAL_Strncmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);

/**
\brief String compare without regard to up/lower case. CNcomment: �޴�Сд���ַ����Ƚ�CNend
\attention \n
This function is the abstact of strncpy.CNcomment:���ӿڽ����Ƕ�strncasecmp�ļ򵥷�װCNend
\param[in] pszStr1 the point of buffer that the first string.CNcomment:�ַ���Ŀ1�ĵ�ַCNend
\param[in] pszStr2 the point of buffer that the second string.CNcomment:�ַ���Ŀ2�ĵ�ַCNend
\param[in] ulLen  the max length of the string.CNcomment:�Ƚ��ַ�����󳤶� CNend
\retval ::equal return 0,unequal return 1 or -1 CNcomment:��ͬ����0������ͬ���ط�0 CNend
\see \n
N/A CNcomment: �� CNend
*/
HI_S32 HI_OSAL_Strncasecmp(const HI_CHAR *pszStr1, const HI_CHAR *pszStr2, HI_SIZE_T ulLen);

/**
\brief appends the src string to the dest string. CNcomment: �ַ���ƴ��CNend
\attention \n
This function is the abstact of strncpy.CNcomment:���ӿڽ����Ƕ�strncat�ļ򵥷�װCNend
\param[in] pszDest  the point of buffer that the destinative string.CNcomment:Ŀ���ַ����ĵ�ַCNend
\param[in] pszSrc the point of buffer that the source string.CNcomment:Դ�ַ����ĵ�ַCNend
\param[in] ulLen  the max length of the string to copy.CNcomment:Դ�ַ�����󳤶� CNend
\retval ::the point of buffer that the destinative string.CNcomment:����Ŀ���ַ�����ַCNend
\see \n
N/A CNcomment: �� CNend
*/
HI_CHAR* HI_OSAL_Strncat(HI_CHAR *pszDest, const HI_CHAR *pszSrc, HI_SIZE_T ulLen);

/**
\brief write most uLen byte to the character string str . CNcomment:��ʽ��������ַ���CNend
\attention \n
This function is the abstact of strncpy.CNcomment:���ӿڽ����Ƕ�snprintf�ļ򵥷�װCNend
\param[in] pszStr  the point of buffer that the destinative string.CNcomment:Ŀ���ַ����ĵ�ַCNend
\param[in] ulLen the max length of the string.CNcomment:��д���ַ�������󳤶�CNend
\param[in] pszFormat  the format arguments.CNcomment:��ʽ���ı���б�CNend
\retval ::sucess return the length of the string,failure return -1 CNcomment:�ɹ������ַ������ȣ�ʧ�ܷ��ظ���CNend
\see \n
N/A CNcomment: �� CNend
*/
HI_S32 HI_OSAL_Snprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, ...);

/**
\brief write most uLen byte to the character string str . CNcomment:��ʽ��������ַ���CNend
\attention \n
This function is the abstact of strncpy.CNcomment:���ӿڽ����Ƕ�vsnprintf�ļ򵥷�װCNend
\param[in] pszStr  the point of buffer that the destinative string.CNcomment:Ŀ���ַ����ĵ�ַCNend
\param[in] ulLen the max length of the string.CNcomment:��д���ַ�������󳤶�CNend
\param[in] pszFormat  the string arguments.CNcomment:�ַ���CNend
\param[in] stVAList  the list of format arguments.CNcomment:��ʽ�������б�CNend
\retval ::sucess return the length of the string,failure return -1 CNcomment:�ɹ������ַ������ȣ�ʧ�ܷ��ظ���CNend
\see \n
N/A CNcomment: �� CNend
*/
HI_S32 HI_OSAL_Vsnprintf(HI_CHAR *pszStr, HI_SIZE_T ulLen, const HI_CHAR *pszFormat, HI_VA_LIST_S stVAList);


/** @}*/  /** <!-- ==== API Declaration End ====*/

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif /* __HI_OSAL_H__ */

