/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
  File Name     : hi_unf_vo.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/1/22
  Last Modified :
  Description   : header file for audio and video output control
  Function List :
  History       :
  1.Date        :
  Author        : z67193
  Modification  : Created file
******************************************************************************/
/** 
 * \file
 * \brief Describes the information about the SOUND (SND) module. CNcomment:�ṩSOUND�������Ϣ CNend
 */
 
#ifndef  __HI_UNF_SND_H__
#define  __HI_UNF_SND_H__

#include "hi_unf_common.h"
//#include "hi_debug.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */

/********************************Macro Definition********************************/
/** \addtogroup      SOUND */
/** @{ */  /** <!-- ��SOUND�� */

/**Maximum sound outputport*/
/**CNcomment:���sound����˿�*/
#define HI_UNF_SND_OUTPUTPORT_MAX 16

/** @} */  /** <!-- ==== Macro Definition end ==== */


/*************************** Structure Definition ****************************/
/** \addtogroup      SOUND */
/** @{ */  /** <!--  ��SOUND�� */

/**Audio volume attribute*/
/**CNcomment:��Ƶ��������*/
typedef struct hiHI_UNF_SND_GAIN_ATTR_S
{
    HI_BOOL bLinearMode; /**< gain type of volume*/ /**<CNcomment:����ģʽ */
    HI_S32  s32Gain; /**<Linear gain(bLinearMode is HI_TRUE) , ranging from 0 to 100*/ /**<CNcomment:��������: 0~100 */
                     /**<Decibel gain(bLinearMode is HI_FALSE) , ranging from -70dB to 0dB */ /**<CNcomment: dB����:-70~0*/
} HI_UNF_SND_GAIN_ATTR_S;

/**Audio outputport: DAC0,I2S0,SPDIF0,HDMI0,ARC0*/
/**CNcomment:��Ƶ����˿�:DAC0,I2S0,SPDIF0,HDMI0,ARC0*/
typedef enum hiUNF_SND_OUTPUTPORT_E
{
    HI_UNF_SND_OUTPUTPORT_DAC0 = 0,

    HI_UNF_SND_OUTPUTPORT_I2S0,

    HI_UNF_SND_OUTPUTPORT_I2S1,

    HI_UNF_SND_OUTPUTPORT_SPDIF0,

    HI_UNF_SND_OUTPUTPORT_HDMI0,

    HI_UNF_SND_OUTPUTPORT_ARC0,
	
    HI_UNF_SND_OUTPUTPORT_EXT_DAC1 = 0x50,

    HI_UNF_SND_OUTPUTPORT_EXT_DAC2,

    HI_UNF_SND_OUTPUTPORT_EXT_DAC3,

    HI_UNF_SND_OUTPUTPORT_ALL = 0x7fff,

    HI_UNF_SND_OUTPUTPORT_BUTT,
} HI_UNF_SND_OUTPUTPORT_E;

/**Defines internal Audio DAC outport attribute */
/**CNcomment:����������ƵDAC����˿�����*/
typedef struct hiUNF_SND_DAC_ATTR_S
{
    HI_VOID* pPara;
} HI_UNF_SND_DAC_ATTR_S;

/**Defines  Audio I2S outport attribute */
/**CNcomment:������ƵI2S����˿�����*/
typedef struct hiUNF_SND_I2S_ATTR_S
{
    HI_UNF_I2S_ATTR_S  stAttr;
}  HI_UNF_SND_I2S_ATTR_S;

/**Defines  S/PDIF outport attribute */
/**CNcomment:����S/PDIF����˿�����*/
typedef struct hiUNF_SND_SPDIF_ATTR_S
{
    HI_VOID* pPara;
}  HI_UNF_SND_SPDIF_ATTR_S;

/**Defines  HDMI Audio outport attribute */
/**CNcomment:����HDMI��Ƶ����˿�����*/
typedef struct hiUNF_SND_HDMI_ATTR_S
{
    HI_VOID* pPara;
} HI_UNF_SND_HDMI_ATTR_S;

/**Defines  HDMI ARC outport attribute */
/**CNcomment:����HDMI��Ƶ�ش�ͨ���˿�����*/
typedef struct hiUNF_SND_ARC_ATTR_S
{
    HI_VOID* pPara;
} HI_UNF_SND_ARC_ATTR_S;

/**Defines  Audio outport attribute */
/**CNcomment:������Ƶ����˿�����*/
typedef struct hiUNF_SND_OUTPORT_S
{
    HI_UNF_SND_OUTPUTPORT_E enOutPort;
    union
    {
        HI_UNF_SND_DAC_ATTR_S stDacAttr;
        HI_UNF_SND_I2S_ATTR_S stI2sAttr;
        HI_UNF_SND_SPDIF_ATTR_S stSpdifAttr;
        HI_UNF_SND_HDMI_ATTR_S stHDMIAttr;
        HI_UNF_SND_ARC_ATTR_S stArcAttr;
    } unAttr;
} HI_UNF_SND_OUTPORT_S;

