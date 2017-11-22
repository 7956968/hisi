/******************************************************************************

   Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
   File Name     : hi_unf_disp.h
   Version       : Initial Draft
   Author        : Hisilicon multimedia software group
   Created       : 2009/12/15
   Description   :
   History       :
   1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

 *******************************************************************************/


/**
 * \file
 **\brief : define DISPLAY module information
 * \brief supply infor about display.
 */

#ifndef  __HI_UNF_DISP_H__
#define  __HI_UNF_DISP_H__

#include "hi_unf_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*************************** Structure Definition ****************************/
/** \addtogroup      DISPLAY */
/** @{ */  /** <!-- [DISPLAY] */

/**max DAC count*/
/**CNcomment:����DAC��Ŀ*/
#define MAX_DAC_NUM (4)

/**enum define about DISPLAY channel*/
/**CNcomment:����DISPLAYͨ��ö��*/
typedef enum hiUNF_DISP_E
{
    HI_UNF_DISP_SD0 = 0x0,  /**< SD DISPLAY0 */ /**<CNcomment:����DISPLAY0 */
    HI_UNF_DISP_HD0,         /**< HD DISPLAY0 *//**<CNcomment: ����DISPLAY0 */
    HI_UNF_DISP_BUTT
}HI_UNF_DISP_E;

/**define the sigal type of DAC output*/
/**CNcomment:����֧�ֵ�DAC�źŸ�ʽ*/
typedef enum hiUNF_DISP_DAC_MODE_E
{
    HI_UNF_DISP_DAC_MODE_SILENCE = 0,    /**<No signal */ /**<CNcomment:�����*/
    HI_UNF_DISP_DAC_MODE_CVBS,           /**<CVBS output */ /**<CNcomment:CVBS���*/
    HI_UNF_DISP_DAC_MODE_Y,              /**<Y component signal output*//**<CNcomment:Y���ȷ������*/
    HI_UNF_DISP_DAC_MODE_PB,             /**<PB component signal output*//**<CNcomment:��ɫɫ��������*/
    HI_UNF_DISP_DAC_MODE_PR,             /**<PR componet signal output *//**<CNcomment:��ɫɫ��������*/
    HI_UNF_DISP_DAC_MODE_R,              /**< Red component signal output*//**<CNcomment:CNcomment:��ɫ�������*/
    HI_UNF_DISP_DAC_MODE_G,              /**<green component signal output*//**<CNcomment:��ɫ�������*/
    HI_UNF_DISP_DAC_MODE_B,              /**<blue component signal output*//**<CNcomment:��ɫ�������*/
    HI_UNF_DISP_DAC_MODE_SVIDEO_Y,       /**<SVIDEO Y signal output*//**<CNcomment:SVIDEO_Y�������*/
    HI_UNF_DISP_DAC_MODE_SVIDEO_C,       /**<SVIDEO C signal output *//**<CNcomment:SVIDEO_C�������*/
    HI_UNF_DISP_DAC_MODE_HD_Y,           /**<HD Y component signal output*//**<CNcomment:����Y���ȷ������*/
    HI_UNF_DISP_DAC_MODE_HD_PB,          /**<HD PB component signal output*//**<CNcomment:������ɫɫ��������*/
    HI_UNF_DISP_DAC_MODE_HD_PR,          /**< HD PR component signal output *//**<CNcomment:�����ɫɫ��������*/
    HI_UNF_DISP_DAC_MODE_HD_R,           /**< HD red component signal output*/ /**<CNcomment:�����ɫ�������*/
    HI_UNF_DISP_DAC_MODE_HD_G,           /**< HD green component signal output*//**<CNcomment:������ɫ�������*/
    HI_UNF_DISP_DAC_MODE_HD_B,           /**< HD blue component signal output*//**<CNcomment:������ɫ�������*/
    HI_UNF_DISP_DAC_MODE_BUTT
}HI_UNF_DISP_DAC_MODE_E;


/**define the interface type of display output*/
/**CNcomment:����֧�ֵ���ʾ����ӿ� */
typedef struct  hiUNF_DISP_INTERFACE_S
{
    HI_BOOL                      bScartEnable;                /**<flag of enable scart output*//**<CNcomment:�Ƿ����Scart�ź�*/
    HI_BOOL                      bBt1120Enable;               /**<flag of enable Bt1120 output*//**<CNcomment:�Ƿ����Bt1120�����ź�*/
    HI_BOOL                      bBt656Enable;                /**<flag of enable Bt656 output*//**<CNcomment:�Ƿ����Bt656�����ź�*/
    HI_UNF_DISP_DAC_MODE_E       enDacMode[MAX_DAC_NUM];      /**<the DAC output signal type of the defined interface type *//**<CNcomment:����DAC������ź�*/
}HI_UNF_DISP_INTERFACE_S ;


/**define LCD output data width*/
/**CNcomment:����LCD���������λ��*/
typedef enum hiUNF_DISP_LCD_DATA_WIDTH_E
{
    HI_UNF_DISP_LCD_DATA_WIDTH8 = 0,       /**<8 bits*//**<CNcomment:8λ*/
    HI_UNF_DISP_LCD_DATA_WIDTH16,          /**<16 bits*//**<CNcomment:16λ*/
    HI_UNF_DISP_LCD_DATA_WIDTH24,          /**<24 bits*//**<CNcomment:24λ*/
    HI_UNF_DISP_LCD_DATA_WIDTH_BUTT
}HI_UNF_DISP_LCD_DATA_WIDTH_E;

