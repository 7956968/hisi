/******************************************************************************

  Copyright (C), 2001-2011, Huawei Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_priv_hdmi.h
  Version       : Initial Draft
  Author        : q46153 l00168554
  Created       : 2010/6/15
  Last Modified :
  Description   : hdmi ioctl and HDMI API common data structs

******************************************************************************/

#ifndef  __HI_DRV_HDMI_H__
#define  __HI_DRV_HDMI_H__

//#include "hi_common_id.h"
#include "hi_module.h"
//#include "hi_common_log.h"
#include "hi_debug.h"

#include "hi_unf_hdmi.h"
#include "hi_error_mpi.h"
#include "hi_mpi_ao.h"

#ifdef __cplusplus
#if __cplusplus
	extern "C"{
#endif
#endif





/*
**HDMI Debug
*/
#ifndef CONFIG_SUPPORT_CA_RELEASE
#define HI_FATAL_HDMI(fmt...)       HI_FATAL_PRINT  (HI_ID_HDMI, fmt)
#define HI_ERR_HDMI(fmt...)         HI_ERR_PRINT    (HI_ID_HDMI, fmt)
#define HI_WARN_HDMI(fmt...)        HI_WARN_PRINT   (HI_ID_HDMI, fmt)
#define HI_INFO_HDMI(fmt...)        HI_INFO_PRINT   (HI_ID_HDMI, fmt)

#define debug_printk(fmt,args...) // printk(fmt,##args)
#else

#define HI_FATAL_HDMI(fmt...) 
#define HI_ERR_HDMI(fmt...) 
#define HI_WARN_HDMI(fmt...)
#define HI_INFO_HDMI(fmt...)
#define debug_printk(fmt,args...)  

#endif

/*hdmi audio interface */
typedef enum  hiHDMI_AUDIOINTERFACE_E
{
    HDMI_AUDIO_INTERFACE_I2S,
    HDMI_AUDIO_INTERFACE_SPDIF, 
    HDMI_AUDIO_INTERFACE_HBR, 
    HDMI_AUDIO_INTERFACE_BUTT
}HDMI_AUDIOINTERFACE_E;


typedef struct hiHDMI_AUDIO_ATTR_S
{
//  HI_BOOL                 bEnableAudio;        /**<Enable flag of Audio*//**CNcomment:�Ƿ�Enable��Ƶ */
    HDMI_AUDIOINTERFACE_E   enSoundIntf;         /**<the origin of Sound,suggestion set HI_UNF_SND_INTERFACE_I2S,the parameter need consistent with Ao input *//**<CNcomment:HDMI��Ƶ��Դ, ����HI_UNF_SND_INTERFACE_I2S,�˲�����Ҫ��AO���뱣��һ�� */
    HI_BOOL                 bIsMultiChannel;     /**<set mutiChannel or stereo ;0:stereo,1:mutichannel fixup 8 channel *//**<CNcomment:������������������0:��������1:�������̶�Ϊ8���� */
	HI_U32 					u32Channels;         //��channel��multy channel���������������ں�̬�ɵ�multy channel
    HI_UNF_SAMPLE_RATE_E    enSampleRate;        /**<the samplerate of audio,this parameter consistent with AO config *//**<CNcomment:PCM��Ƶ������,�˲�����Ҫ��AO�����ñ���һ�� */
    HI_U8                   u8DownSampleParm;    /**<PCM parameter of dowmsample,default 0*//**CNcomment:PCM��Ƶ����downsample�����ʵĲ�����Ĭ��Ϊ0 */
    
    HI_UNF_BIT_DEPTH_E      enBitDepth;          //ĿǰĬ����16bit    /**<the audio bit depth,defualt 16,this parameter consistent with AO config*//**<CNcomment:��Ƶλ��Ĭ��Ϊ16,�˲�����Ҫ��AO�����ñ���һ�� */
    HI_U8                   u8I2SCtlVbit;        /**<reserve:config 0,I2S control(0x7A:0x1D)*//**CNcomment:������������Ϊ0, I2S control (0x7A:0x1D) */

    HI_UNF_HDMI_CODING_TYPE_E   enAudioCode;

//  HI_BOOL                 bEnableAudInfoFrame; /**<Enable flag of Audio InfoFrame,suggestion:enable*//**<CNcomment:�Ƿ�ʹ�� AUDIO InfoFrame������ʹ�� */
}HDMI_AUDIO_ATTR_S;

/**HDMI status*/
/**CNcomment: HDMI״̬ */
typedef struct hiDRV_HDMI_STATUS_S
{
    HI_BOOL             bConnected;             /**<The Device is connected or disconnected *//**<CNcomment:�豸�Ƿ����� */
    HI_BOOL             bSinkPowerOn;           /**<The sink is PowerOn or not*//**<CNcomment:Sink�豸�Ƿ��ϵ� */
    HI_BOOL             bAuthed;                /**<HDCP Authentication *//**<CNcomment:HDCP �Ƿ�������� */
    HI_U8               u8Bksv[5];              /**<Bksv of sink 40bits*//**<CNcomment:���ն˵�Bksv */
}HI_DRV_HDMI_STATUS_S;

/**HDMI Delay struct*/
/**CNcomment: HDMI ��ʱ�ṹ */
typedef struct hiDRV_HDMI_DELAY_S
{
    HI_U32  u32MuteDelay;           /**<delay for avmute *//**<CNcomment:avmute����ʱ */
    HI_U32  u32FmtDelay;  			/**<delay for setformat *//**<CNcomment:������ʽ����ʱ */
    HI_BOOL bForceFmtDelay;         /**<force setformat delay mode *//**<CNcomment:�л���ʽǿ����ʱģʽ */
    HI_BOOL bForceMuteDelay;        /**<force avmute delay mode *//**<CNcomment:muteǿ����ʱģʽ */
}HI_DRV_HDMI_DELAY_S;

/**EDID MANUFACTURE Info struct*//**CNcomment:EDID��������Ϣ */
typedef struct hiHDMI_EDID_MANUFACTURE_INFO_E
{
    HI_U8               u8IDManufactureName[4]; /**<Manufacture name*//**<CNcomment:�豸���̱�ʶ */
    HI_U32              u32IDProductCode;       /**<Product code*//**<CNcomment:�豸ID */
    HI_U32              u32IDSerialNumber;      /**<Serial numeber of Manufacture*//**<CNcomment:�豸���к� */
    HI_U32              u32WeekOfManufacture;   /**<the week of manufacture*//**<CNcomment:�豸��������(��) */
    HI_U32              u32YearOfManufacture;   /**<the year of manufacture*//**<CNcomment:�豸��������(��) */
}HI_HDMI_EDID_MANUFACTURE_INFO_S;

typedef struct hiHDMI_AUDIO_CAPABILITY_S
{
    HI_BOOL             bAudioFmtSupported[HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT]; /**<Audio capability,reference EIA-CEA-861-D,table 37,HI_TRUE:support this Audio type;HI_FALSE,nonsupport this Audio type*//**<CNcomment:��Ƶ������, ��ο�EIA-CEA-861-D ��37;HI_TRUE��ʾ֧��������ʾ��ʽ��HI_FALSE��ʾ��֧�� */
    HI_U32              u32AudioSampleRateSupported[HI_UNF_HDMI_MAX_AUDIO_SMPRATE_COUNT]; /**<PCM smprate capability,0: illegal value,other is support PCM smprate *//**<CNcomment:PCM��Ƶ��������������0Ϊ�Ƿ�ֵ������Ϊ֧�ֵ���Ƶ������ */
    HI_U32              u32MaxPcmChannels;        /**<Audio max PCM Channels number*//**CNcomment:��Ƶ����PCMͨ���� */
    //HI_U8             u8Speaker;                /**<Speaker location,please reference EIA-CEA-D the definition of SpekearDATABlock*//**<CNcomment:������λ�ã���ο�EIA-CEA-861-D��SpeakerDATABlock�Ķ��� */
}HI_DRV_HDMI_AUDIO_CAPABILITY_S;


/*In order to extern ,so we define struct*/
typedef struct hiHDMI_VIDEO_ATTR_S
{
//	HI_BOOL                 bEnableHdmi;         /**<force to HDMI or DVI,the value must set before HI_UNF_HDMI_Start or behind HI_UNF_HDMI_Stop*//**<CNcomment:�Ƿ�ǿ��HDMI,����ΪDVI.��ֵ������ HI_UNF_HDMI_Start֮ǰ����HI_UNF_HDMI_Stop֮������  */
//  HI_BOOL                 bEnableVideo;        /**<parameter must set HI_TRUE,or the HDMI diver will force to set HI_TRUE*//**<CNcomment:������HI_TRUE, �����HI_FALSE:HDMI������ǿ������ΪHI_TRUE */ 
    HI_UNF_ENC_FMT_E        enVideoFmt;          /**<video fromat ,the format must consistent with display  config*//**<CNcomment:��Ƶ��ʽ,�˲�����Ҫ��Display���õ���ʽ����һ�� */
//  HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;       /**<HDMI output vedio mode VIDEO_MODE_YCBCR,VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 *//**<CNcomment:HDMI�����Ƶģʽ��VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 */
//  HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode;    /**<Deep Color output mode,defualt: HI_UNF_HDMI_DEEP_COLOR_24BIT *//**<CNcomment:DeepColor���ģʽ, Ĭ��ΪHI_UNF_HDMI_DEEP_COLOR_24BIT */
//  HI_BOOL                 bxvYCCMode;          /**<the xvYCC output mode,default:HI_FALSE*//**<CNcomment:< xvYCC���ģʽ��Ĭ��ΪHI_FALSE */

//  HI_BOOL                 bEnableAviInfoFrame; /**<Enable flag of AVI InfoFrame,suggestion:enable *//**<CNcomment:�Ƿ�ʹ�� AVI InfoFrame������ʹ�� */
//  HI_BOOL                 bEnableSpdInfoFrame; /**<Enable flag of SPD info frame,suggestion:disable*//**<CNcomment:�Ƿ�ʹ�� SPD InfoFrame�� ����ر� */
//  HI_BOOL                 bEnableMpegInfoFrame;/**<Enable flag of MPEG info frame,suggestion:disable*//**<CNcomment:�Ƿ�ʹ�� MPEG InfoFrame�� ����ر� */

    HI_BOOL                 b3DEnable;           /**<0:disable 3d,1,enable 3d mode*//**<CNcomment:< 0:3D�����1:3Dģʽ�� */
    HI_U32                  u83DParam;           /**<3D Parameter,defualt HI_FALSE*//**<CNcomment:< 3D Parameter, Ĭ��ΪHI_FALSE */

//  HI_U32                  bDebugFlag;          /**<the flag of hdmi dubug,suggestion:disable*//**<CNcomment:< �Ƿ�ʹ�� ��hdmi�ڲ�debug��Ϣ�� ����ر� */
//  HI_BOOL                 bHDCPEnable;         /**<0:HDCP disable mode,1:eable HDCP mode*//**<CNcomment:< 0:HDCP�����1:HDCPģʽ�� */
}HDMI_VIDEO_ATTR_S;
    
    /*In order to extern ,so we define struct*/
typedef struct hiHDMI_APP_ATTR_S
{
    HI_BOOL                 bEnableHdmi;         /**<force to HDMI or DVI,the value must set before HI_UNF_HDMI_Start or behind HI_UNF_HDMI_Stop*//**<CNcomment:�Ƿ�ǿ��HDMI,����ΪDVI.��ֵ������ HI_UNF_HDMI_Start֮ǰ����HI_UNF_HDMI_Stop֮������  */
    HI_BOOL                 bEnableVideo;        /**<parameter must set HI_TRUE,or the HDMI diver will force to set HI_TRUE*//**<CNcomment:������HI_TRUE, �����HI_FALSE:HDMI������ǿ������ΪHI_TRUE */
    HI_BOOL                 bEnableAudio;        /**<Enable flag of Audio*//**CNcomment:�Ƿ�Enable��Ƶ */

    HI_UNF_HDMI_VIDEO_MODE_E enVidOutMode;       /**<HDMI output vedio mode VIDEO_MODE_YCBCR,VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 *//**<CNcomment:HDMI�����Ƶģʽ��VIDEO_MODE_YCBCR444��VIDEO_MODE_YCBCR422��VIDEO_MODE_RGB444 */
    HI_UNF_HDMI_DEEP_COLOR_E enDeepColorMode;    /**<Deep Color output mode,defualt: HI_UNF_HDMI_DEEP_COLOR_24BIT *//**<CNcomment:DeepColor���ģʽ, Ĭ��ΪHI_UNF_HDMI_DEEP_COLOR_24BIT */
    HI_BOOL                 bxvYCCMode;          /**<the xvYCC output mode,default:HI_FALSE*//**<CNcomment:< xvYCC���ģʽ��Ĭ��ΪHI_FALSE */

    HI_BOOL                 bEnableAviInfoFrame; /**<Enable flag of AVI InfoFrame,suggestion:enable *//**<CNcomment:�Ƿ�ʹ�� AVI InfoFrame������ʹ�� */
    HI_BOOL                 bEnableSpdInfoFrame; /**<Enable flag of SPD info frame,suggestion:disable*//**<CNcomment:�Ƿ�ʹ�� SPD InfoFrame�� ����ر� */
    HI_BOOL                 bEnableMpegInfoFrame;/**<Enable flag of MPEG info frame,suggestion:disable*//**<CNcomment:�Ƿ�ʹ�� MPEG InfoFrame�� ����ر� */
    HI_BOOL                 bEnableAudInfoFrame; /**<Enable flag of Audio InfoFrame,suggestion:enable*//**<CNcomment:�Ƿ�ʹ�� AUDIO InfoFrame������ʹ�� */

    HI_U32                  bDebugFlag;          /**<the flag of hdmi dubug,suggestion:disable*//**<CNcomment:< �Ƿ�ʹ�� ��hdmi�ڲ�debug��Ϣ�� ����ر� */
    HI_BOOL                 bHDCPEnable;         /**<0:HDCP disable mode,1:eable HDCP mode*//**<CNcomment:< 0:HDCP�����1:HDCPģʽ�� */
    
    HI_BOOL                 b3DEnable;           /**<0:disable 3d,1,enable 3d mode*//**<CNcomment:< 0:3D�����1:3Dģʽ�� */
    HI_U32                  u83DParam;           /**<3D Parameter,defualt HI_FALSE*//**<CNcomment:< 3D Parameter, Ĭ��ΪHI_FALSE */
}HDMI_APP_ATTR_S;

/*hdmi struct */
typedef struct hiHDMI_ATTR_S
{
    HDMI_AUDIO_ATTR_S  stAudioAttr;
    HDMI_VIDEO_ATTR_S  stVideoAttr;
    HDMI_APP_ATTR_S    stAppAttr;
}HDMI_ATTR_S;



    
    
/******************************************** end ************************************************/
  
///*
//** HDMI IOCTL Data Structure
//*/
//typedef struct hiHDMI_OPEN_S
//{
//    HI_UNF_HDMI_ID_E                  enHdmi;
//    HI_UNF_HDMI_FORCE_ACTION_E        enDefaultMode;
//}HDMI_OPEN_S;

typedef struct hiHDMI_OPEN_S
{
	HI_UNF_HDMI_ID_E                  enHdmi;
    HI_UNF_HDMI_FORCE_ACTION_E        enDefaultMode;
    HI_U32                            u32ProcID;
}HDMI_OPEN_S;


typedef struct hiHDMI_DEINIT_S
{
    HI_U32                          NoUsed;
}HDMI_DEINIT_S;

//typedef struct hiHDMI_INIT_S
//{
//    HI_U32                          NoUsed;
//}HDMI_INIT_S;

/*
** HDMI IOCTL Data Structure
*/
typedef struct hiHDMI_INIT_S
{
    HI_UNF_HDMI_FORCE_ACTION_E      enForceMode;
    HI_U32                          UCallbackAddr;
}HDMI_INIT_S;

typedef struct hiHDMI_COLSE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_CLOSE_S;

typedef struct hiHDMI_START_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_START_S;

typedef struct hiHDMI_STOP_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          NoUsed;
}HDMI_STOP_S;