/**Defines  AVC attribute */
/**CNcomment:AVC����*/
typedef struct hiHI_UNF_SND_AVC_ATTR_S
{
    HI_S32  s32ThresholdLevel;  /**<the threshold value, if -32.125, value is -32125, step by 0.125dB,threshold ranging from -40dB to -16dB,default value -32dB*/
                                /**<CNcomment: �ﵽ���̧�������ת�۵�ƽ,��ֵΪ-32.125,����ֵΪ-32125,����Ϊ0.125dB,��Χ:[-40dB, -16dB),Ĭ��ֵ-32dB*/
    HI_S32  s32Gain;            /**<the gain value, if 2.125, value is 2125,step by 0.125dB,threshold ranging from 0dB to 22dB,default value 5dB*/
                                /**<CNcomment: ̧������,��ֵΪ2.125,����ֵΪ2125,����Ϊ0.125dB,��Χ:[0dB,22dB],Ĭ��ֵ5dB*/
    HI_S32  s32LimiterLevel;    /**<the target level value, if -2.125, value is -2125, step by 0.125dB,threshold ranging from -16dB to -6dB,default value -10dB*/
                                /**<CNcomment:Ŀ��ֵ,��ֵΪ-2.125,����ֵΪ-2125,����Ϊ0.125dB,��Χ:[-16dB,-6dB],Ĭ��ֵ-10dB*/
    HI_U32  u32AttackTime;      /**<the attack time(unit:ms),ranging from 20 to 2000,default value 50(recommended)*/ /**<CNcomment: ��ʼ��Чʱ��(��λ:ms),��Χ:[20, 2000]��Ĭ��ֵ50���������޸�*/
    HI_U32  u32ReleaseTime;     /**<the release time(unit:ms),ranging from 20 to 2000,default value 100(recommended)*/ /**<CNcomment: ��ʼʧЧʱ��(��λ:ms),��Χ:[20, 2000]��Ĭ��ֵ100���������޸�*/
} HI_UNF_SND_AVC_ATTR_S;
/**HDMI MODE:AUTO,LPCM,RAW,HBR2LBR*/
/**CNcomment:HDMI ģʽ:AUTO,LPCM,RAW,HBR2LBR*/
typedef enum hiHI_UNF_SND_HDMI_MODE_E
{
    HI_UNF_SND_HDMI_MODE_LPCM = 0,       /**<HDMI LCPM2.0*/ /**<CNcomment: ������pcm*/
    HI_UNF_SND_HDMI_MODE_RAW,            /**<HDMI Pass-through.*/ /**<CNcomment: HDMI͸��*/
    HI_UNF_SND_HDMI_MODE_HBR2LBR,        /**<HDMI Pass-through force high-bitrate to low-bitrate.*/ /**<CNcomment: �����������Ƶ��������*/
    HI_UNF_SND_HDMI_MODE_AUTO,           /**<automatically match according to the EDID of HDMI */ /**<CNcomment: ����HDMI EDID�����Զ�ƥ��*/
    HI_UNF_SND_HDMI_MODE_BUTT
} HI_UNF_SND_HDMI_MODE_E;

/**SPDIF MODE:LPCM,RAW*/
/**CNcomment:SPDIF ģʽ:LPCM,RAW*/
typedef enum hiHI_UNF_SND_SPDIF_MODE_E
{
    HI_UNF_SND_SPDIF_MODE_LPCM,           /**<SPDIF LCPM2.0*/ /**<CNcomment: ������pcm*/
    HI_UNF_SND_SPDIF_MODE_RAW,            /**<SPDIF Pass-through.*/ /**<CNcomment: SPDIF͸��*/
    HI_UNF_SND_SPDIF_MODE_BUTT
} HI_UNF_SND_SPDIF_MODE_E;
/** @} */  /** <!-- ==== Structure Definition end ==== */
/**Audio Track Type: Master, Slave, Virtual channel*/
/**CNcomment:��ƵTrack����:��ͨ�� ��ͨ�� ����ͨ��*/
typedef enum hiHI_UNF_SND_TRACK_TYPE_E
{
    HI_UNF_SND_TRACK_TYPE_MASTER = 0,     
    HI_UNF_SND_TRACK_TYPE_SLAVE, 
    HI_UNF_SND_TRACK_TYPE_BUTT 
} HI_UNF_SND_TRACK_TYPE_E;
/**Audio output attribute */
/**CNcomment:��Ƶ�������*/
typedef struct hiHI_UNF_AUDIOTRACK_ATTR_S
{
    HI_UNF_SND_TRACK_TYPE_E   enTrackType;    /**<Track Type*/ /**<CNcomment:Track����*/
    HI_U32                  u32FadeinMs;      /**<Fade in time(unit:ms)*/ /**<CNcomment:����ʱ��(��λ: ms)*/
    HI_U32                  u32FadeoutMs;     /**<Fade out time(unit:ms)*/ /**<CNcomment:����ʱ��(��λ: ms)*/
    HI_U32                    u32OutputBufSize;  /**<Track output buffer size*/ /**<CNcomment:Track��������С*/
    HI_U32                  u32BufLevelMs;    /**<Output buffer data size control(ms)*/ /**<CNcomment:�������������������(ms)*/
} HI_UNF_AUDIOTRACK_ATTR_S;


/**Defines the ID of the audio output (AO) device.*/
/**CNcomment:������Ƶ����豸��*/
typedef enum hiUNF_SND_E
{
    HI_UNF_SND_0,           /**<AO device 0*/ /**<CNcomment:��Ƶ����豸0 */
    HI_UNF_SND_BUTT    
} HI_UNF_SND_E;

/**Defines the type of the AO interface.*/
/**CNcomment:������Ƶ����ӿ�����*/
typedef enum hiUNF_SND_INTERFACE_E
{
    HI_UNF_SND_INTERFACE_I2S,      /**<Inter-IC sound (I2S) mode*/ /**<CNcomment:I2S ģʽ */
    HI_UNF_SND_INTERFACE_SPDIF,    /**<Sony/Philips digital interface (SPDIF) mode*/ /**<CNcomment:SPDIF ģʽ */
    HI_UNF_SND_INTERFACE_HBR,      /**<SPDIF mode*/ /**<CNcomment:SPDIF ģʽ */
    HI_UNF_SND_INTERFACE_BUTT
}HI_UNF_SND_INTERFACE_E;