/**define the LCD data format*/
/**CNcomment:����LCD�����ݸ�ʽ*/
typedef enum hiHI_UNF_DISP_LCD_DATA_FMT
{
    HI_UNF_DISP_LCD_DATA_FMT_YUV422 = 0,         /**<YUV422��data width is 16*//**<CNcomment:YUV422��λ��Ϊ16*/
    HI_UNF_DISP_LCD_DATA_FMT_RGB565 = 0x8,       /**<RGB565��data width is 16*//**<CNcomment:RGB565��λ��Ϊ16*/
    HI_UNF_DISP_LCD_DATA_FMT_RGB444 = 0xa,       /**<RGB444��data width is 16*//**<CNcomment:RGB444��λ��Ϊ16*/
    HI_UNF_DISP_LCD_DATA_FMT_RGB666 = 0xb,       /**<RGB666��data width is 24*//**<CNcomment:RGB666��λ��Ϊ24*/
    HI_UNF_DISP_LCD_DATA_FMT_RGB888 = 0xc,       /**<RGB888��data widht is 24*//**<CNcomment:RGB888��λ��Ϊ24*/
    HI_UNF_DISP_LCD_DATA_FMT_BUTT
}HI_UNF_DISP_LCD_DATA_FMT_E;

/**define LCD timing */
/**CNcomment:����LCD��ʱ�����*/
typedef struct hiUNF_DISP_LCD_PARA_S
{
    HI_U32                    VFB;                 /**<vertical front blank*//**<CNcomment:��ֱǰ����*/
    HI_U32                    VBB;                 /**<vertical back blank*//**<CNcomment:��ֱ������*/
    HI_U32                    VACT;                /**<vertical active area*//**<CNcomment:��ֱ��Ч��*/
    HI_U32                    HFB;                 /**<horizonal front blank*//**<CNcomment:ˮƽǰ����*/
    HI_U32                    HBB;                 /**<horizonal back blank*//**<CNcomment:ˮƽ������*/
    HI_U32                    HACT;                /**<horizonal active area*/ /**<CNcomment:ˮƽ��Ч��*/
    HI_U32                    VPW;                 /**<vertical sync pluse width*//**<CNcomment:��ֱ������*/
    HI_U32                    HPW;                 /**<horizonal sync pluse width*/ /**<CNcomment:ˮƽ������*/
    HI_BOOL                   IDV;                 /**< flag of data valid signal is needed flip*//**<CNcomment:��Ч�����ź��Ƿ�ת*/
    HI_BOOL                   IHS;                 /**<flag of horizonal sync pluse is needed flip*//**<CNcomment:ˮƽͬ�������ź��Ƿ�ת*/
    HI_BOOL                   IVS;                 /**<flag of vertical sync pluse is needed flip*//**<CNcomment:��ֱͬ�������ź��Ƿ�ת*/
    HI_BOOL                   ClockReversal;       /**<flag of clock is needed flip*//**<CNcomment:ʱ���Ƿ�ת*/
    HI_UNF_DISP_LCD_DATA_WIDTH_E   DataWidth;      /**<data width*/ /**<CNcomment:����λ��*/
    HI_UNF_DISP_LCD_DATA_FMT_E       ItfFormat;      /**<data format.*//**<CNcomment:���ݸ�ʽ.*/
    HI_BOOL                   DitherEnable;        /**< flag of is enable Dither*//**<CNcomment:���ݸ�ʽ.*/
    HI_U32                    ClkPara0;            /**<PLL  register SC_VPLL1FREQCTRL0  value *//**<CNcomment:PLL  SC_VPLL1FREQCTRL0  �Ĵ���*/
    HI_U32                    ClkPara1;            /**<PLL  register SC_VPLL1FREQCTRL1 value*//**<CNcomment:PLL   SC_VPLL1FREQCTRL1�Ĵ���*/
    HI_U32                    InRectWidth;         /**< display rect width  */ /**< CNcomment:��ʾ����� */
    HI_U32                    InRectHeight;        /**< display rect height  */ /**< CNcomment:��ʾ�����  */
} HI_UNF_DISP_LCD_PARA_S;

/**define the layer which can attach to different display channel*/
/**CNcomment:��������ɰ󶨵ĵ��Ӳ�*/
typedef enum hiUNF_DISP_FREE_LAYER_E
{
    HI_UNF_DISP_FREE_LAYER_OSD_0,         /**<0sd layer0 which can be attach*/    /**<CNcomment:�����ɰ�ͼ�β�0*/

    HI_UNF_DISP_FREE_LAYER_BUTT
}HI_UNF_DISP_FREE_LAYER_E;


/**define the layer which can be overlay on the display channel*/
/**CNcomment:����DISPLAYͨ���ϵĵ��Ӳ�*/
typedef enum hiUNF_DISP_LAYER_E
{
    HI_UNF_DISP_LAYER_VIDEO_0 = 0,   /**<video layer 0*//**<CNcomment:��Ƶ��0*/
    HI_UNF_DISP_LAYER_OSD_0,         /**<osd layer 0*//**<CNcomment:ͼ�β�0*/
    HI_UNF_DISP_LAYER_ATTACH_OSD_0,  /**< attached osd layer 0*//**<CNcomment:��ͼ�β�0*/

    HI_UNF_DISP_LAYER_BUTT
}HI_UNF_DISP_LAYER_E;


/**define the struct about color */
/**CNcomment:������ʾ��ɫ�Ľṹ�� */
typedef struct  hiUNF_DISP_BG_COLOR_S
{
    HI_U8 u8Red ;            /**<red */ /**<CNcomment:��ɫ����*/
    HI_U8 u8Green ;          /**<green*//**<CNcomment:��ɫ����*/
    HI_U8 u8Blue ;           /**<blue*//**<CNcomment:��ɫ����*/
} HI_UNF_DISP_BG_COLOR_S;

