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

#ifndef  __HI_IR_H__
#define  __HI_IR_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"

typedef enum hiUNF_IR_CODE_E
{
    HI_UNF_IR_CODE_NEC_SIMPLE = 0,  /**<NEC with simple repeat code*/   /**<CNcomment:NEC with simple repeat code����*/
    HI_UNF_IR_CODE_TC9012,          /**<TC9012 code*/                   /**<CNcomment:TC9012����*/
    HI_UNF_IR_CODE_NEC_FULL,        /**<NEC with full repeat code*/     /**<CNcomment:NEC with full repeat code����*/
    HI_UNF_IR_CODE_SONY_12BIT,      /**<SONY 12-bit code */             /**<CNcomment:SONY 12BIT����*/
    HI_UNF_IR_CODE_RAW,      	    /**<Raw code*/	                    /**<CNcomment:raw ����*/
    HI_UNF_IR_CODE_RSTEP,			/**<Rstep code */                  /**<CNcomment:RSETP����,��mv330֧��*/
    HI_UNF_IR_CODE_BUTT
}HI_UNF_IR_CODE_E;

/**state of key*/
/**CNcomment:����״̬*/
typedef enum  hiUNF_KEY_STATUS_E
{
    HI_UNF_KEY_STATUS_DOWN = 0 ,   /**<Pressed*/   /**<CNcomment:���°��� */
    HI_UNF_KEY_STATUS_HOLD ,       /**<Hold*/      /**<CNcomment:��ס���� */
    HI_UNF_KEY_STATUS_UP ,         /**<Released*/  /**<CNcomment:̧�𰴼� */

    HI_UNF_KEY_STATUS_BUTT
}HI_UNF_KEY_STATUS_E ;


HI_S32 HI_UNF_IR_Init(HI_VOID);
HI_S32 HI_UNF_IR_DeInit(HI_VOID);
HI_S32 HI_UNF_IR_Enable(HI_BOOL bEnable);
HI_S32 HI_UNF_IR_GetValue(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId, HI_U32 u32TimeoutMs);
//HI_S32 HI_UNF_IR_GetValueWithProtocol(HI_UNF_KEY_STATUS_E *penPressStatus, HI_U64 *pu64KeyId, 
//        HI_CHAR *pszProtocolName, HI_S32 s32NameSize, HI_U32 u32TimeoutMs);
HI_S32 HI_UNF_IR_SetCodeType(HI_UNF_IR_CODE_E enIRCode);
HI_S32 HI_UNF_IR_EnableKeyUp(HI_BOOL bEnable);
HI_S32 HI_UNF_IR_EnableRepKey(HI_BOOL bEnable);
HI_S32 HI_UNF_IR_SetFetchMode(HI_S32 s32Mode);

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif /* End of #ifndef __HI_UNF_IR_H__ */

