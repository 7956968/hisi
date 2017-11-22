/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	    : sample_scrolltext.c
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
#include "hi_go.h"
#include "sample_displayInit.h"

/***************************** Macro Definition ******************************/

#define FONT_NAME "./res/DroidSansFallbackLegacy.ttf"

#define WIDTH 4092//��Ļbuffer��,������Ļ�ַ��������ô�ֵ��Խ���ʾ֧���ַ�����Խ�࣬��Ļbuffer������ܴ��ڵ���4096

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{

    HI_S32 s32Ret   = 0;
    HI_HANDLE hFont = HIGO_INVALID_HANDLE;
    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HI_HANDLE hLayer    = HIGO_INVALID_HANDLE;
    HI_HANDLE hLayerAD0 = HIGO_INVALID_HANDLE;
    HI_HANDLE hLayerAD1 = HIGO_INVALID_HANDLE;
    HI_HANDLE hLayerSurface;
    HI_RECT stRect   = {0, 0, WIDTH, 36};
    HI_CHAR szText[] = "abcdefghijklmnopqrstuvwxyz0abcdefghijklmnopqrstuvwxyz1abcdefghijklmnopqrstuvwxyz2abcdefghijklmnopqrstuvwxyz3abcdefghijklmnopqrstuvwxyz4abcdefghijklmnopqrstuvwxyz5abcdefghijklmnopqrstuvwxyz6abcdefghijklmnopqrstuvwxyz7abcdefghijklmnopqrstuvwxyz8abcdefghijklmnopqrstuvwxyz9";//"不能链接到您请求的网页此页可能暂时不可用如果您以前查看过该页";

#ifdef CHIP_TYPE_hi3110ev500
    HIGO_LAYER_E eLayerID = HIGO_LAYER_SD_0;
#else
    HIGO_LAYER_E eLayerID = HIGO_LAYER_HD_0;
#endif

    HI_HANDLE hTmpSurface;
    HI_PIXELDATA pData;  
    HI_HANDLE hScrollText0,hScrollText1;
    HIGO_SCROLLTEXT_ATTR_S stScrollAttr = {0};
    HIGO_SCROLLTEXT_DATA_S stScrollData = {0};
    HI_BOOL bScrollTextPause = HI_FALSE;  
    HI_U32  u32Cnt           = 0;
    HI_U32 u32Offset         = 0;
    HI_U32 u32FbVramSize     = 0;
    HI_S32 s32FmtDepth       = 0;
    
    #ifdef CFG_HI_HD_FB_VRAM_SIZE
    u32FbVramSize = CFG_HI_HD_FB_VRAM_SIZE;
    #endif

    
    s32Ret = HI_GO_Init();
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: init higo s32Ret:%#x\n", __LINE__, s32Ret);
        return 0;
    }

    s32Ret = Display_Init();
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: init display s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_Deinit();
        return 0;
    }



    //������ͼ�β�
    s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo); 
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: get layer param s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;
    }
    
    if(u32FbVramSize < HI_HD_FB_VRAM_SIZE)
   	{
        stLayerInfo.PixelFormat = HIGO_PF_1555;
    	s32FmtDepth             = 2;
    }
    else
    {
        stLayerInfo.PixelFormat = HIGO_PF_8888;
    	s32FmtDepth             = 4;
    }
    
	#ifdef CHIP_TYPE_hi3110ev500
        stLayerInfo.PixelFormat = HIGO_PF_1555;
		s32FmtDepth             = 2;
	#endif

    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;      
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: create layer s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;
    }

    s32Ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: get layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }

    s32Ret = HI_GO_FillRect(hLayerSurface, NULL, 0xFF0000FF, HIGO_COMPOPT_NONE);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: fill layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }


 
    //����������Ļͼ��
    s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_AD_0, &stLayerInfo); 
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: get layer param s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;
    }
    if(u32FbVramSize < HI_HD_FB_VRAM_SIZE)
        stLayerInfo.PixelFormat = HIGO_PF_1555;
    else
        stLayerInfo.PixelFormat = HIGO_PF_8888;

	#ifdef CHIP_TYPE_hi3110ev500
        stLayerInfo.PixelFormat = HIGO_PF_1555;
	#endif


    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;      
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayerAD0);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: create layer s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;
    }

    
    s32Ret = HI_GO_GetLayerSurface(hLayerAD0, &hLayerSurface);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: get layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }

    s32Ret = HI_GO_FillRect(hLayerSurface, NULL, 0xFF0000FF, HIGO_COMPOPT_NONE);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: fill layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }

    /*��Ҫ���������Ļ��������֮ǰˢ��һ��layer*/
    s32Ret = HI_GO_RefreshLayer(hLayerAD0, NULL);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: refresh layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }

    s32Ret = HI_GO_CreateText(FONT_NAME, NULL,  &hFont);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: create text s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;       
    }

    s32Ret = HI_GO_SetTextColor(hFont, 0xffffffff);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: create text s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;       
    }

    /*����������Ļsurface*/
    s32Ret = HI_GO_CreateSurface(WIDTH, 36, stLayerInfo.PixelFormat, &hTmpSurface);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: create temp surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;       
    }

    /*����������Ļbuffer�׵�ַ*/
    s32Ret = HI_GO_LockSurface(hTmpSurface, pData, HI_FALSE);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: lock temp surface s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_FreeSurface(hTmpSurface);
        goto RET;       
    }
    
    s32Ret = HI_GO_UnlockSurface(hTmpSurface);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: unlock temp surface s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_FreeSurface(hTmpSurface);
        goto RET;       
    }


    /*����������Ļ0*/
    stScrollAttr.ePixelFormat   = stLayerInfo.PixelFormat; /** ��Ҫ��layerһ��          **/
    stScrollAttr.Layer          = hLayerAD0;               /** ������Ļ������layer      **/
    stScrollAttr.u16CacheNum    = 2;                       /** ������Ļʹ�õĻ�������   **/
    stScrollAttr.stScrollRect.x = 0;                       /** ������Ļ����Ļ�ϵ�startx **/
    stScrollAttr.stScrollRect.y = 20;                      /** ������Ļ����Ļ�ϵ�starty **/
    
	#ifdef CHIP_TYPE_hi3110ev500
    stScrollAttr.stScrollRect.w = 720;                     /** ������Ļ����Ļ�ϵ�widht  **/
	#else
    stScrollAttr.stScrollRect.w = 1280;                    /** ������Ļ����Ļ�ϵ�widht  **/
	#endif

    stScrollAttr.stScrollRect.h = 36;                      /** ������Ļ����Ļ�ϵ�height **/
    stScrollAttr.bDeflicker     = HI_TRUE;                 /** ʹ�ܿ�������             **/
    s32Ret = HI_GO_CreateScrollText(&stScrollAttr, &hScrollText0);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: create scroll text s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_UnlockSurface(hTmpSurface);
        HI_GO_FreeSurface(hTmpSurface);
        goto RET;       
    }

    /*����������Ļ1*/
    stScrollAttr.ePixelFormat     = stLayerInfo.PixelFormat; /** ��Ҫ��layerһ��          */
    stScrollAttr.Layer            = hLayer;                  /** ������Ļ������layer      */
    stScrollAttr.u16CacheNum      = 2;                       /** ������Ļʹ�õĻ�������   */
    stScrollAttr.stScrollRect.x   = 0;                       /** ������Ļ����Ļ�ϵ�startx */

	#ifdef CHIP_TYPE_hi3110ev500
	stScrollAttr.stScrollRect.y   = 500;                     /** ������Ļ����Ļ�ϵ�starty */
    stScrollAttr.stScrollRect.w   = 720;                     /** ������Ļ����Ļ�ϵ�widht  */
	#else
	stScrollAttr.stScrollRect.y   = 664;                     /** ������Ļ����Ļ�ϵ�starty */
    stScrollAttr.stScrollRect.w   = 1280;                    /** ������Ļ����Ļ�ϵ�widht  */
	#endif
    
    stScrollAttr.stScrollRect.h  = 36;                       /** ������Ļ����Ļ�ϵ�height */
    stScrollAttr.bDeflicker      = HI_TRUE;                  /** ʹ�ܿ�������             */
    s32Ret = HI_GO_CreateScrollText(&stScrollAttr, &hScrollText1);
    if (HI_SUCCESS != s32Ret) 
    {
        SAMPLE_TRACE("<line:%d>error: create scroll text s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_UnlockSurface(hTmpSurface);
        HI_GO_FreeSurface(hTmpSurface);
        goto RET;       
    }
    
    stRect.y = 0;
    stRect.h = 36;

#ifdef CHIP_TYPE_hi3110ev500
    stRect.x = 720;
#else
    stRect.x = 1280;
#endif
    stRect.w = WIDTH - stRect.x;

    /*������Ļ����Ϳ��*/
    s32Ret = HI_GO_FillRect(hTmpSurface, NULL, 0xFF000000, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: fill temp surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET; 
    }
    /*���������Ļ�ı���������Ļsurface*/
    s32Ret = HI_GO_TextOutEx(hFont, hTmpSurface, szText, &stRect, HIGO_LAYOUT_LEFT);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: text out s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET; 
    }

    /*ͬ����ȷ��������Ļ�ı�������*/
    s32Ret = HI_GO_SyncSurface(hTmpSurface, HIGO_SYNC_MODE_TDE);
    if (HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: HI_GO_SyncSurface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET; 
    }

    /*��Ҫ���������Ļ��������֮ǰˢ��һ��layer*/
    s32Ret = HI_GO_RefreshLayer(hLayer, NULL);
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: refresh layer surface s32Ret:%#x\n", __LINE__, s32Ret);
        goto RET;        
    }

    while(1)
    {
        /*�����ƶ�����*/
        if (!bScrollTextPause)
        {
            u32Offset += 3;
            if (u32Offset >= WIDTH)
            {
                u32Offset = 0;
                continue;
            }
        }     
        //��������Ļ0

		/**
		 ** ����ֵ�������ַʱ����ʹ�������ַ
         ** �������ַΪ0ʱ��ʹ�������ַ
         **/
        stScrollData.pu8VirAddr = (HI_U8 *)pData[0].pData   + u32Offset * s32FmtDepth; /** ������Ļ�������ַ **/
        stScrollData.u32PhyAddr = (HI_U32)pData[0].pPhyData + u32Offset * s32FmtDepth; /** ������Ļ�������ַ **/
        stScrollData.u32Stride  = pData[0].Pitch;                                      /** ������Ļ��stride   **/
        s32Ret = HI_GO_FillScrollText(hScrollText0, &stScrollData);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_TRACE("<line:%d>error: HI_GO_FillScrollText s32Ret:%#x\n", __LINE__, s32Ret);
            continue;
        }
        //��������Ļ1
        stScrollData.pu8VirAddr = (HI_U8 *)pData[0].pData + u32Offset   * s32FmtDepth;   /** ������Ļ�������ַ **/
        stScrollData.u32PhyAddr = (HI_U32)pData[0].pPhyData + u32Offset * s32FmtDepth;   /** ������Ļ�������ַ **/
        stScrollData.u32Stride  = pData[0].Pitch;                                        /** ������Ļ��stride   **/
        s32Ret = HI_GO_FillScrollText(hScrollText1, &stScrollData);
        if (HI_SUCCESS != s32Ret)
        {
            SAMPLE_TRACE("<line:%d>error: HI_GO_FillScrollText s32Ret:%#x\n", __LINE__, s32Ret);
            continue;
        }
    
        /*��ͣ������Ļ*/
        if(1000 == u32Cnt)
        {
            s32Ret = HI_GO_PauseScrollText(hScrollText0);
            if(HI_SUCCESS != s32Ret)
            {
                SAMPLE_TRACE("<line:%d>error: stop scroll s32Ret:%#x\n", __LINE__, s32Ret);
            }
            bScrollTextPause = HI_TRUE;
        }
        /*�ָ�������Ļ����*/
        if(2000 == u32Cnt)
        {
            s32Ret = HI_GO_ResumeScrollText(hScrollText0);
            if(HI_SUCCESS != s32Ret)
            {
                SAMPLE_TRACE("<line:%d>error: resume scroll s32Ret:%#x\n", __LINE__, s32Ret);
            }
            bScrollTextPause = HI_FALSE;
        }


        if(3000 == u32Cnt)
        {
            /*���ٹ�����Ļ*/
            s32Ret = HI_GO_DestoryScrollText(hScrollText0);
            if(HI_SUCCESS != s32Ret)
            {
                SAMPLE_TRACE("<line:%d>error: destory scroll s32Ret:%#x\n", __LINE__, s32Ret);
            }
            else
            {
                stScrollAttr.ePixelFormat   = stLayerInfo.PixelFormat;   /** ��Ҫ��layerһ��            **/
                stScrollAttr.Layer          = hLayerAD0;                 /** ������Ļ������layer        **/
                stScrollAttr.u16CacheNum    = 2;                         /** ������Ļʹ�õĻ�������     **/
                stScrollAttr.stScrollRect.x = 0;                         /** ������Ļ����Ļ�ϵ�startx   **/
                stScrollAttr.stScrollRect.y = 20;                        /** ������Ļ����Ļ�ϵ�starty   **/
                #ifdef CHIP_TYPE_hi3110ev500
                    stScrollAttr.stScrollRect.w = 720;                   /** ������Ļ����Ļ�ϵ�widht    **/
                #else
                    stScrollAttr.stScrollRect.w = 1280;                  /** ������Ļ����Ļ�ϵ�widht    **/
                #endif
                stScrollAttr.stScrollRect.h = 36;                        /** ������Ļ����Ļ�ϵ�height   **/
                stScrollAttr.bDeflicker     = HI_TRUE;                   /** ʹ�ܿ�������               **/
                s32Ret = HI_GO_CreateScrollText(&stScrollAttr, &hScrollText0);
                if (HI_SUCCESS != s32Ret) 
                {
                    SAMPLE_TRACE("<line:%d>error: create scroll text s32Ret:%#x\n", __LINE__, s32Ret);
                    HI_GO_UnlockSurface(hTmpSurface);
                    HI_GO_FreeSurface(hTmpSurface);
                    goto RET;       
                }
                u32Cnt = 0;
            }
        }

        u32Cnt++;
    }

    HI_GO_DestoryScrollText(hScrollText0);
    HI_GO_DestoryScrollText(hScrollText1);
    HI_GO_FreeSurface(hTmpSurface);

RET:
    if (HIGO_INVALID_HANDLE != hLayer)
    {
        HI_GO_DestroyLayer(hLayer);
    }
    if (HIGO_INVALID_HANDLE != hLayerAD0)
    {
        HI_GO_DestroyLayer(hLayerAD0);
    }  
    if (HIGO_INVALID_HANDLE != hLayerAD1)
    {
        HI_GO_DestroyLayer(hLayerAD1);
    }     
    if  (HIGO_INVALID_HANDLE != hFont)
    {
        HI_GO_DestroyText(hFont);
    }

    Display_DeInit();
    
    HI_GO_Deinit();

    return HI_SUCCESS;
}