/********************************ENCODER STRUCT********************************/
/** define the enum of Macrovision output type*/
/** CNcomment:��ʾ���Macrovisionģʽö�ٶ���*/
typedef enum hiUNF_DISP_MACROVISION_MODE_E
{
    HI_UNF_DISP_MACROVISION_MODE_TYPE0,  /**<type 0 *//**<CNcomment:��������0 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE1,  /**<type 1 *//**<CNcomment:��������1 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE2,  /**<type 2  *//**<CNcomment:��������2 */
    HI_UNF_DISP_MACROVISION_MODE_TYPE3,  /**<type 3  *//**<CNcomment:��������3 */
    HI_UNF_DISP_MACROVISION_MODE_CUSTOM, /**<type of configure by user  *//**<CNcomment:�û��Զ������� */
    HI_UNF_DISP_MACROVISION_MODE_BUTT
} HI_UNF_DISP_MACROVISION_MODE_E;

/** define teletex data info structure*/
/** CNcomment:����ͼ����Ϣ���ݽṹ*/
typedef struct hiUNF_DISP_TTX_DATA_S
{
    HI_U8   *pu8DataAddr;               /**<Ttx data buffer virtual address*/ /**<CNcomment:Ttx�����û������ַ*/
    HI_U32   u32DataLen;                /**<Ttx data lenght*//**<CNcomment:Ttx���ݳ���*/
} HI_UNF_DISP_TTX_DATA_S;


/** CGMS type select */
/**CNcomment:CGMS ����ѡ��*/
typedef enum hiUNF_DISP_CGMS_TYPE_E
{
    HI_UNF_DISP_CGMS_TYPE_A = 0x00,
    HI_UNF_DISP_CGMS_TYPE_B,

    HI_UNF_DISP_CGMS_TYPE_BUTT
}HI_UNF_DISP_CGMS_TYPE_E;


/** definition of CGMS mode */
/**CNcomment:���忽�����ι���ϵͳ(CGMS) ģʽ*/
typedef enum hiUNF_DISP_CGMS_MODE_E
{
   HI_UNF_DISP_CGMS_MODE_COPY_FREELY    = 0,    /* copying is permitted without restriction *//**<CNcomment:����û�����ƵĿ���*/
   HI_UNF_DISP_CGMS_MODE_COPY_NO_MORE   = 0x01, /* No more copies are allowed (one generation copy has been made) *//**<CNcomment:һ���������ٱ�����*/
   HI_UNF_DISP_CGMS_MODE_COPY_ONCE      = 0x02, /* One generation of copies may be made *//**<CNcomment:һ����������*/
   HI_UNF_DISP_CGMS_MODE_COPY_NEVER     = 0x03, /* No copying is permitted *//**<CNcomment:��������*/
      
   HI_UNF_DISP_CGMS_MODE_BUTT
}HI_UNF_DISP_CGMS_MODE_E;


/** definition of CGMS configuration */
typedef struct hiUNF_DISP_CGMS_CFG_S
{
    HI_BOOL                  bEnable;    /** HI_TRUE:CGMS is enabled; HI_FALSE:CGMS is disabled *//**<CNcomment:HI_TRUE:��ʾCGMSʹ�ܣ�HI_FALSE��ʾCGMS�ر�*/
    HI_UNF_DISP_CGMS_TYPE_E  enType;     /** type-A or type-B or None(BUTT) *//**<CNcomment:����A��������B���߿�*/
    HI_UNF_DISP_CGMS_MODE_E  enMode;     /** CGMS mode. *//**<CNcomment:CGMSģʽ*/
     
}HI_UNF_DISP_CGMS_CFG_S;


/** define VBI information structure*/
/**CNcomment: ����VBI��Ϣ���ݽṹ*/
typedef struct hiUNF_DISP_VBI_DATA_S
{
    HI_U8  *pu8DataAddr;                 /**<Vbi data buffer virtual address*/ /**<CNcomment:Vbi�����û������ַ*/
    HI_U32  u32DataLen;                  /**<Vbi data lenght*/  /**<CNcomment:Vbi���ݳ���*/
} HI_UNF_DISP_VBI_DATA_S;


/**define WSS information structure*/
/**CNcomment:����ͼ����Ϣ���ݽṹ */
typedef struct hiUNF_DISP_WSS_DATA_S
{
    HI_BOOL bEnable;                /**<WSS configure enable  HI_TRUE: enable, HI_FALSE: disnable*/  /**<CNcomment:WSS����ʹ�ܡ�HI_TRUE��ʹ�ܣ�HI_FALSE����ֹ*/
    HI_U16  u16Data;                /**<Wss data */ /**<CNcomment:Wss����*/
} HI_UNF_DISP_WSS_DATA_S;

/**define display channel interface type */
/**CNcomment:����DISPLAYͨ���Ľӿ�����*/
typedef enum hiUNF_DISP_INTF_TYPE_E
{
    HI_UNF_DISP_INTF_TYPE_TV = 0,    /**<TV type*/ /**<CNcomment:����ӿ�ΪTV*/
    HI_UNF_DISP_INTF_TYPE_LCD,       /**<LCD type*/ /**<CNcomment:����ӿ�ΪLCD*/
    HI_UNF_DISP_INTF_TYPE_BUTT
}HI_UNF_DISP_INTF_TYPE_E;

/**define display output range struct */
/**CNcomment:����DISPLAY�����Χ����*/
typedef struct hiUNF_RECT_S
{
    HI_S32 x, y;

    HI_S32 w, h;
} HI_UNF_RECT_S;

