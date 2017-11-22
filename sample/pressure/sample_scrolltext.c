/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : higo_stereo.c
Version             : Initial Draft
Author              : 
Created             : 2014/08/06
Description         : ������Ļ�㣬���Դ�����������������Ļ
                      �������Ρ�
Function List       : 
History             :
Date                       Author                   Modification
2014/08/06                 y00181162                Created file        
******************************************************************************/

/*********************************add include here******************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>

#include "hi_go.h"
#include "sample_displayInit.h"


/***************************** Macro Definition ******************************/
#define FONT_NAME        "./res/DroidSansFallbackLegacy.ttf"
#define JPEG_FILE_NAME   "./res/jpg/Img201007080005_N.JPG"

#define WIDTH            2560

#define SCREEN_WIDTH       1280
#define SCREEN_HEIGHT      720

/*************************** Structure Definition ****************************/


/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
static HI_S32 jpeg_dec(HI_CHAR *pszFileName, HI_HANDLE *pSurface);


/*****************************************************************************
* func            : DISP_Init
* description     : init display
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
void __attribute__ ((constructor)) DISP_Init(void)
{
    Display_Init();
    HI_GO_Init();
    return;
}

/*****************************************************************************
* func            : DISP_DInit
* description     : dinit display
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
void __attribute__ ((destructor)) DISP_DInit(void)
{
    HI_GO_Deinit();
    Display_DeInit();
    return;
}

/*****************************************************************************
* func			: PTHREAD_DISP_ScrollText
* description	: ������Ļ
* others:		: NA
*****************************************************************************/
static HI_VOID *PTHREAD_DISP_ScrollText(HI_VOID* pTreadTask)
{

    HI_S32 s32Ret    = HI_SUCCESS;
    HI_HANDLE hFont  = HIGO_INVALID_HANDLE;
    HI_RECT stRect   = {0, 0, WIDTH, 36};
    HI_CHAR szText[] = "abcdefghijklmnopqrstuvwxyz0abcdefghijklmnopqrstuvwxyz1abcdefghijklmnopqrstuvwxyz2abcdefghijklmnopqrstuvwxyz3abcdefghijklmnopqrstuvwxyz4abcdefghijklmnopqrstuvwxyz5abcdefghijklmnopqrstuvwxyz6abcdefghijklmnopqrstuvwxyz7abcdefghijklmnopqrstuvwxyz8abcdefghijklmnopqrstuvwxyz9";//"不能链接到您请求的网页此页可能暂时不可用如果您以前查看过该页";
    
    HI_PIXELDATA pData;  
    HI_HANDLE hScrollTextHandle;
    HI_BOOL bScrollTextPause = HI_FALSE;  
    HI_U32  u32Cnt = 0;
    HI_U32 u32Offset = 0;

    HIGO_LAYER_INFO_S stLayerInfo = {0};
    HI_HANDLE hScrollTextLayer = HIGO_INVALID_HANDLE;
    HI_HANDLE hScrollTextSurface;
    HIGO_SCROLLTEXT_ATTR_S stScrollAttr = {0};
    HIGO_SCROLLTEXT_DATA_S stScrollData = {0};

    
    /** ��ȡ��Ļ��Ĭ����Ϣ **/
    s32Ret = HI_GO_GetLayerDefaultParam(HIGO_LAYER_AD_0, &stLayerInfo); 
    if(HI_SUCCESS != s32Ret)
    {
        return NULL;
    }
    stLayerInfo.CanvasHeight   = 36;
    stLayerInfo.DisplayHeight  = 36;
    stLayerInfo.ScreenHeight   = 36; /** ֧��1280 * 90**/
    stLayerInfo.PixelFormat    = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL; /** ��buffer,no display buffer **/
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hScrollTextLayer);
    if(HI_SUCCESS != s32Ret)
    {
        return NULL;
    }
    /** create text surface **/
    s32Ret = HI_GO_CreateSurface(WIDTH, 36, stLayerInfo.PixelFormat, &hScrollTextSurface);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR3;
    }

    /** ��ȡsurface������ݣ����ﲻ��Ҫͬ��  **/
    s32Ret = HI_GO_LockSurface(hScrollTextSurface, pData, HI_FALSE);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR2;
    }
    s32Ret = HI_GO_UnlockSurface(hScrollTextSurface);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR2;
    }
    
    /** ��������Ļ����ɫ **/
    s32Ret = HI_GO_FillRect(hScrollTextSurface, NULL, 0xFFFFFFFF, HIGO_COMPOPT_NONE);
    if (HI_SUCCESS != s32Ret)
    {
        HI_PRINT("[%s:%d]\n", __FUNCTION__, __LINE__);
        goto ERR2;
    }
    HI_GO_SyncSurface(hScrollTextSurface, HIGO_SYNC_MODE_CPU);

    
    /** �������� **/
    s32Ret = HI_GO_CreateText(FONT_NAME, NULL,  &hFont);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR2;
    }
    /** ����������ɫ **/
    s32Ret = HI_GO_SetTextColor(hFont, 0xff000000);
    if (HI_SUCCESS != s32Ret) 
    {
        goto ERR1;
    }

    /** ���Ҫ���������� **/
    stRect.x = 1280;
    stRect.y = 4;
    stRect.w = WIDTH - stRect.x;
    stRect.h = 90;
    s32Ret = HI_GO_TextOutEx(hFont, hScrollTextSurface, szText, &stRect, HIGO_LAYOUT_LEFT);
    if (HI_SUCCESS != s32Ret)
    {
        HI_PRINT("[%s:%d]\n", __FUNCTION__, __LINE__);
        goto ERR1;
    }
    /** ����������Ļ **/
    stScrollAttr.ePixelFormat    = stLayerInfo.PixelFormat; /** ���ظ�ʽ               **/
    stScrollAttr.Layer           = hScrollTextLayer;        /** �󶨵�ͼ��             **/
    stScrollAttr.u16CacheNum     = 2;                       /** ˫bufferģʽ           **/
    stScrollAttr.stScrollRect.x  = 0;                       /** ��ʼX����              **/
    stScrollAttr.stScrollRect.y  = 0;                       /** ��ʼY����              **/
    stScrollAttr.stScrollRect.w  = 1280;                    /** ��Ļ���               **/
    stScrollAttr.stScrollRect.h  = 36;                      /** ��Ļ�߶�               **/
    stScrollAttr.u16RefreshFreq  = 0;                       /** ˢ��Ƶ��,���û������  **/
    stScrollAttr.bDeflicker      = HI_TRUE;                 /** �Ƿ���               **/
    s32Ret = HI_GO_CreateScrollText(&stScrollAttr, &hScrollTextHandle);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR1;
    }
    /** ���ݴ��� **/
    while(1)
    {
        if (!bScrollTextPause)
        {/** û����ͣ����ı�ƫ���� **/
            u32Offset += 3; /** 3������3�����ؽ���ƫ�� **/
            if(u32Offset >= WIDTH)
            {
                u32Offset = 0;
                continue;
            }
        }
        /**
         ** physical addr is perfered to virtual addr;virtual addr 
         ** will be used only if physical addr is equal to zero
         ** ��������ݾ��ǹ�����Ļ����surface������
         **/
        stScrollData.pu8VirAddr = (HI_U8 *)pData[0].pData   + u32Offset * 4;    /*virtual addr*/
        stScrollData.u32PhyAddr = (HI_U32)pData[0].pPhyData + u32Offset * 4;    /*physical addr*/
        stScrollData.u32Stride  = pData[0].Pitch;

        /** ���ݴ��� **/
        s32Ret = HI_GO_FillScrollText(hScrollTextHandle, &stScrollData);
        if (HI_SUCCESS != s32Ret)
        {
            continue;
        }
        /*pause scrolltext*/
        if(1000 == u32Cnt)
        {
            HI_PRINT("===== pause scroll text\n");
            HI_GO_PauseScrollText(hScrollTextHandle);
            bScrollTextPause = HI_TRUE;
        }
        /*resume scrolltext*/
        if(2000 == u32Cnt)
        {
            HI_PRINT("===== resume scroll text\n");
            HI_GO_ResumeScrollText(hScrollTextHandle);
            bScrollTextPause = HI_FALSE;
        }
        u32Cnt++;
        
    }
    
    HI_GO_DestoryScrollText(hScrollTextHandle);