/**Audio mixing type*/
/**CNcomment:��Ƶ�������� */
typedef enum hiHI_UNF_SND_MIX_TYPE_E
{
    HI_UNF_SND_MIX_TYPE_MASTER,     /**<Primary audio*/ /**<CNcomment:��Ϊ���� */
    HI_UNF_SND_MIX_TYPE_SLAVE,      /**<Mixing audio*/ /**<CNcomment:��Ϊ���� */
    HI_UNF_SND_MIX_TYPE_BUTT 
} HI_UNF_SND_MIX_TYPE_E;


/**Defines the AO mode.*/
/**CNcomment:������Ƶ���ģʽ�ṹ��*/
typedef struct hiUNF_SND_INTERFACE_S
{
    HI_UNF_SND_INTERFACE_E  enInterface;     /**<Output type of the audio interface*/ /**<CNcomment:��Ƶ�ӿ�������� */
    HI_BOOL                 bEnable;         /**<AO enable*/ /**<CNcomment:�Ƿ�ʹ����Ƶ��� */
}HI_UNF_SND_INTERFACE_S;

/**Defines the AO attributes.*/
/**CNcomment:������Ƶ������Խṹ��*/
typedef struct hiHI_UNF_SND_ATTR_S
{
    HI_U32                    u32PortNum;  /**<Outport number attached sound*/ /**<CNcomment:�󶨵�Sound�豸������˿���*/
    HI_UNF_SND_OUTPORT_S stOutport[HI_UNF_SND_OUTPUTPORT_MAX];  /**<Outports attached sound*/ /**<CNcomment:�󶨵�Sound�豸������˿�*/
    HI_UNF_SAMPLE_RATE_E    enSampleRate;       /**<Sound samplerate*/ /**<CNcomment:Sound�豸���������*/
    HI_U32              u32MasterOutputBufSize; /**<Size (in byte) of the buffer for playing the master audio. Note: The size must be 64 times of word8.*/
                                                /**<CNcomment:�����طŻ������ռ��С. unit:byte */
    HI_U32              u32SlaveOutputBufSize;  /**<Size (in byte) of the buffer for playing the slave audio. Note: The size must be 64 times of word8 */
                                                /**<CNcomment:�����طŻ������ռ��С. unit:byte */
    HI_BOOL            bMixerEngineSupport;   /**< if bMixerEngineSupport == HI_TRUE,  UNF_SND support MixerEngine,slave pcm is repalced by one mixer of the MixerEngine,
    									  if bMixerEngineSupport == HI_FALSE, UNF_SND dont support MixerEngine default: HI_FALSE
    									  */  
} HI_UNF_SND_ATTR_S;

/**Audio Mixer attribute */
typedef struct hiHI_UNF_MIXER_ATTR_S
{
    HI_U32              u32MixerWeight;    /**<Mixer weight, ranging from 0 to 100*/ /**<CNcomment:����Ȩ��: 0~100 */
    HI_U32              u32MixerBufSize;   /**<Size (in byte) of the mixer buffer, ranging from (1024 x 16) to (1024 x 1024)*/ /**<CNcomment:��������������С unit:byte */
} HI_UNF_MIXER_ATTR_S;

typedef struct hiHI_UNF_MIXER_STATUSINFO_S
{
    HI_S32 u32MixerID;   /**<Mixer ID, ranging from 0 to 7*/ /**<CNcomment:������Ӳ��ID : ��Χ0~7 */
    HI_S32 u32Working;   /**<Working status of the mixer. 0 : stop; 1: pause; 2: running*/ /**<CNcomment:����������״̬: 0 : stop, 1: pause, 2:running */

    HI_U32 u32BufferSize;      /**<Size (in byte) of the mixer buffer*/ /**<CNcomment:��������������Сunit: byte.*/
    HI_U32 u32BufferByteLeft;  /**<Remaining audio data (in byte) in the mixer buffer*/ /**<CNcomment:��������������Ƶ����ʣ�� unit : byte.*/
    HI_BOOL bDataNotEnough;    /**<HI_FALSE : data is enough, HI_TRUE: data is not enough,need more data */
    HI_U32  u32BufDurationMs;  /**<Duration (in ms) for playing the remaining audio data in the mixer buffer*/ /**<CNcomment:������������ʣ����Ƶ���ݿɲ���ʱ��  unit : MS.*/

} HI_UNF_MIXER_STATUSINFO_S;

/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API declaration *****************************/
/** \addtogroup      SOUND */
/** @{ */  /** <!--  [SOUND] */

/** 
\brief Initializes an AO device. CNcomment:��ʼ����Ƶ����豸 CNend
\attention \n
Before calling the SND module, you must call this application programming interface (API). CNcomment:����SNDģ��Ҫ�����ȵ��øýӿ� CNend
\param N/A
\retval HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_Init(HI_VOID);

/** 
\brief Deinitializes an AO device. CNcomment:ȥ��ʼ����Ƶ����豸 CNend
\attention \n
N/A
\param N/A
\retval HI_SUCCESS Success CNcomment:�ɹ� CNend 
\see \n
N/A
*/
HI_S32 HI_UNF_SND_DeInit(HI_VOID);