/**define display color temperature control struct */
/**CNcomment:����DISPLAYɫ�¿��ƽṹ*/
typedef struct hiUNF_DISP_COLORTEMPERATURE_S
{
    HI_U32 u32Kr; /**< Red component*/ /**<CNcomment:��ɫ����*/
    HI_U32 u32Kg; /**< Green component*/ /**<CNcomment:��ɫ����*/
    HI_U32 u32Kb; /**< Blue component*/ /**<CNcomment:��ɫ����*/
} HI_UNF_DISP_COLORTEMPERATURE_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */

/******************************* API declaration *****************************/
/** \addtogroup      DISPLAY */
/** @{ */  /** <!-- [DISPLAY] */

/**
\brief Initiallization DISP module. CNcomment:��ʼ��DISPģ�� CNend
\attention \n
Please call this API function, before call anyother API of DISP module. 
CNcomment:����DISPģ�������ӿ�ǰҪ�����ȵ��ñ��ӿ� CNend
\param  none. CNcomment:�� CNend
\retval ::HI_SUCCESS  operation success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NOT_EXIST DISP device not exist.CNcomment:�豸������ CNend
\retval ::HI_ERR_DISP_NOT_DEV_FILE  DISP  not device file .CNcomment:���豸 CNend
\retval ::HI_ERR_DISP_DEV_OPEN_ERR  DISP  open fail.CNcomment:��ʧ�� CNend
\see \n
none. CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_Init(HI_VOID);

/**
\brief deinital. CNcomment:ȥ��ʼ��DISPģ�� CNend
\attention \n
please call API HI_UNF_DISP_Close and open all the DISP device, before call this API.
CNcomment:�ڵ���::HI_UNF_DISP_Close�ӿڹر����д򿪵�DISP����ñ��ӿ� CNend
\param none. CNcomment:�� CNend
\retval ::HI_SUCCESS success.CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_CLOSE_ERR  DISP close fail.CNcomment:�ر�ʧ�� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_DeInit(HI_VOID);

/**
\brief Attach one DISP channel to anoter.CNcomment:������DISPͨ���� CNend
\attention \n
Please finish the attach operation before the DISP channel has been open, and currently we only support HD channel attach to SD channel.
CNcomment:Ŀǰ֧�ֽ�����DISP�󶨵�����DISPͨ���ϣ����ڴ�DISPͨ��ǰ������ɰ󶨲����� CNend
\param[in] enDstDisp   Destination DISP channel  CNcomment:Ŀ��DISPͨ���� CNend
\param[in] enSrcDisp   source DISP channel.CNcomment:  ԴDISPͨ���� CNend
\retval ::HI_SUCCESS  operation success.CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT  display not be initialization. CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter. CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT   invalid opeation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_Attach(HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp);

/**
\brief dettach DISP.CNcomment:������DISPͨ����� CNend
\attention \n
should  close the DISP channels, before do detach operation.
CNcomment:ֻ���ڹر�����DISPͨ������ܽ��н�󶨲����� CNend
\param[in] enDstDisp   Destination DISP channel.  CNcomment:Ŀ��DISPͨ���� CNend
\param[in] enSrcDisp    source DISP channel.CNcomment: ԴDISPͨ���� CNend
\retval ::HI_SUCCESS  success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT   DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT   invalid operation.CNcomment:�����Ƿ� CNend
\see \n
none. CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_Detach(HI_UNF_DISP_E enDstDisp, HI_UNF_DISP_E enSrcDisp);

/**
\brief open DISP channel. CNcomment:��ָ��DISPͨ�� CNend
\attention \n
Please config the valid parameters before open DISP. 
CNcomment:�ڴ�DISP֮ǰ������ɶ���Ĳ������ã����⻭����˸ CNend
\param[in] enDisp   DISP channel ID, please reference the define of HI_UNF_DISP_E.CNcomment:DISPͨ���ţ���μ�::HI_UNF_DISP_E CNend
\retval ::HI_SUCCESS success.CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT   DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_CREATE_ERR    DISP create fail.CNcomment:DISP����ʧ�� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_Open(HI_UNF_DISP_E enDisp);

/**
\brief  coase DISP channel. CNcomment:�ر�ָ��DISP CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp DISP channel ID. CNcomment:DISPͨ���� CNend
\retval ::HI_SUCCESS success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT DISP uninitialization.  CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA invalid input parameter. CNcomment:��������Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_Close(HI_UNF_DISP_E enDisp);

/**
\brief  attach OSD layer to DISP. CNcomment:��OSD�󶨵�DISP�� CNend
\attention \n
please finish attach operation before DISP channel be open.
CNcomment:������DISP��֮ǰ��ɰ󶨡� CNend
\param[in] enDisp      DISP channel ID. CNcomment:DISPͨ���� CNend
\param[in] enLayer    the osd layer needed to be attach. CNcomment:�ɰ󶨵�ͼ�β� CNend
\retval ::HI_SUCCESS  success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT   DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR      CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA invalid input parameter.CNcomment: ��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT   invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none. CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_AttachOsd(HI_UNF_DISP_E enDisp, HI_UNF_DISP_FREE_LAYER_E enLayer);

/**
\brief CNcomment:detach OSD layer from DISP. CNcomment:��OSD��DISP��� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp      DISP channel ID. CNcomment:DISPͨ���� CNend
\param[in] enLayer     OSD layer needed detach. CNcomment:�ɰ󶨵�ͼ�β� CNend
\retval ::HI_SUCCESS  success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization. CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR      Input pointer is NULL. CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT  invalid operation.  CNcomment:�����Ƿ� CNend
\see \n
none. CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_DetachOsd(HI_UNF_DISP_E enDisp, HI_UNF_DISP_FREE_LAYER_E enLayer);

/**
\brief  set DISP interface type. CNcomment:����DISP�Ľӿ����� CNend
\attention \n
Interface type  setting is the first paramter, needed by DISP configuration. 
CNcomment:�ӿ�������DISPӦ�����õĵ�һ�������� CNend
\param[in] enDisp      DISP channel ID. CNcomment:DISPͨ���� CNend
\param[in] enIntfType  DISP interface type.  CNcomment:DISP�Ľӿ����� CNend
\retval ::HI_SUCCESS   success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT   DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR      Input pointer is NULL. CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter. CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT   invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetIntfType(HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_TYPE_E enIntfType);

/**
\brief get DISP interface type. CNcomment:��ȡDISP�Ľӿ����� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp        DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] penIntfType  the pointer of DISP type. CNcomment:DISP�Ľӿ�����ָ�� CNend
\retval ::HI_SUCCESS   success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT   DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR        Input pointer is NULL. CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA  invalid input parameter. CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT   invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetIntfType(HI_UNF_DISP_E enDisp, HI_UNF_DISP_INTF_TYPE_E *penIntfType);

/**
\brief set DISP output format.CNcomment:����DISP����ʽ CNend
\attention \n
This API is only useful for TV interface type source , for LCD type this API has no effect. \n
for HD DISP channel please set HD display format, and for SD DISP channel please set SD display format.\n
for scenario of HD,SD use same source, only support format which frame rate is 50Hz or 60Hz.\n
CNcomment:ֻ�е�����DISPΪTV�󣬵��ñ��ӿڲ������塣�Ը����DISP��ֻ�����ø������ʽ���Ա����DISP��ֻ�����ñ������ʽ����ͬԴ��ʾ�����£��ݲ�֧��ˢ���ʷ�50Hz/60Hz����ʽ���磺�ݲ�֧��1080P24/1080P25/1080P30 CNend
\param[in] enDisp               DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] enEncodingFormat    DISP format. CNcomment:DISP����ʽ CNend
\retval ::HI_SUCCESS  success. CNcomment:�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none. CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetFormat(HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E enEncodingFormat);

/**
\brief get DISP format. CNcomment:��ȡDISP����ʽ CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp               DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] penEncodingFormat    poiner of DISP format.DCNcomment:ISP����ʽָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetFormat(HI_UNF_DISP_E enDisp, HI_UNF_ENC_FMT_E *penEncodingFormat);

/**
\brief  set DAC mode.CNcomment:����DACģʽ CNend
\attention \n
Please config DISP paramter first, then call this API set DAC mode. \n
There are 6 DAC provided by SOC. suggest, please use the typcal DAC configuration.  DAC 0/1/2 used by HD channel, DAC3/4/5 used by SD channel.
CNcomment:DAC��������DISP������Դ�������DISP�Ĳ������ò��򿪺��ٵ��ñ��ӿ����DAC�����á�����6��DAC���Ƽ�0��1��2�������������3��4��5�������������CNend
\param[in] pstDacMode   pointer of Dac mode. CNcomment:ָ�����ͣ�DACģʽ����μ�::HI_UNF_OUTPUT_INTERFACE_S CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetDacMode(const HI_UNF_DISP_INTERFACE_S *pstDacMode);

/**
\brief  Get DAC mode. CNcomment:��ȡDACģʽ CNend
\attention \n
none. CNcomment:�� CNend
\param[out] pstDacMode    pointer of Dac mode.CNcomment:ָ�����ͣ�DACģʽ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetDacMode(HI_UNF_DISP_INTERFACE_S *pstDacMode);

/**
\brief Set LCD parameter. CNcomment:����DISP��LCD���� CNend
\attention \n
This API function is only useful for DISP interface is LCD, but is no effect for TV type.
CNcomment:ֻ�е�����DISPΪLCD�󣬵��ñ��ӿڲ������塣 CNend
\param[in] enDisp         DISP channel ID. CNcomment:DISPͨ���� CNend
\param[in] pstLcdPara     pointer of LCD paramter. CNcomment:ָ�����ͣ�LCD��������μ�::HI_UNF_DISP_LCD_PARA_S CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetLcdPara(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_LCD_PARA_S *pstLcdPara);

/**
\brief  get LCD parameter which had been config. CNcomment:��ȡDISP��LCD���� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp          DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstLcdPara      pointer of LCD paramter. CNcomment:ָ�����ͣ�LCD���� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetLcdPara(HI_UNF_DISP_E enDisp, HI_UNF_DISP_LCD_PARA_S *pstLcdPara);

/**
\brief   set DISP layers Z order.CNcomment:����DISP�ϵ��Ӳ��Z�� CNend
\attention \n
CNcomment:�� CNend
\param[in] enDisp          DISP channel ID.CNcomment: DISPͨ���� CNend
\param[in] enLayer        layer ID which needed overlay to the DISP channel.CNcomment: DISP�ϵĵ��Ӳ� CNend
\param[in] enZFlag        the way of overlay. CNcomment:Z����ڷ�ʽ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetLayerZorder(HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_LAYER_ZORDER_E enZFlag);

/**
\brief  get DISP overlay z order.CNcomment:��ȡDISP�ϵ��Ӳ��Z�� CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
CNcomment:�ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP�� CNend
\param[in] enDisp            DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] enLayer           CNcomment:DISP�ϵĵ��Ӳ� CNend
\param[in] pu32Zorder        CNcomment:���Ӳ�Z����ֵ����ֵԽ�����ȼ�Խ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetLayerZorder(HI_UNF_DISP_E enDisp, HI_UNF_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder);

/**
\brief set DISP back ground color.CNcomment:����DISP����ɫ CNend
\attention \n
none.  CNcomment:�� CNend
\param[in] enDisp       CNcomment:DISPͨ���� CNend
\param[in] pstBgColor   CNcomment:ָ�����ͣ������õ���ʾ�������ɫ����μ�::HI_UNF_BG_COLOR_S CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetBgColor(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_BG_COLOR_S *pstBgColor);

/**
\brief  set DISP back ground color.CNcomment:��ȡDISP����ɫ CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
CNcomment:�ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP�� CNend
\param[in] enDisp          DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] pstBgColor   pointer of back ground color.CNcomment: ָ�����ͣ���ʾ�������ɫ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetBgColor(HI_UNF_DISP_E enDisp, HI_UNF_DISP_BG_COLOR_S *pstBgColor);

/**
\brief  set DISP brightness.CNcomment:����DISP���� CNend
\attention \n
if the value seted more than 100, we clip it to 100.CNcomment:����100��ֵ��100���� CNend
\param[in] enDisp            DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] u32Brightness   brightness value. the range is 0~100, 0 means the min brightness value.
CNcomment:�����õ���ʾ�������ֵ��ȡֵ��ΧΪ0��100��0����С���ȣ�100��������� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetBrightness(HI_UNF_DISP_E enDisp, HI_U32 u32Brightness);

/**
\brief   get the DISP brightness.CNcomment:��ȡDISP���� CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.
CNcomment:��ѯ��Ĭ������ֵΪ50���ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP��CNend
\param[in] enDisp              DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] pu32Brightness    pointer of brightness. CNcomment:ָ�����ͣ���ʾ�������ֵ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetBrightness(HI_UNF_DISP_E enDisp, HI_U32 *pu32Brightness);

/**
\brief  set DISP contrast value.CNcomment:����DISP�Աȶ� CNend
\attention \n
If the value is more than 100, we clip it to 100.
CNcomment:����100��ֵ��100���� CNend
\param[in] enDisp          DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] u32Contrast    contrast value. the range is 0~100, 0 means the min contrast value. \n
CNcomment:�����õ���ʾ����Աȶ�ֵ��ȡֵ��ΧΪ0��100��0����С�Աȶȣ�100�����Աȶ� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetContrast(HI_UNF_DISP_E enDisp, HI_U32 u32Contrast);

/**
\brief  get DISP contrast value.CNcomment:��ȡDISP�Աȶ� CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
CNcomment:��ѯ��Ĭ�϶Աȶ�ֵΪ50���ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP��CNend
\param[in] enDisp           DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] pu32Contrast   pointer of contrast. CNcomment:ָ�����ͣ���ʾ����Աȶ�ֵ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetContrast(HI_UNF_DISP_E enDisp, HI_U32 *pu32Contrast);

/**
\brief   set DISP saturation. CNcomment:����DISP���Ͷ� CNend
\attention \n
If the value is more than 100, we clip it to 100.
CNcomment:����100��ֵ��100���� CNend
\param[in] enDisp             DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] u32Saturation      saturation value. the range is 0~100, 0 means the min saturation value. \n
CNcomment:�����õ���ʾ������Ͷ�ֵ��ȡֵ��ΧΪ0��100��0����С���Ͷȣ�100����󱥺Ͷ� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetSaturation(HI_UNF_DISP_E enDisp, HI_U32 u32Saturation);

/**
\brief   get DISP saturation.CNcomment:��ȡDISP���Ͷ� CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
CNcomment:��ѯ��Ĭ�ϱ��Ͷ�ֵΪ50. �ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP��CNend 
\param[in] enDisp                 DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] pu32Saturation    pointer of saturation. CNcomment:ָ�����ͣ���ʾ������Ͷ�ֵ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetSaturation(HI_UNF_DISP_E enDisp, HI_U32 *pu32Saturation);

/**
\brief  set DISP hueplus. CNcomment:����DISPɫ�� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp              DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] u32HuePlus       hueplus value. the range is 0~100, 0 means the min hueplus value. \n
CNcomment:��ʾ���ɫ������ֵ����ΧΪ0��100��0����ʾ��Сɫ�����棻100����ʾ���ɫ������ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetHuePlus(HI_UNF_DISP_E enDisp, HI_U32 u32HuePlus);

/**
\brief get DISP hueplus. CNcomment:��ȡDISPɫ�� CNend
\attention \n
In the case of HD atach to SD. the Z order configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
CNcomment:��ѯ��Ĭ��ɫ��ֵΪ50���ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP��CNend
\param[in] enDisp          DISP channel ID.CNcomment: DISPͨ���� CNend
\param[out] pu32HuePlus     pointer of hueplus. CNcomment:ָ�����ͣ���ʾ���ɫ������ֵ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetHuePlus(HI_UNF_DISP_E enDisp, HI_U32 *pu32HuePlus);

/**
\brief  set DISP color temperature. CNcomment:����DISPɫ�� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp              DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] colortemp           temperature value. the range is 0~100, 0 means the min value. \n
CNcomment:��ʾ���ɫ������ֵ����ΧΪ0��100��0����ʾ��Сɫ�����棻100����ʾ���ɫ������ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR       Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetColorTemperature(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_COLORTEMPERATURE_S *colortemp);

/**
\brief get DISP hueplus. CNcomment:��ȡDISPɫ�� CNend
\attention \n
In the case of HD atach to SD. the configuration of HD will auto sync to SD, also the configuration of SD will auto sync to HD.\n
CNcomment:��ѯ��Ĭ��ɫ��ֵΪ50���ڸ߱��������£�����DISP�����û��Զ�ͬ��������DISP������DISP�����û��Զ�ͬ��������DISP��CNend
\param[in] enDisp          DISP channel ID.CNcomment: DISPͨ���� CNend
\param[out] colortemp      pointer of color temperature. CNcomment:ָ�����ͣ���ʾ���ɫ������ֵ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetColorTemperature(HI_UNF_DISP_E enDisp, HI_UNF_DISP_COLORTEMPERATURE_S *colortemp);


/**
\brief set auto contrast adjustment.CNcomment:����Ӧ�Աȶȵ��� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp           DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] bEnable         flag of enable or disnable.CNcomment:ʹ��/��ֹ��־ CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetAccEnable(HI_UNF_DISP_E enDisp, HI_BOOL bEnable);

/**
\brief Send teletex data pes.CNcomment:����ͼ������PES�� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp            DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstTtxData       pointer of ttxdata pes. CNcomment:ָ�����ͣ�ָ��ͼ�����ݰ���ָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SendTtxData(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_TTX_DATA_S *pstTtxData);

/**
\brief  send VBI data pes. CNcomment:����VBI��Vertical Blanking Interval������PES�� CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp             DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstVbiData        pointer of VBI data pes description structure.CNcomment:ָ�����ͣ�ָ��VBI���ݰ���ָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SendVbiData(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_VBI_DATA_S *pstVbiData);

/**
\brief  set Wss. CNcomment:ֱ������WSS��Wide Screen Singnalling������ CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp           DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstWssData        pointer of Wss data description structure.CNcomment:ָ�����ͣ�ָ��WSS���ݵ�ָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetWss(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_WSS_DATA_S *pstWssData);

/**
\brief  set Macrovision mode CNcomment:����Macrovisionģʽ CNend
\attention \n
none.CNcomment:�� CNend
\param[in] enDisp          DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] enMode        Macrovision mode.  CNcomment:Macrovisionģʽ����μ�::HI_UNF_MACROVISION_MODE_E CNend
\param[in] pData          pointer of Macrovision control data CNcomment: ָ�����ͣ��Զ����Macrovision�������� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetMacrovision(HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E enMode, const HI_VOID *pData);

/**
\brief get Macrovision mode. CNcomment: ��ȡMacrovisionģʽ CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp      DISP channel ID.CNcomment:DISPͨ���� CNend
\param[out] penMode    pointer of Macrovision mode. CNcomment:ָ�����ͣ�MACROVISIONģʽ CNend
\param[out] pData      a data pointer only valid whenpenMode=HI_MACROVISION_MODE_CUSTOM.\n
CNcomment:ָ�����ͣ�����penMode=HI_MACROVISION_MODE_CUSTOMʱ��Ч CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetMacrovision(HI_UNF_DISP_E enDisp, HI_UNF_DISP_MACROVISION_MODE_E *penMode, const HI_VOID *pData);


/**
\brief   set CGMS data. CNcomment:����CGMS��Copy Generation Management System������ CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp                   DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstCgmsCfg             pointer of CGMS configuration  CNcomment:ָ�����ͣ�ָ��CGMS�������ݵ�ָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetCgms(HI_UNF_DISP_E enDisp, const HI_UNF_DISP_CGMS_CFG_S *pstCgmsCfg);

/**
\brief set Disp output range. CNcomment:����Display������� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp                   DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] pstOutRect             pointer of display output range.CNcomment:ָ�����ͣ�ָ��Display��������������ݵ�ָ�� CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NO_INIT    DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
\retval ::HI_ERR_DISP_NULL_PTR          Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
\retval ::HI_ERR_DISP_INVALID_PARA   invalid input parameter.CNcomment:��������Ƿ� CNend
\retval ::HI_ERR_DISP_INVALID_OPT    invalid operation. CNcomment:�����Ƿ� CNend
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetOutputWin(HI_UNF_DISP_E enDisp, HI_UNF_RECT_S *pstOutRect);
/** @} */  /** <!-- ==== API declaration end ==== */