ERR1:
    HI_GO_DestroyText(hFont);
ERR2:
    HI_GO_FreeSurface(hScrollTextSurface);
ERR3:
    HI_GO_DestroyLayer(hScrollTextLayer);

    return NULL;
    
}

/*****************************************************************************
* func			: PTHREAD_DISP_HD0_Layer
* description	: ��UI����ʾ
* others:		: NA
*****************************************************************************/
static HI_VOID *PTHREAD_DISP_HD0_Layer(HI_VOID* pTreadTask)
{
    HI_S32 s32Ret  = HI_SUCCESS;
    HI_S32 s32MaxW = 0;
    HI_HANDLE hDecSurface, hLayer, hLayerSurface;
    HI_RECT stRect;
    HIGO_LAYER_INFO_S stLayerInfo ;
    HIGO_BLTOPT_S stBltOpt = {0};
    HIGO_LAYER_E eLayerID  = HIGO_LAYER_HD_0;
    
    s32Ret = HI_GO_GetLayerDefaultParam(eLayerID, &stLayerInfo);
    if (HI_SUCCESS != s32Ret)
    {
        return NULL;
    }
    stLayerInfo.ScreenWidth    = SCREEN_WIDTH;
    stLayerInfo.ScreenHeight   = SCREEN_HEIGHT;
    stLayerInfo.DisplayWidth   = SCREEN_WIDTH;
    stLayerInfo.DisplayHeight  = SCREEN_HEIGHT;
    stLayerInfo.CanvasWidth    = SCREEN_WIDTH;
    stLayerInfo.CanvasHeight   = SCREEN_HEIGHT;
    stLayerInfo.PixelFormat    = HIGO_PF_8888;
    stLayerInfo.LayerFlushType = HIGO_LAYER_FLUSH_NORMAL;
    s32Ret = HI_GO_CreateLayer(&stLayerInfo, &hLayer);
    if (HI_SUCCESS != s32Ret)
    {
        return NULL;
    }

    s32Ret = HI_GO_GetLayerSurface(hLayer, &hLayerSurface);
    if (HI_SUCCESS != s32Ret)
    {
        goto ERR2;
    }

    /**
     **rect pos and size
     **/
    stRect.x = 100;
    stRect.y = stLayerInfo.DisplayHeight/2 - 50;
    stRect.w = stLayerInfo.DisplayWidth - 2 * stRect.x;
    stRect.h = 2 * 50;

    s32MaxW = stLayerInfo.DisplayWidth - 2 * stRect.x;

    stBltOpt.EnableScale = HI_TRUE;
    HI_GO_FillRect(hLayerSurface, NULL, 0x80000000, HIGO_COMPOPT_NONE);

    /**decoding*/
    s32Ret = jpeg_dec(JPEG_FILE_NAME, &hDecSurface);
    if(HI_SUCCESS != s32Ret)
    {
        HI_PRINT("========= jpeg dec failure \n");
        goto ERR2;
    }

    while (1)
    {
        /** Blit it to graphic layer Surface */
        s32Ret = HI_GO_Blit(hDecSurface, NULL, hLayerSurface, &stRect, &stBltOpt);
        if (HI_SUCCESS == s32Ret)
        {
            /** fresh display*/
            HI_GO_RefreshLayer(hLayer, NULL);
            HI_GO_FillRect(hLayerSurface, &stRect, 0x80000000, HIGO_COMPOPT_NONE);
            if(stRect.w >= s32MaxW)
            {
               stRect.w = 5;
            }
            else
            {
               stRect.w += 1;
            }
        }

        usleep(5 * 1000);
    }

    /** free the memory which saving the file name */
    usleep(200 * 1000);

    /**  free Surface */
    HI_GO_FreeSurface(hDecSurface);

ERR2:
    s32Ret |= HI_GO_DestroyLayer(hLayer);

    return NULL;
    
}

