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

#include <stdio.h>
#include <stdlib.h>
#include "hi_type.h"
#include "hi_unf_advca.h"
#include "sample_ca_common.h"

static unsigned char rsa_key[] = {
    0xa3 , 0x1f , 0x12 , 0xf8 , 0xa6 , 0xbf , 0xb5 , 0x19 , 0xad , 0xa3 , 0xef , 0x18 , 0x68 , 0x37 , 0x2b , 0xd1,
    0xa5 , 0x7d , 0x1c , 0x2f , 0x83 , 0x88 , 0x56 , 0xe5 , 0xfb , 0xe6 , 0xa0 , 0xac , 0xa4 , 0xd7 , 0xb9 , 0xd8,
    0xb7 , 0xca , 0x37 , 0x5a , 0xda , 0xd7 , 0x6d , 0xb7 , 0x76 , 0x59 , 0x96 , 0x44 , 0x9f , 0x79 , 0xab , 0xd8,
    0x18 , 0xe2 , 0x00 , 0xb7 , 0x67 , 0xa0 , 0x0f , 0x59 , 0x9b , 0x9f , 0x8b , 0xb0 , 0x5d , 0x6f , 0x9f , 0xa6,
    0x74 , 0xe0 , 0xfd , 0x14 , 0x4d , 0xae , 0x23 , 0x9b , 0x5d , 0x18 , 0x7f , 0x37 , 0x82 , 0x03 , 0x44 , 0xac,
    0xca , 0x6d , 0x88 , 0xc3 , 0x2c , 0x5b , 0x47 , 0x8c , 0xb5 , 0x07 , 0x6b , 0x9c , 0xd7 , 0x6b , 0x64 , 0x1e,
    0x82 , 0x6e , 0xb8 , 0x7e , 0xd0 , 0xc2 , 0xf4 , 0x26 , 0x17 , 0xe1 , 0xdc , 0x2f , 0x81 , 0x3b , 0xa6 , 0x9e,
    0x1f , 0xbe , 0xc6 , 0x89 , 0xc4 , 0x1d , 0xa3 , 0xe1 , 0x2f , 0x88 , 0x5f , 0x53 , 0xd8 , 0xae , 0xe6 , 0xef,
    0xb2 , 0x5c , 0x6a , 0xba , 0xba , 0xfd , 0x29 , 0xfe , 0x99 , 0x1a , 0x89 , 0x73 , 0xf7 , 0xc0 , 0x6e , 0xb7,
    0x13 , 0x58 , 0xc8 , 0x9a , 0x29 , 0x17 , 0xba , 0xf2 , 0x6c , 0xd7 , 0x16 , 0xc4 , 0x9e , 0x3b , 0xe8 , 0x90,
    0x72 , 0x40 , 0xa6 , 0x4f , 0xee , 0x45 , 0x9d , 0xe9 , 0xae , 0xa0 , 0x41 , 0xfc , 0xc6 , 0x01 , 0x9b , 0x26,
    0x38 , 0xf6 , 0x5d , 0x11 , 0xa1 , 0x18 , 0x54 , 0x2d , 0x15 , 0x60 , 0x7a , 0xac , 0xc7 , 0x7a , 0x18 , 0x1d,
    0xeb , 0xcf , 0x15 , 0x4c , 0xdd , 0x88 , 0xa4 , 0x41 , 0xbe , 0x6b , 0x1a , 0x5a , 0x4f , 0xf7 , 0x8e , 0xbe,
    0x5b , 0xc9 , 0x6a , 0x2f , 0x6a , 0x74 , 0xb0 , 0xfc , 0x8b , 0xe8 , 0xae , 0x68 , 0xd7 , 0x86 , 0xad , 0x60,
    0x5b , 0xf0 , 0x8a , 0x2c , 0xe1 , 0xb2 , 0xf9 , 0x3f , 0x35 , 0x5d , 0xa3 , 0xd6 , 0x5e , 0xfd , 0x12 , 0xb6,
    0x21 , 0xca , 0x72 , 0xa9 , 0xba , 0xf2 , 0x97 , 0xbc , 0x2d , 0x79 , 0xba , 0xd6 , 0x48 , 0x53 , 0x97 , 0x5d,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x01 , 0x00 , 0x01
};

HI_S32 main(HI_S32 argc,HI_CHAR** argv)
{
    HI_S32 Ret;
    HI_U8 u8RSAKey[512] = {0};
    HI_U32 i = 0;

    Ret = HI_UNF_ADVCA_Init();
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("HI_UNF_ADVCA_Init failed\n");
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_SetRSAKey(rsa_key);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("set RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit(); 
        return HI_FAILURE;
    }

    Ret = HI_UNF_ADVCA_GetRSAKey(u8RSAKey);
    if (HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("get RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit(); 
        return HI_FAILURE;
    }
    // The host CPU cann't read the RSA Key after the lock operation.
    Ret = HI_UNF_ADVCA_ConfigLockFlag(HI_UNF_ADVCA_LOCK_RSA_KEY, HI_NULL, 0);
    if(HI_SUCCESS != Ret)
    {
        HI_DEBUG_ADVCA("Lock RSA key failed\n");
        (HI_VOID)HI_UNF_ADVCA_DeInit(); 
        return HI_FAILURE;
    }
    
    printf("The RSA Key are:\n");
    for (i = 0; i < 512; i++)
    {
        printf("%02x%s", u8RSAKey[i], ((i + 1) % 16) ? " " : "\n");
    }

    (HI_VOID)HI_UNF_ADVCA_DeInit(); 
    return HI_SUCCESS;
}