typedef enum hiUNF_MIRRIR_DATA_FORMAT_E
{
    HI_UNF_MIRROR_PACKAGE_UYVY = 0,                /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is UYVY.*/ /**<CNcomment: YUV�ռ������ʽΪpackage, �����Ų���ʽΪUYVY*/
    HI_UNF_MIRROR_PACKAGE_YUYV,                /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YUYV.*/ /**<CNcomment: YUV�ռ������ʽΪpackage,�����Ų���ʽΪYUYV*/
    HI_UNF_MIRROR_PACKAGE_YVYU,                /**<The YUV spatial sampling format is package, and the pixel arrangement sequence in the memory is YVYU.*/ /**<CNcomment: YUV�ռ������ʽΪpackage,�����Ų�YVYU*/
}HI_UNF_MIRRIR_DATA_FORMAT_E;


/**define the max buffer number allocated by user or driver.*/
#define HI_DISP_MIRROR_BUFFER_MAX_NUMBER 16

/** define the cfg infor when get cast frame.*/
/**CNcomment: ��ץ��ʱ������������Ϣ*/
typedef struct hiUNF_DISP_CAST_CFG_S
{   
    HI_U32 u32Width; /**< expected output width of frame.*//**<CNcomment:ϣ����ȡ��֡�Ŀ��*/
    HI_U32 u32Height;/**< expected output height of frame.*//**<CNcomment:ϣ����ȡ��֡�ĸ߶�*/   
    
    HI_RECT_S  zmeCropRect;/**< crop the source frame.*//**<CNcomment: �ü�Դ֡.*/    
    HI_UNF_VIDEO_FORMAT_E u32DataFormat;/**< set the output pixel format.*//**<CNcomment: ����������ش�Ÿ�ʽ*/
    
    HI_U32  bufferNumber; /**< set the buffer number allocated by user.*//**<CNcomment: �����û������buffer��Ŀ.*/    
    HI_BOOL bUserAlloc;   /**< set the flag whether user allocate buffer.*//**<CNcomment: �����û�����buffer�ı�־.*/    
    
    HI_U32 u32BufPhyAddr[HI_DISP_MIRROR_BUFFER_MAX_NUMBER]; /**< set the flag whether user allocate buffer.*//**<CNcomment: �����û�����buffer�ı�־.*/    
    HI_U32 u32BufStride;/**< set the buffer line stride.*//**<CNcomment: ����buffer����stride.*/    
    HI_U32 u32BufSize;/**<set the buffer size.*//**<CNcomment: ����buffer�Ĵ�С.*/    
} HI_UNF_DISP_CAST_CFG_S;