//typedef struct hiHDMI_POLL_EVENT_S
//{
//    HI_UNF_HDMI_ID_E                enHdmi;
//    HI_UNF_HDMI_EVENT_TYPE_E        Event;
//    HI_U32                          u32CallbackAddr;
//}HDMI_POLL_EVENT_S;

typedef struct hiHDMI_POLL_EVENT_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_EVENT_TYPE_E        Event;
    HI_U32                          u32ProcID;
}HDMI_POLL_EVENT_S;    


typedef struct hiHDMI_SINK_CAPABILITY_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_SINK_CAPABILITY_S   SinkCap;
}HDMI_SINK_CAPABILITY_S;

//typedef struct hiHDMI_PORT_ATTR_S
//{
//    HI_UNF_HDMI_ID_E                enHdmi;
//    HDMI_ATTR_S                     stHDMIAttr;
//}HDMI_PORT_ATTR_S;

typedef struct hiHDMI_PORT_ATTR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HDMI_APP_ATTR_S                 stHdmiAppAttr;
}HDMI_PORT_ATTR_S;


typedef struct hiHDMI_INFORFRAME_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_INFOFRAME_TYPE_E    enInfoFrameType;
    HI_UNF_HDMI_INFOFRAME_S         InfoFrame;
}HDMI_INFORFRAME_S;