/*****************************************************************************
* func            main
: description  :  �ߵ��²���
* retval       :  NA
* others:	   :  NA
*****************************************************************************/
HI_S32 main(HI_S32 argc, HI_CHAR* argv[])
{
    HI_S32 s32Ret = HI_SUCCESS;
    pthread_t pThreadScrollTextLayerId;
    pthread_t pThreadHD0LayerId;
    s32Ret = Display_Init();
    if(HI_SUCCESS != s32Ret)
    {
        SAMPLE_TRACE("<line:%d>error: init display s32Ret:%#x\n", __LINE__, s32Ret);
        HI_GO_Deinit();
        return 0;
    }
    s32Ret = pthread_create(&pThreadHD0LayerId, HI_NULL,(HI_VOID *)*PTHREAD_DISP_HD0_Layer,NULL);
    if(HI_SUCCESS != s32Ret){
        HI_PRINT("=================================================================\n");
        HI_PRINT("thread create failed\n");
        HI_PRINT("=================================================================\n");
        return HI_FAILURE;
    }
    sleep(10);
#if 1
    s32Ret = pthread_create(&pThreadScrollTextLayerId, HI_NULL,(HI_VOID *)*PTHREAD_DISP_ScrollText,NULL);
    if(HI_SUCCESS != s32Ret){
        HI_PRINT("=================================================================\n");
        HI_PRINT("thread create failed\n");
        HI_PRINT("=================================================================\n");
        return HI_FAILURE;
    }
    sleep(5);
#endif
    while(1){
        sleep(5);
    }
    
    return HI_SUCCESS;
    
}