/**define the cast frame infor.*/
/**CNcomment: ץ�����֡��Ϣ*/
typedef struct hiUNF_DISP_MIRROR_FRAME_S
{
    HI_U32 u32Width;/**< the width of frame.*//**<CNcomment:ץ��֡�Ŀ��*/
    HI_U32 u32Height;/**< the height of frame.*//**<CNcomment:ץ��֡�ĸ߶�*/
    
    HI_U32 fieldFlag; /**< useless in this version.*//**<CNcomment:���汾��֧��*/
    HI_UNF_VIDEO_FORMAT_E eDataFmt;  /**< the output pixel format*//**<CNcomment:������ش�Ÿ�ʽ*/
    HI_U32 u32PTS; /**< the output pts of the frame*//**<CNcomment:���֡��ʱ���*/
    HI_U32 u32Index; /**< the buffer index of the frame*//**<CNcomment:���֡���󶨵�buffer����*/

    HI_U32 u32BufPhyAddr; /**< the physical address of the buffer *//**<CNcomment:buffer�������ַ*/
    HI_U32 u32BufVirAddr;/**< the virtual address of the buffer *//**<CNcomment:buffer�������ַ*/
    HI_U32 u32YStride; /**<the line the stride of the frame. *//**<CNcomment:֡���в���*/
} HI_UNF_DISP_MIRROR_FRAME_S;