typedef struct hiHDMI_AVMUTE_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          AVMuteEnable;
}HDMI_AVMUTE_S;

//typedef struct hiHDMI_VIDEOTIMING_S
//{
//    HI_UNF_HDMI_ID_E                enHdmi;
//    HI_U32                          VideoTiming;
//}HDMI_VIDEOTIMING_S;

typedef struct hiHDMI_VIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_ENC_FMT_E                VideoTiming;
    HI_UNF_HDMI_3D_FORMAT_E         enStereo;
}HDMI_VIDEOTIMING_S;


typedef struct hiHDMI_PREVIDEOTIMING_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          VideoTiming;
}HDMI_PREVIDEOTIMING_S;

typedef struct hiHDMI_DEEPCOLOR_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_DEEP_COLOR_E        enDeepColor;
}HDMI_DEEPCOLORC_S;

typedef struct hiHDMI_SET_XVYCC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          xvYCCEnable;
}HDMI_SET_XVYCC_S;

typedef struct hiHDMI_CEC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_CMD_S           CECCmd;
    HI_U32                          timeout;
}HDMI_CEC_S;


typedef struct
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_UNF_HDMI_CEC_STATUS_S        stStatus;
}HDMI_CEC_STATUS;

typedef struct hiHDMI_EDID_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U8                           u8EdidValid;
    HI_U32                          u32Edidlength;
    HI_U8                           u8Edid[512];
}HDMI_EDID_S;