/*****************************************************************************
* func            : jpeg_dec
* description     : jpeg decode
* param[in]       : pszFileName
* param[in]       : pSurface
* retval          : HI_SUCCESS
                    HI_FAILURE
*****************************************************************************/
static HI_S32 jpeg_dec(HI_CHAR *pszFileName, HI_HANDLE *pSurface)
{
    HI_S32 ret         = HI_SUCCESS;
    HI_HANDLE hDecoder = 0;
    HIGO_DEC_ATTR_S stSrcDesc;
    HIGO_DEC_IMGINFO_S stImgInfo;
    HIGO_DEC_IMGATTR_S stAttr;

    stSrcDesc.SrcType = HIGO_DEC_SRCTYPE_FILE;
    stSrcDesc.SrcInfo.pFileName = pszFileName;

    ret = HI_GO_CreateDecoder(&stSrcDesc, &hDecoder);
    if(HI_SUCCESS != ret){
        return ret;
    }

    ret = HI_GO_DecImgInfo(hDecoder, 0, &stImgInfo);
    if(HI_SUCCESS != ret){
        HI_GO_DestroyDecoder(hDecoder);
	    return ret;
    }

    if(stImgInfo.Width > 4095){
	   stAttr.Width = 4095;
    }else{
       stAttr.Width = stImgInfo.Width;
    }

    if(stImgInfo.Height > 4095){
	    stAttr.Height = 4095;
    }else{
	    stAttr.Height = stImgInfo.Height;
    }

    stAttr.Format = HIGO_PF_8888;

    if(stImgInfo.Width > 4095 || stImgInfo.Height > 4095){
	    ret = HI_GO_DecImgData(hDecoder, 0, &stAttr, pSurface);
    }else{	
    	ret  = HI_GO_DecImgData(hDecoder, 0, NULL, pSurface);
    }
    ret |= HI_GO_DestroyDecoder(hDecoder);
    
    return ret;
    
}