/**
\brief  creat a cast device to capture the screen.
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp                DISP channel ID.
\param[in] pstCfg                configure the attr we capture the screen.
\param[out] pstCfg               return the handle for the coming operation.
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT      invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_CreateCast(HI_UNF_DISP_E enDisp, HI_UNF_DISP_CAST_CFG_S *pstCfg, HI_HANDLE *phCast);

/**
\brief  destroy a cast device and release all the resources allocated.
\attention \n
none. CNcomment:�� CNend
\param[in] hCast                operation handle generated by HI_UNF_DISP_CreateCast.
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT        invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_DestroyCast(HI_HANDLE hCast);

/**
\brief  enable capture screen.
\attention \n
none. CNcomment:�� CNend
\param[in] hCast                operation handle generated by HI_UNF_DISP_CreateCast.
\param[in] bEnable              enable flag: 0: disable; 1: enable.
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT        invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable);

/**
\brief  get cast enable status.
\attention \n
none. CNcomment:�� CNend
\param[in] hCast           operation handle generated by HI_UNF_DISP_CreateCast.
\param[out] pbEnable        output value to indicates the enable status.
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT        invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable);

/**
\brief  get the capture frame from the capture channel hCast.
\attention \n
none. CNcomment:�� CNend
\param[in]  hCast        operation handle generated by HI_UNF_DISP_CreateCast.
\param[out] pstFrame     descriptor containning the stream infor. 
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT        invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_UNF_DISP_MIRROR_FRAME_S *pstFrame);

/**
\brief  return the capture frame to the capture channel hCast.
\attention \n
none. CNcomment:�� CNend
\param[in]  hCast        operation handle generated by HI_UNF_DISP_CreateCast.
\param[int] pstFrame     descriptor containning the stream infor. 
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_INVALID_PARA     invalid params
\retval ::HI_ERR_DISP_NO_INIT          display device not inited.
\retval ::HI_ERR_VO_INVALID_OPT        invalid operation.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_UNF_DISP_MIRROR_FRAME_S *pstFrame);

/**
\brief set Disp output range. CNcomment:����TC ������ɫ�ռ䣻 CNend
\attention \n
none. CNcomment:�� CNend
\param[in] enDisp                DISP channel ID.CNcomment:DISPͨ���� CNend
\param[in] bEnable              CNcomment:�Ƿ�ʹ��TC colorspace mode. CNend
\retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
\retval ::HI_ERR_DISP_DEV_NOT_EXIST     DISP DEVICE NOT EXIST.
\retval ::HI_ERR_DISP_NOT_DEV_FILE          not char device.
\retval ::HI_ERR_DISP_DEV_OPEN_ERR     open device error.
\see \n
none.CNcomment:�� CNend
*/
HI_S32 HI_UNF_DISP_SetCsc(HI_UNF_DISP_E enDisp, HI_BOOL bEnable);


/**
   \brief get color modulation parameter.CNcomment:����DISP��Ĭ������ CNend
   \attention \n
   none. CNcomment:�� CNend
   \retval ::HI_SUCCESS CNcomment: success.�ɹ� CNend
   \retval ::HI_ERR_DISP_DEV_NO_INIT	DISP uninitialization.CNcomment:DISPδ��ʼ�� CNend
   \retval ::HI_ERR_DISP_NULL_PTR		   Input pointer is NULL.CNcomment:����ָ��Ϊ�� CNend
   \retval ::HI_ERR_DISP_INVALID_PARA	invalid input parameter.CNcomment:��������Ƿ� CNend
   \retval ::HI_ERR_DISP_INVALID_OPT	invalid operation. CNcomment:�����Ƿ� CNend
   \see \n
   none.CNcomment:�� CNend
 */
HI_S32 HI_UNF_DISP_SetDefaultParam( HI_VOID);


#ifdef __cplusplus
 #if __cplusplus
}

 #endif
#endif
#endif