typedef struct hiHDMI_PLAYSTAUS_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          u32PlayStaus;
}HDMI_PLAYSTAUS_S;

typedef struct hiHDMI_CEC_ENABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_ENABlE_S;

typedef struct hiHDMI_CEC_DISABLE_S
{
    HI_U32                          NoUsed;
}HDMI_CEC_DISABLE_S;

typedef struct hiHDMI_REGCALLBACKFUNC_S
{
    HI_UNF_HDMI_ID_E                enHdmi;
    HI_U32                          u32CallBackAddr;
}HDMI_REGCALLBACKFUNC_S;

/******************************************** start ************************************************/
typedef struct hiHDMI_GETPROCID_S
{
	HI_UNF_HDMI_ID_E                enHdmi;
	HI_U32                          u32ProcID;
}HDMI_GET_PROCID_S;

typedef struct hiHDMI_GETAOATTR_S
{
	HI_UNF_HDMI_ID_E                enHdmi;
	HDMI_AUDIO_ATTR_S               stAudAttr;
}HDMI_GETAOATTR_S;

typedef struct hiHDMI_STATUS_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_DRV_HDMI_STATUS_S           stStatus;
}HDMI_STATUS_S;

typedef struct hiHDMI_SETFMT_DELAY_S
{
	HI_UNF_HDMI_ID_E               enHdmi;
	HI_DRV_HDMI_DELAY_S            stDelay;
}HDMI_DELAY_S;