/** 
\brief Sets an AO device. CNcomment:������Ƶ����豸 CNend
\attention \n
You must call this API before calling HI_UNF_SND_Open. Otherwise, the VO device is started in the default mode. 
CNcomment:������HI_UNF_SND_Open ǰ���ã�������Ĭ�Ϸ�ʽ����Ƶ�豸 CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] pstAttr     Configured audio attributes CNcomment:��Ƶ�������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetOpenAttr(HI_UNF_SND_E enSound,HI_UNF_SND_ATTR_S *pstAttr);

/** 
\brief Obtains the configured parameters of an AO device. CNcomment:��ȡ��Ƶ����豸���ò��� CNend
\attention \n
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pstAttr     Audio attributes CNcomment:��Ƶ���� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetOpenAttr(HI_UNF_SND_E enSound, HI_UNF_SND_ATTR_S *pstAttr);

/** 
\brief Starts an AO device. CNcomment:����Ƶ����豸 CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\retval ::HI_SUCCESS Success CNcomment:
\retval ::HI_ERR_HIAO_CREATE_FAIL	    The device fails to be started. CNcomment:���豸ʧ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_Open(HI_UNF_SND_E enSound);

/** 
\brief Destroys a VO SND instance. CNcomment:������Ƶ���Soundʵ�� CNend
\attention \n
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_Close(HI_UNF_SND_E enSound);

/** 
\brief Enables an AO port. CNcomment:������Ƶ����˿�ʹ�ܿ��� CNend
\attention \n
The analog I2S output port and SPDIF output port can be controlled separately. CNcomment:���Էֱ����ģ��I2S�����SPDIF��� CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] pInterface    Pointer to the AO mode. For details, see the description of ::HI_UNF_SND_INTERFACE_S.
CNcomment:ָ�����ͣ���Ƶ���ģʽ�ṹ�壬��μ�::HI_UNF_SND_INTERFACE_S CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN       No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetInterface(HI_UNF_SND_E enSound, const HI_UNF_SND_INTERFACE_S *pInterface);

/** 
\brief Obtains the enable flag of an AO port. CNcomment:��ȡ��Ƶ����˿�ʹ��״̬ CNend
\attention \n
The enInterface parameter of pInterface is an input parameter, and the bEnable parameter is an output parameter.
CNcomment:�ڶ��������нṹ��ĵ�һ��������enInterface��Ϊ����������ڶ���������bEnable��Ϊ������� CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in/out] pInterface   Pointer to the AO mode. For details, see the description of ::HI_UNF_SND_INTERFACE_S. 
CNcomment:ָ�����͡���Ƶ���ģʽ�ṹ�壬��μ�::HI_UNF_SND_INTERFACE_S CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is disabled. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetInterface(HI_UNF_SND_E enSound, HI_UNF_SND_INTERFACE_S *pInterface);


/** 
\brief Sets the mute status of an analog AO port. CNcomment:��Ƶģ������ľ����������� CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bMute    AO mute enable. HI_TRUE: mute; HI_FALSE: unmute CNcomment:ģ������������ء�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN       No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetAnalogMute(HI_UNF_SND_E enSound, HI_BOOL bMute);

/** 
\brief Obtains the mute status of an analog AO port. CNcomment:��ȡ��Ƶģ������ľ�������״̬ CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pbMute    Pointer to the mute status. HI_TRUE: mute; HI_FALSE: unmute CNcomment:ָ�����͡�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetAnalogMute(HI_UNF_SND_E enSound, HI_BOOL *pbMute);

/** 
\brief Sets the mute status of a digital AO port. CNcomment:��Ƶ��������ľ����������� CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bMute    Digital AO mute enable. HI_TRUE: mute; HI_FALSE: unmute CNcomment:��������������ء�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetDigitalMute(HI_UNF_SND_E enSound, HI_BOOL bMute);

/** 
\brief Obtains the mute status of a digital AO port. CNcomment:��ȡ��Ƶ��������ľ�������״̬ CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pbMute    Pointer to the mute status. HI_TRUE: mute; HI_FALSE: unmute CNcomment:ָ�����͡�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN       No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetDigitalMute(HI_UNF_SND_E enSound, HI_BOOL *pbMute);

/** 
\brief Sets the mute status of the digital and analog AO ports. CNcomment:��Ƶ���ֺ�ģ������ľ��������� CNend
\attention \n
N/A
\param[in] enSound CNcomment:��Ƶ����豸�� CNend
\param[in] bMute CNcomment:��Ƶ���ֺ�ģ������ľ������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetMute(HI_UNF_SND_E enSound, HI_BOOL bMute);

/** 
\brief Obtains the mute status of the digital and analog AO ports. CNcomment:��ȡ��Ƶ���ֺ�ģ������ľ�������״̬ CNend
\attention \n
N/A
\param[in] enSound CNcomment:��Ƶ����豸�� CNend
\param[in] pbMute CNcomment: �������ÿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetMute(HI_UNF_SND_E enSound, HI_BOOL *pbMute);

/** 
\brief Sets the mute status of the DAC for analog audio output. CNcomment:��Ƶģ�������DAC������������ CNend
\attention \n
This API is applicable to the scenario where the analog output audio needs to be muted, but the HDMI audio needs to be retained.
CNcomment:����ģ���������������HDMI��������ĳ�����һ������²���Ҫʹ�� CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bMute    DAC muteHI_TRUE: mute; HI_FALSE: unmute CNcomment:DAC�������ء�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetADACMute(HI_UNF_SND_E enSound, HI_BOOL bMute);

/** 
\brief Obtains the mute status of the DAC for analog audio output. CNcomment:��ȡ��Ƶģ�������DAC�ľ�������״̬ CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pbMute  Pointer typeHI_TRUE: mute; HI_FALSE: unmute CNcomment:ָ�����͡�HI_TRUE��������HI_FALSE���Ǿ��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN       No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetADACMute(HI_UNF_SND_E enSound, HI_BOOL *pbMute);

/** 
\brief Sets the output audio data of the SPDIF interface. CNcomment:����SPDIF ��Ƶ������� CNend
\attention \n
Before calling this API, ensure that the audio decoding mode is set to :HD_DEC_MODE_SIMUL (PCM+transparent decoding mode).\n
For the SPDIF interface, INTERFACE_SPDIF must be enabled no matter what data is output. 
CNcomment:ʹ�øýӿ�ʱ����Ƶ����ģʽ�����趨Ϊ:HD_DEC_MODE_SIMUL(PCM + ͸������ģʽ)\n
ͬʱ������SPDIF�ӿڣ��������ʲô���ݣ�����ʹ��INTERFACE_SPDIF CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bEnable   Transparent data output enable. HI_TRUE: The transparent data is output; HI_FALSE: The PCM data is output.
 CNcomment:͸������������ء�HI_TRUE�����͸�����ݣ�HI_FALSE�����PCM���� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetSpdifPassThrough(HI_UNF_SND_E enSound, HI_BOOL bEnable);

/** 
\brief Sets the pass-through function of the HDMI.
\attention \n
Ensure that the audio decoder works in HD_DEC_MODE_SIMUL mode. CNcomment:ʹ�øýӿ�ʱ����Ƶ����ģʽ�����趨Ϊ:HD_DEC_MODE_SIMUL(PCM + ͸������ģʽ) CNend
\param[in] enSound CNcomment:��Ƶ����豸��  CNend
\param[in] bEnable , HI_TRUE: pass-through enabled; HI_FALSE: pass-through disabled CNcomment:͸������������ء�HI_TRUE��ʹ��HDMI͸�������HI_FALSE���ر�HDMI͸����� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetHdmiPassThrough(HI_UNF_SND_E enSound, HI_BOOL bEnable);

/** 
\brief Sets auto edid function of the HDMI.
\attention \n
Ensure that the audio decoder works in HD_DEC_MODE_SIMUL mode. CNcomment:ʹ�øýӿ�ʱ����Ƶ����ģʽ�����趨Ϊ:HD_DEC_MODE_SIMUL(PCM + ͸������ģʽ) CNend
\param[in] enSound CNcomment:��Ƶ����豸��  CNend
\param[in] bEnable , HI_TRUE: EDID AUTO enabled; HI_FALSE: EDID AUTO disabled CNcomment:�Զ�HDMI EDID������ء�HI_TRUE��ʹ��EDID�Զ�ģʽ��HI_FALSE���ر�HDMI EDID�Զ�ģʽ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetHdmiEdidAutoMode(HI_UNF_SND_E enSound, HI_BOOL bEnable);

/**
\brief Sets the output mode of the HDMI(Auto/PCM/RAW/HBR2LBR).
\attention \n
\param[in] enSound CNcomment:��Ƶ����豸��  CNend
\param[in] enOutPort  Audio OutputPort   CNcomment:��Ƶ����˿� CNend
\param[in] enHdmiMode HDMI mode CNcomment:HDMIģʽCNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_FAILURE FAILURE CNcomment:ʧ�� CNend
\retval ::HI_ERR_AO_SOUND_NOT_OPEN    Sound device is not opened. CNcomment:Sound�豸δ�� CNend
\retval ::HI_ERR_AO_INVALID_ID      The parameter enSound is invalid. CNcomment:��ЧSound ID CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_SetHdmiMode(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_HDMI_MODE_E enHdmiMode);


/** 
\brief Sets the output volume value. CNcomment:����������� CNend
\attention \n
If u32Volume is set to a value greater than 100, the value 100 is used by default. CNcomment:���u32Volume���ô���100��Ĭ�����õ�ֵΪ100 CNend
\param[in] enSound        ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] u32Volume     Volume value, ranging from 0 to 100. 0: mute; 100: maximum volume CNcomment:���õ�����ֵ����Χ0��100��0��������100��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetVolume(HI_UNF_SND_E enSound, HI_U32 u32Volume);

/** 
\brief Sets the output volume value. CNcomment:��������������� CNend
\attention \n
If s32AbsVolume is set to a value greater than 0 dB, the value 0 dB is used by default. CNcomment:���s32AbsVolume���ô��� 0 dB��Ĭ�����õ�ֵΪ0 dB CNend
\param[in] enSound        ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] s32AbsVolume     Volume value, ranging from -70dB to 0 dB -70dB: mute; 0 dB: maximum volume CNcomment:���õ�����ֵ����Χ-70dB~0dB��-70dB��������0dB��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetAbsoluteVolume(HI_UNF_SND_E enSound, HI_S32 s32AbsVolume);

/** 
\brief Obtains the output volume value. CNcomment:��ȡ����������� CNend
\attention \n
The default volume value is 100. CNcomment:��ѯ��Ĭ������ֵΪ0dB CNend
\param[in] enSound         ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] ps32AbsVolume    Pointer to the obtained volume value CNcomment:ָ�����ͣ���ȡ��������ֵ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetAbsoluteVolume(HI_UNF_SND_E enSound, HI_S32 *ps32AbsVolume);

/** 
\brief Obtains the output volume value. CNcomment:��ȡ������� CNend
\attention \n
The default volume value is 100. CNcomment:��ѯ��Ĭ������ֵΪ100 CNend
\param[in] enSound         ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pu32Volume    Pointer to the obtained volume value CNcomment:ָ�����ͣ���ȡ��������ֵ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetVolume(HI_UNF_SND_E enSound, HI_U32 *pu32Volume);

/** 
\brief Sets the sampling rate during audio output. CNcomment:������Ƶ���ʱ�Ĳ����� CNend
\attention \n
At present, the sampling rate cannot be set, and it is fixed at 48 kHz. The streams that are not sampled at 48 kHz are resampled at 48 kHz.
CNcomment:Ŀǰ���������Ĭ��Ϊ48k(ʵ�����������Ϊ48K~192K)��֧�ִ�8K��192K�������룬�����֧��6���ز�����\n
��˵��趨���������Ϊ192Kʱ(ʵ�����������Ϊ192K)����С��32K�����������(��ʱ��Ӱ�����������ʵ������л�)\n
Ȼ�����趨���������Ϊ8Kʱ(ʵ�����������Ϊ8K~192K)����ʱ����ͨ��HDMI���С��32K������(HDMI��֧��) CNend
\param[in] enSound          ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] enSampleRate    Audio sampling rate. For details, see the description of ::HI_UNF_SAMPLE_RATE_E. CNcomment:��Ƶ�����ʡ���μ�::HI_UNF_SAMPLE_RATE_E CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E enSampleRate);

/** 
\brief Obtains the sampling rate during audio output. CNcomment:��ȡ��Ƶ���ʱ�Ĳ����� CNend
\attention \n
The 48 kHz sampling rate is returned by default. CNcomment:�˽ӿ�Ĭ�Ϸ���48kHz������ CNend
\param[in] enSound           ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] penSampleRate   Pointer to the type of the audio sampling rate CNcomment:ָ�����ͣ���Ƶ�����ʵ����� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetSampleRate(HI_UNF_SND_E enSound, HI_UNF_SAMPLE_RATE_E *penSampleRate);

/** 
\brief Enables the AO auto-resampling function. CNcomment:��Ƶ����Զ������ز����������� CNend
\attention \n
By default, auto-resampling is enabled and cannot be disabled.\n CNcomment:Ĭ���Ѿ����ز�����������ر� CNend
At present, the source sampling rates 8 kHz, 16 kHz, 22.05 kHz, 24 kHz, 32 kHz, 44.1 kHz, and 48 kHz are supported. The default value is 48 kHz.\n
If you call this API to set bAutoResample to HI_TRUE, HI_SUCCESS is returned.\n
If you call this API to set bAutoResample to HI_FALSE, the error code HI_UNF_ERR_SND_NOTSUPPORT is returned.
CNcomment:Ŀǰ��֧��Դ������Ϊ8kHz��16kHz��22.05kHz��24kHz��32kHz��44.1kHz��48kHz��Ĭ����48kHz���\n
���ͨ�����ӿ�����bAutoResampleΪHI_TRUE����˽ӿڽ�ֱ�ӷ��سɹ�\n
���ͨ�����ӿ�����bAutoResampleΪHI_FALSE����˽ӿڽ�ֱ�ӷ���HI_UNF_ERR_SND_NOTSUPPORT������ CNend
\param[in] enSound            ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bAutoResample     Auto-resampling enable. HI_TRUE: enabled; HI_FALSE: disabled CNcomment:�Ƿ��Զ��ز�����HI_TRUE�������Զ��ز�����HI_FALSE���������Զ��ز��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetAutoResample(HI_UNF_SND_E enSound, HI_BOOL bAutoResample);

/** 
\brief Obtains the enable status of AO auto-sampling. CNcomment:��ȡ��Ƶ����Զ��ز�������״̬ CNend
\attention \n
N/A
\param[in] enSound              ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pbAutoResample     Pointer to the enable status of auto-sampling CNcomment:ָ�����ͣ��Ƿ��Զ��ز��� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetAutoResample(HI_UNF_SND_E enSound, HI_BOOL *pbAutoResample);

/** 
\brief Sets the smart volume for audio output. CNcomment:��Ƶ��������������������������� CNend
\attention \n
1. The smart volume is disabled by default.\n 
2. The smart volume is valid only for the master audio.\n 
3. The smart volume is enabled only when the program is switched. 
CNcomment:1. Ĭ�Ϲرո���������\n
2. ������������������Ч\n
3. �������������л���Ŀʱ���� CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] bSmartVolume     Smart volume enableHI_TRUE: enabled; HI_FALSE: disabled CNcomment:�Ƿ������������HI_TRUE���򿪣�HI_FALSE���ر� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetSmartVolume(HI_UNF_SND_E enSound, HI_BOOL bSmartVolume);

/** 
\brief Obtains the status of the smart volume for audio output. CNcomment:��ȡ��Ƶ���������������״̬\attention \n CNend
\attention \n
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] pbSmartVolume     Pointer to the enable status of the smart volume CNcomment:ָ�����ͣ��Ƿ���������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetSmartVolume(HI_UNF_SND_E enSound, HI_BOOL *pbSmartVolume);

/** 
\brief Set the AO track mode. CNcomment:������Ƶ�������ģʽ CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] enMode     Audio track mode. For details, see the description of ::HI_UNF_TRACK_MODE_E. CNcomment:��Ƶ����ģʽ����μ�::HI_UNF_TRACK_MODE_E CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetTrackMode(HI_UNF_SND_E enSound, HI_UNF_TRACK_MODE_E enMode);

/** 
\brief Obtains the AO track mode. CNcomment:��ȡ��Ƶ�������ģʽ CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] penMode   Pointer to the AO track mode. For details, see the description of ::HI_UNF_TRACK_MODE_E.
CNcomment:ָ�����ͣ���Ƶ����ģʽ����μ�::HI_UNF_TRACK_MODE_E CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	    No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	    The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetTrackMode(HI_UNF_SND_E enSound, HI_UNF_TRACK_MODE_E *penMode);

/** 
\brief Attaches the SND module to an audio/video player (AVPLAY). CNcomment:����Ƶ���Sound��AV��Audio Video�������� CNend
\attention \n
Before calling this API, you must create a player and ensure that the player has no output. CNcomment:���ô˽ӿ�ǰ�����ȴ�������������Ӧ��·������û����� CNend
\param[in] enSound          ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource         
\param[in] enMixType       Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\param[in] u32MixWeight    Mixing weight, ranging from 0 to 100. 0: minimum value; 100: maximum value CNcomment:Ȩ�أ���ΧΪ0��100��0����Сֵ��100�����ֵ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PORT	        The attach type is invalid. CNcomment:��Ч������ CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_Attach(HI_UNF_SND_E enSound, HI_HANDLE hSource, HI_UNF_SND_MIX_TYPE_E enMixType, HI_U32 u32MixWeight);

/** 
\brief Detaches the SND module from an AVPLAY. CNcomment:���Sound��AV�������� CNend
\attention \n
N/A
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource    Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_Detach(HI_UNF_SND_E enSound, HI_HANDLE hSource);

/** 
\brief Sets the mixing weight of an audio player. CNcomment:������Ƶ����������Ȩ�� CNend
\attention \n
The output volumes of two players are calculated as follows: (volume x weight 1 + volume x weight 2)/100. The formula of calculating the output volumes of multiple players is similar.
CNcomment:������������������ļ��㷽��Ϊ�������õ�����%Ȩ��1+���õ�����%Ȩ��2��/100������������ļ��㷽��������� CNend
\param[in] enSound              ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource    Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\param[in] u32MixWeight    Mixing weight, ranging from 0 to 100. 0: minimum value; 100: maximum value CNcomment:Ȩ�أ���ΧΪ0��100��0����Сֵ��100�����ֵ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetMixWeight(HI_UNF_SND_E enSound, HI_HANDLE hSource, HI_U32 u32MixWeight);

/** 
\brief Obtains the mixing weight of an audio player. CNcomment:��ȡ��Ƶ����������Ȩ�� CNend
\attention \n

\param[in] enSound            ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource    Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\param[in] pu32MixWeight     Pointer to the mixing weight CNcomment:ָ�����ͣ�Ȩ��ֵ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR               The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetMixWeight(HI_UNF_SND_E enSound, HI_HANDLE hSource, HI_U32 *pu32MixWeight);

/** 
\brief Sets the fade in time and fade out time of an audio player. CNcomment:������Ƶ���������뵭��ʱ�� CNend
\attention \n
\param[in] enSound             ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource    Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\param[in] u32FadeinMs         Fade in time (in ms). The maximum value is 2000 ms. CNcomment:����ʱ�䣬��λΪ���룬���ȡֵ2000 CNend
\param[in] u32FadeoutMs        Fade out time (in ms). The maximum value is 500 ms. CNcomment:����ʱ�䣬��λΪ���룬���ȡֵ500 CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetFadeTime(HI_UNF_SND_E enSound, HI_HANDLE hSource, HI_U32 u32FadeinMs, HI_U32 u32FadeoutMs);

/** 
\brief Obtains the fade in time and fade out time of an audio player. CNcomment:��ȡ��Ƶ���������뵭��ʱ�� CNend
\attention \n

\param[in] enSound           ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[in] hSource    Instance handle of an AVPLAY CNcomment:AV����������ʵ����� CNend
\param[in] pu32FadeinMs      Pointer to the fade in time CNcomment:ָ�����ͣ�����ʱ�� CNend
\param[in] pu32FadeoutMs     Pointer to the fade out time CNcomment:ָ�����ͣ�����ʱ�� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_HIAO_DEV_NOT_OPEN	        No audio device is started. CNcomment:Audio�豸δ�� CNend
\retval ::HI_ERR_HIAO_NULL_PTR               The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_HIAO_INVALID_PARA	        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetFadeTime(HI_UNF_SND_E enSound, HI_HANDLE hSource, HI_U32 *pu32FadeinMs, HI_U32 *pu32FadeoutMs);

/** 
\brief Enables a mixer. CNcomment:��һ�������� CNend
\attention \n
A mixer is available only after HI_UNF_SND_Open is called successfully. CNcomment:ֻ��HI_UNF_SND_Open �ɹ��󣬲��ܲ��������豸 CNend
\param[in] enSound     ID of an AO device CNcomment:��Ƶ����豸�� CNend
\param[out] phMixer   Pointer to the handle of the created mixer CNcomment:ָ�����ͣ������Ļ����� ��� CNend
\param[in] pstMixerAttr    Pointer to mixer attributes CNcomment:ָ�����ͣ����������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_INVALID_ID	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Open(HI_UNF_SND_E enSound,HI_HANDLE *phMixer, HI_UNF_MIXER_ATTR_S *pstMixerAttr);

/** 
\brief Disables a mixer. CNcomment:�ر� һ�������� CNend
\attention \n
N/A
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Close(HI_HANDLE hMixer);

/** 
\brief Starts a mixer. CNcomment:���������� CNend
\attention \n
N/A
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Start(HI_HANDLE hMixer);

/** 
\brief Stops a mixer. CNcomment:ֹͣ������ CNend
\attention \n
The data in the buffer will be discarded. CNcomment:���������ݽ��ᶪ�� CNend
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Stop(HI_HANDLE hMixer);

/** 
\brief Pauses a mixer. CNcomment:��ͣ������ CNend
\attention \n
The data in the buffer is retained. CNcomment:������������Ȼ���� CNend
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Pause(HI_HANDLE hMixer);

/** 
\brief  Resets the status of a mixer. CNcomment:������״̬��λ CNend
\attention \n
N/A
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_Flush(HI_HANDLE hMixer);

/** 
\brief Obtains the status of a mixer. CNcomment:��ȡ������״̬��Ϣ CNend
\attention \n
N/A
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\param[out] pstStatus  Mixer handle CNcomment:��������� CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_ERR_SND_NULL_PTR	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_Mixer_GetStatus(HI_HANDLE hMixer, HI_UNF_MIXER_STATUSINFO_S *pstStatus);

/** 
\brief  Transmits data to a mixer. CNcomment:�����ݵ������� CNend
\attention \n
1. If HI_ERR_HIAO_OUT_BUF_FULL is returned, you need to transmit the data that fails to be transmitted last time to ensure the audio continuity.
2. For the PCM data, the restrictions are as follows:
    s32BitPerSample must be set to 16.
    bInterleaved must be set to HI_TRUE. Only interlaced mode is supported.
    u32Channels can be set to 1 or 2.
    u32PtsMs can be ignored.
    ps32PcmBuffer indicates the PCM data pointer.
    ps32BitsBuffer can be ignored.
    u32PcmSamplesPerFrame indicates the number of audio sampling. The data length (in byte) is calculated as follows: u32PcmSamplesPerFrame x u32Channels x sizeof(HI_u16)
    u32BitsBytesPerFrame can be ignored.
    u32FrameIndex can be ignored.
CNcomment:1 �������HI_ERR_HIAO_OUT_BUF_FULL����Ҫ�����߼������ϴ�ʧ�����ݣ����ܱ�֤����������
2 PCM ���ݸ�ʽ�ڻ���������������
    s32BitPerSample: ����Ϊ16
    bInterleaved: ����ΪHI_TRUE, ��֧�ֽ�֯ģʽ
    u32Channels: 1 ��2
    u32PtsMs: ���Ըò���
    ps32PcmBuffer: PCM ����ָ��
    ps32BitsBuffer: ���Ըò���
    u32PcmSamplesPerFrame: ��Ƶ������, ���ݳ���(unit:Bytes): u32PcmSamplesPerFrame*u32Channels*sizeof(HI_u16)
    u32BitsBytesPerFrame: ���Ըò���
    u32FrameIndex: ���Ըò��� CNend
\param[in] hMixer   Mixer handle CNcomment:��������� CNend
\param[out] pstAOFrame   Information about the audio data CNcomment:��Ƶ������Ϣ CNend
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_ERR_SND_NULL_PTR	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_SND_INVALID_PARA	The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_HIAO_OUT_BUF_FULL  Data fails to be transmitted because the mixer buffer is full. CNcomment:������������������������ʧ�� CNend
\see \n
N/A
*/
HI_S32	 HI_UNF_SND_Mixer_SendData(HI_HANDLE hMixer, const HI_UNF_AO_FRAMEINFO_S *pstAOFrame);

