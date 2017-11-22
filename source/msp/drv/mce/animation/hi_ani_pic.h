/*********************************************************************************
*
*  Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
*  This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
*  (Hisilicon), and may not be copied, reproduced, modified, disclosed to
*  others, published or used, in whole or in part, without the express prior
*  written permission of Hisilicon.
*
***********************************************************************************
  File Name	: hi_ani_pic.h
  Version		: Initial Draft
  Author		: Hisilicon multimedia software group
  Created		: 2009/12/21
  Description	:
  History		:
  1.Date		: 2009/12/21
    Author		: 
    Modification	: Created file

*******************************************************************************/
#ifndef __HI_ANI_PIC_H__
#define __HI_ANI_PIC_H__

#include "hi_type.h"
#include "hi_go.h"
#include "hi_drv_mmz.h"
#include "hi_drv_mce.h"

#define ANI_ERROR(Errno,fmt,args... )\
 do { \
            HI_ERR_MCE("+++++++	ret 0x%x: "fmt,  Errno,##args);\
    } while (0)
    
/*play attribute*//* CNcomment: ��������*/
typedef enum
{
    HI_ANI_PIC_PLAYMODE_LOOP,  			/*time after time*//* CNcomment: ѭ������*/
    HI_ANI_PIC_PLAYMODE_ONE,   			/*once*//* CNcomment: ѭ��һ��*/
    HI_ANI_PIC_PLAYMODE_BYTIME,			/*loop by time*//* CNcomment: ��ʱ����Ϊ���ſ���ģʽ*/
    HI_ANI_PIC_PLAYMODE_BUTT
}HI_ANI_PIC_PLAYMODE;

/*common attribute of player*//* CNcomment: �������Ĺ�������*/
typedef struct
{
    HI_U32 		   uCount;         			/*number of frame that need play*//* CNcomment: ���ŵ�֡��*/
    HI_U32                     u32Xpos;        			/*X coordinate of palette*//* CNcomment: ��ʾ�����ϵ�X����*/
    HI_U32                    u32Ypos;        		 	/*Y coordinate of palette*//* CNcomment: ��ʾ�����ϵ�Y����*/
    HI_U32                    u32Width;        			/*width coordinate of palette*//* CNcomment:  ��ʾ�����ϵ�width */
    HI_U32                  	  u32Height;        			/*height coordinate of palette*//* CNcomment: ��ʾ�����ϵ�height */
    HI_U32                     BGColor;         			/*background of player*//* CNcomment:  �������ı���ɫ*/ 
    HI_ANI_PIC_PLAYMODE     PlayMode;   		/*the mode for cartoon*//* CNcomment: ��������ģʽ*/
    HI_U32 		   uLoopCount; 			/*times for play that only effect the loop mode*//* CNcomment: ��Ҫѭ���Ĵ�����ֻ��LOOPģʽ��Ч��-1��ʾֱ���û�����stopΪֹ*/
    HI_U32 		   uPlaySecond;			/*time for paly that only effect the HI_ANI_PIC_PLAYMODE_BYTIME mode*//* CNcomment:  ���Ŷ������ӣ�ֻ��HI_ANI_PIC_PLAYMODE_BYTIMEģʽ��Ч*/
}HI_ANI_PIC_COMMINFO;


/*the attribute of signal picture*//* CNcomment:  ����ͼƬ������*/
typedef struct
{
	HI_U32          u32FileAddr;	/*the physics address of picture*//* CNcomment:  ͼƬ���������ַ*/
	HI_U32          u32FileLen;	/*length of picture*//* CNcomment:  ͼƬ����*/
	HI_U32          u32Xpos;    	/*X position to show*//* CNcomment:  ��ʾλ��*/
	HI_U32          u32Ypos;	     	/*Y position to show*//* CNcomment:  ��ʾλ��*/
	HI_U32          u32Width;		/*Width of show*//* CNcomment: ��ʾ��С*/
	HI_U32          u32Height; 	/*Height of show*//* CNcomment:  ��ʾ��С*/
	HI_U32          u32DelayTime;	/*distance for play*//* CNcomment:  ͼƬ���ż��*/
}HI_ANI_PIC_PICTURE;

/*init the higo and some variable*//* CNcomment:��ʼ��higo�Լ�һЩ����*/
HI_S32 HI_ANI_PIC_Init(MMZ_BUFFER_S *pBuffer,HI_MCE_PALY_INFO_S *pPlayInfo,HI_U32 u32LayerID);

/*undefine the higo and reset some variable*//* CNcomment:ȥ��ʼ��higo�Լ���λһЩ����*/
HI_S32 HI_ANI_PIC_Deinit(HI_VOID);

/*read the common information from flash*//* CNcomment:��flash��ȡ�����Ĺ�����Ϣ*/
HI_S32 HI_ANI_PIC_GetCommInfo(HI_U32 uLayerID , HI_ANI_PIC_COMMINFO *pComInfo);

/*read the data information from flash *//* CNcomment:��flash��ȡ������������Ϣ*/
HI_S32 HI_ANI_PIC_GetPicInfo(HI_U32 uLayerID,HI_ANI_PIC_PICTURE *pPicInfo);

/*play logo*//* CNcomment:�������ſ�������( ���������߳�)*/
HI_S32 HI_ANI_PIC_Start(HI_U32 uLayerID, HI_ANI_PIC_COMMINFO *pComInfo, HI_ANI_PIC_PICTURE *pPicInfo);

/*stop logo*//* CNcomment:ֹͣ���ſ�������*/
HI_S32 HI_ANI_PIC_Stop(HI_U32 uLayerID);

/* get  stop ani state flag */
HI_S32 HI_ANI_PIC_IsFinish(HI_U32 uLayerID, HI_BOOL *bFinished);

/* set  extern stop ani flag */
HI_S32 HI_ANI_PIC_SetExtStopAni(HI_VOID);

/* get animotion loop play count*/
HI_U32 HI_ANI_PIC_GetPlayCount(HI_VOID);

#endif