/******************************************** end ************************************************/
  


HI_S32 HI_DRV_HDMI_Init(HI_VOID);
HI_VOID HI_DRV_HDMI_Deinit(HI_VOID);
HI_S32 HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi);
HI_S32 HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi);

HI_S32 HI_DRV_HDMI_PlayStus(HI_UNF_HDMI_ID_E enHdmi, HI_U32 *pu32Stutus);
HI_S32 HI_DRV_AO_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);
HI_S32 HI_DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_SINK_CAPABILITY_S *pstSinkCap);
HI_S32 HI_DRV_HDMI_GetAudioCapability(HI_UNF_HDMI_ID_E enHdmi, HI_DRV_HDMI_AUDIO_CAPABILITY_S *pstAudCap);
HI_S32 HI_DRV_HDMI_SetAudioMute(HI_UNF_HDMI_ID_E enHdmi);
HI_S32 HI_DRV_HDMI_SetAudioUnMute(HI_UNF_HDMI_ID_E enHdmi);

HI_S32 HI_DRV_HDMI_AudioChange(HI_UNF_HDMI_ID_E enHdmi, HDMI_AUDIO_ATTR_S *pstHDMIAOAttr);

HI_S32 HI_DRV_HDMI_PreFormat(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_ENC_FMT_E enEncodingFormat);
HI_S32 HI_DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_ENC_FMT_E enFmt, HI_UNF_HDMI_3D_FORMAT_E enStereo);

HI_S32 HI_DRV_HDMI_Detach(HI_UNF_HDMI_ID_E enHdmi);
HI_S32 HI_DRV_HDMI_Attach(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_ENC_FMT_E enFmt, HI_UNF_HDMI_3D_FORMAT_E enStereo);

HI_VOID HI_DRV_HDMI_ProcHotPlug(HI_HANDLE hHdmi);
HI_S32 HI_DRV_HDMI_ExtIoctl(unsigned int cmd, void *argp);
HI_S32  HI_DRV_HDMI_GetCECAddr(HI_U32 *pcec_addr);
HI_S32 HI_DRV_HDMI_GetBinInfoFrame(HI_UNF_HDMI_INFOFRAME_TYPE_E enInfoFrameType,void *infor_ptr);
HI_S32 HI_DRV_HDMI_Set3DMode(HI_UNF_HDMI_ID_E enHdmi, HI_BOOL b3DEnable,HI_U8 u83Dmode);

////-----------------
//HI_S32 HI_DRV_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_ENC_FMT_E enEncodingFormat);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
/*--------------------------END-------------------------------*/
