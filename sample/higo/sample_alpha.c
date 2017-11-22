/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : sample_surface.c
Version		    : Initial Draft
Author		    : 
Created		    : 2014/07/22
Description	    :
Function List 	: 

			  		  
History       	:
Date				Author        		Modification
2014/07/22  	    y00181162  		    Created file      	
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/

#define SCREEN_WIDTH       1280
#define SCREEN_HEIGHT      720

/** G0����棬G2��UI **/
//#define CONFIG_UI_FOR_HD

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_HANDLE hLayerHD        = HI_INVALID_HANDLE;
    HI_HANDLE hLayerSurfaceHD = HI_INVALID_HANDLE;
    HI_HANDLE hLayerAD        = HI_INVALID_HANDLE;
    HI_HANDLE hLayerSurfaceAD = HI_INVALID_HANDLE;
    HI_HANDLE hMemSurface     = HI_INVALID_HANDLE;

    HIGO_LAYER_INFO_S stLayerInfo;
    HIGO_BLTOPT_S stBlitOpt;
    HI_RECT stDstRect = {0};
//    HIGO_LAYER_ALPHA_S AlphaInfo = {0};
        
    Display_Init();
    HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_HD0, HI_UNF_DISP_LAYER_OSD_0, HI_LAYER_ZORDER_MOVETOP);

    
    s32Ret = HI_GO_Init();
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }
    
	s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_HD_0, &stLayerInfo);
	if(HI_SUCCESS != s32Ret){
		goto ERROR_EXIT;
	}
    stLayerInfo.ScreenWidth   = SCREEN_WIDTH;
	stLayerInfo.ScreenHeight  = SCREEN_HEIGHT;
	stLayerInfo.CanvasWidth   = SCREEN_WIDTH;
	stLayerInfo.CanvasHeight  = SCREEN_HEIGHT;
	stLayerInfo.DisplayWidth  = SCREEN_WIDTH;
	stLayerInfo.DisplayHeight = SCREEN_HEIGHT;
	s32Ret = HI_GO_CreateLayer(&stLayerInfo,&hLayerHD);
	if(HI_SUCCESS != s32Ret){
		goto ERROR_EXIT;
	}

    s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_AD_0, &stLayerInfo);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }
	stLayerInfo.ScreenWidth   = SCREEN_WIDTH;
	stLayerInfo.ScreenHeight  = SCREEN_HEIGHT;
	stLayerInfo.CanvasWidth   = SCREEN_WIDTH;
	stLayerInfo.CanvasHeight  = SCREEN_HEIGHT;
	stLayerInfo.DisplayWidth  = SCREEN_WIDTH;
	stLayerInfo.DisplayHeight = SCREEN_HEIGHT;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo,&hLayerAD);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }
    
#ifdef CONFIG_UI_FOR_HD
    s32Ret = HI_GO_SetLayerZorder(hLayerHD, HIGO_LAYER_ZORDER_MOVETOP);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }
#else
    /**
     ** ˫ͼ����ӻ�дCSC��������Ϊ���Ӻ��Ԥ������
     ** ��CSC�Ƿ�ʹ��Ԥ������ȡ����G0
     ** ʹ�����ƣ�G0���£���Ԥ�ˣ�������͸��Ч������Ϊ��Ļʹ��,Z������
     **/
    s32Ret = HI_GO_SetLayerAlpha(hLayerHD, 0xff);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }
#endif

    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("begin display g0 red color\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    
	s32Ret = HI_GO_GetLayerSurface (hLayerHD,&hLayerSurfaceHD); 
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}
    
    /** ����alphaҪΪ0��������ڵ���Ƶ�� **/
    s32Ret = HI_GO_FillRect(hLayerSurfaceHD,NULL,0x00000000,HIGO_COMPOPT_NONE);
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}
    
	s32Ret = HI_GO_CreateSurface(320,240,HIGO_PF_1555,&hMemSurface);
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}
    
	s32Ret = HI_GO_SetSurfaceAlpha(hMemSurface,254);
	if(HI_SUCCESS != s32Ret){
		goto ERROR_EXIT;
    }
    
	s32Ret = HI_GO_FillRect(hMemSurface,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}

	stDstRect.x = 0;
    stDstRect.y = 0;
	stDstRect.w = 320;
	stDstRect.h = 240;
	memset(&stBlitOpt,0,sizeof(HIGO_BLTOPT_S));
	s32Ret = HI_GO_Blit(hMemSurface,NULL,hLayerSurfaceHD,&stDstRect,&stBlitOpt);
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}
    
	s32Ret = HI_GO_RefreshLayer(hLayerHD, NULL);
	if(HI_SUCCESS != s32Ret){
		goto ERROR_EXIT;
	}


    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("begin display g2 red color\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    
    s32Ret = HI_GO_GetLayerSurface (hLayerAD,&hLayerSurfaceAD); 
    if(HI_SUCCESS != s32Ret){
       goto ERROR_EXIT;
    }
    
    /** ����alphaҪΪ0��������ڵ���Ƶ�� **/
    s32Ret = HI_GO_FillRect(hLayerSurfaceAD,NULL,0x00000000,HIGO_COMPOPT_NONE);
	if(HI_SUCCESS != s32Ret){
	   goto ERROR_EXIT;
	}
    
    s32Ret = HI_GO_SetSurfaceAlpha(hMemSurface,253);
    if (HI_SUCCESS != s32Ret){
        goto ERROR_EXIT; 
    }
    
    s32Ret = HI_GO_FillRect(hMemSurface,NULL,0xFFFF0000,HIGO_COMPOPT_NONE);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT;
    }

    stDstRect.x = 960;
    stDstRect.y = 480;
    stDstRect.w = 320;
    stDstRect.h = 240;
    memset(&stBlitOpt,0,sizeof(HIGO_BLTOPT_S));
    s32Ret = HI_GO_Blit(hMemSurface,NULL,hLayerSurfaceAD,&stDstRect,&stBlitOpt);
    if(HI_SUCCESS != s32Ret){
       goto ERROR_EXIT; 
    }
    
    s32Ret = HI_GO_RefreshLayer(hLayerAD, NULL);
    if(HI_SUCCESS != s32Ret){
        goto ERROR_EXIT; 
    }
    
    
#ifdef CONFIG_UI_FOR_HD
    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("set g0 layer global alpha\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    s32Ret = HI_GO_SetLayerAlpha(hLayerHD, 0x50);
    if (HI_SUCCESS != s32Ret){
        return HI_FAILURE;
    }
#else
    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("set g2 layer global alpha\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    s32Ret = HI_GO_SetLayerAlpha(hLayerAD, 0x50);
    if (HI_SUCCESS != s32Ret){
        return HI_FAILURE;
    }
#endif

#if 0
#ifndef CONFIG_UI_FOR_HD
    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("set g0 layer pixle alpha\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    /**
     **alpha0ֵ�������ݸ�ʽΪalphaRGB1555ʱ��alphaֵΪ0ʱ���ø�ֵ�滻��
     **alpha1ֵ�������ݸ�ʽΪalphaRGB1555ʱ��alphaֵΪ1ʱ���ø�ֵ�滻��
     **/
    s32Ret = HI_GO_GetLayerAlphaEx(hLayerHD,  &AlphaInfo);
    AlphaInfo.Alpha0 = 0x80;
    AlphaInfo.Alpha1 = 0x80;
    s32Ret = HI_GO_SetLayerAlphaEx(hLayerHD,  &AlphaInfo);
#else
    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("set g2 layer pixle alpha\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    s32Ret = HI_GO_GetLayerAlphaEx(hLayerAD,  &AlphaInfo);
    AlphaInfo.Alpha0 = 0x80;
    AlphaInfo.Alpha1 = 0x80;
    s32Ret = HI_GO_SetLayerAlphaEx(hLayerAD,  &AlphaInfo);
#endif
#endif

    SAMPLE_TRACE("\n================================================================\n");
    SAMPLE_TRACE("exit\n");
    SAMPLE_TRACE("================================================================\n");
    getchar();
    

ERROR_EXIT:
    if(HI_INVALID_HANDLE != hLayerHD){
        HI_GO_DestroyLayer(hLayerHD);
    }
    if(HI_INVALID_HANDLE != hLayerAD){
        HI_GO_DestroyLayer(hLayerAD);
    }
    if(HI_INVALID_HANDLE != hMemSurface){
        HI_GO_FreeSurface(hMemSurface);
    }

    HI_GO_Deinit();

    Display_DeInit();
    
    return HI_SUCCESS;
    
}