/**
\brief Set the attr of Avc. CNcomment:����avc���Ժ���
\attention \n
N/A
\param[in] enSound   sound ID. CNcomment:���� IDֵ
\param[in] pstAvcAttr  attribute of avc. CNcomment:avc ���Բ���
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_SetAvcAttr(HI_UNF_SND_E enSound, const HI_UNF_SND_AVC_ATTR_S* pstAvcAttr);

/**
\brief get the attr of Avc. CNcomment:��ȡavc���Ժ���
\attention \n
N/A
\param[in] enSound   sound ID. CNcomment:���� IDֵ
\param[out] pstAvcAttr  information of avc attribute. CNcomment:avc ���Բ���
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetAvcAttr(HI_UNF_SND_E enSound, HI_UNF_SND_AVC_ATTR_S* pstAvcAttr);

/**
\brief Sets the mute status of  AO ports. CNcomment:��Ƶ��������������� CNend
\attention the function is same of HI_UNF_SND_SetMute when enOutPort is HI_UNF_SND_OUTPUTPORT_ALL.CNcomment:��enOutPortΪHI_UNF_SND_OUTPUTPORT_ALLʱ���˺�������ͬHI_UNF_SND_SetMuteһ��\n
N/A
\param[in] enSound
\param[in] enOutPort CNcomment:sound����˿� CNend
\param[in] bMute
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_FAILURE FAILURE CNcomment:ʧ�� CNend
\retval ::HI_ERR_AO_SOUND_NOT_OPEN    Sound device is not opened. CNcomment:Sound�豸δ�� CNend
\retval ::HI_ERR_AO_INVALID_PARA        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_AO_INVALID_ID      The parameter enSound is invalid. CNcomment:��ЧSound ID CNend
\see \n
N/A
*/
HI_S32   HI_UNF_SND_SetPortMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL bMute);

/**
\brief Obtains the mute status of AO ports. CNcomment:��ȡ��Ƶ����ľ�������״̬ CNend
\attention the function is same of HI_UNF_SND_GetMute when enOutPort is HI_UNF_SND_OUTPUTPORT_ALL.CNcomment:��enOutPortΪHI_UNF_SND_OUTPUTPORT_ALLʱ���˺�������ͬHI_UNF_SND_GetMuteһ��\n
N/A
\param[in] enSound CNcomment:
\param[in] enOutPort CNcomment:sound����˿� CNend
\param[out] pbMute CNcomment:
\retval ::HI_SUCCESS Success CNcomment:�ɹ� CNend
\retval ::HI_FAILURE FAILURE CNcomment:ʧ�� CNend
\retval ::HI_ERR_AO_SOUND_NOT_OPEN    Sound device is not opened. CNcomment:Sound�豸δ�� CNend
\retval ::HI_ERR_AO_NULL_PTR           The pointer is null. CNcomment:ָ�����Ϊ�� CNend
\retval ::HI_ERR_AO_INVALID_PARA        The parameter is invalid. CNcomment:��Ч�Ĳ��� CNend
\retval ::HI_ERR_AO_INVALID_ID      The parameter enSound is invalid. CNcomment:��ЧSound ID CNend
\see \n
N/A
*/
HI_S32 HI_UNF_SND_GetPortMute(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_BOOL *pbMute);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

#endif  /*__HI_UNF_SND_H__*/

