/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_avctr.h
* Description ��Global define for error code base
* 
* Version ��Version ��1.0.0
* Author ��wangyafeng : 
* Date ��2010-08-11
* Record : Change it into standard mode
*
* History :
* (1) 	Version ��1.0.0
* 		Author ��wangyafeng  : 
* 		Date ��2010-08-14
*		Record : Create File 
****************************************************************************/
#ifndef	__MLM_AVCTR_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_AVCTR_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/

/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
typedef enum _m_av_streamtype_e
{
    MM_STREAMTYPE_INVALID =   0,
    /* None : Invalid type                         */
    MM_STREAMTYPE_MP1V    =   1,
    /* Video    : MPEG1                                */
    MM_STREAMTYPE_MP2V    =   2, 
    /* Video    : MPEG2                                */
    MM_STREAMTYPE_MP4V    =   3,
    /* Video    : H264                                 */
    MM_STREAMTYPE_MP1A    =   4, 
    /* Audio    : MPEG 1 Layer I                       */
    MM_STREAMTYPE_MP2A    =   5,
    /* Audio    : MPEG 1 Layer II                      */
    MM_STREAMTYPE_MP4A    =   6, 
    /* Audio    : like HEAAC,Decoder LOAS / LATM - AAC */
    MM_STREAMTYPE_TTXT    =   7, 
    /* Teletext : Teletext pid                         */
    MM_STREAMTYPE_SUBT    =   8, 
    /* Subtitle : Subtitle pid                         */
    MM_STREAMTYPE_PCR     =   9, 
    /* Synchro  : PCR pid                              */
    MM_STREAMTYPE_AC3     =  10, 
    /* Audio    : AC3                                  */
    MM_STREAMTYPE_H264    =  11,
    /* Video    : H264                                 */
    MM_STREAMTYPE_MPEG4P2 =  12,
    /* Video    : MPEG4 Part II                        */
    MM_STREAMTYPE_VC1     =  13, 
    /* Video    : Decode Simple/Main/Advanced profile  */
    MM_STREAMTYPE_AAC     =  14, 
    /* Audio    : Decode ADTS - AAC                    */
    MM_STREAMTYPE_HEAAC   =  15, 
    /* Audio    : Decoder LOAS / LATM - AAC            */
    MM_STREAMTYPE_WMA     =  16, 
    /* Audio    : WMA,WMAPRO                           */
    MM_STREAMTYPE_DDPLUS  =  17, 
    /* Audio    : DD+ Dolby digital                    */
    MM_STREAMTYPE_DTS     =  18,
    /* Audio    : DTS                                  */
    MM_STREAMTYPE_MMV     =  19, 
    /* Video    : Multimedia content                   */
    MM_STREAMTYPE_MMA     =  20,
    /* Audio    : Multimedia content                   */
    MM_STREAMTYPE_AVS     =  21, 
    /* Video    : AVS Video format                     */
    MM_STREAMTYPE_OTHER   = 255  
    /* Misc     : Non identified pid                   */
} MMT_AV_StreamType_E;		

typedef struct _m_av_streamdata_t
{
    MMT_AV_StreamType_E  Type;
    MET_PTI_PID_T        Pid;
} MST_AV_StreamData_T;

typedef enum _m_av_videoscaleparams_e
{
	MM_VIDEO_ASPECT_RATIO_AUTO,
    MM_VIDEO_ASPECT_RATIO_16_9_PILLAR,
    MM_VIDEO_ASPECT_RATIO_16_9_PAN,
    MM_VIDEO_ASPECT_RATIO_4_3_LETTER,
    MM_VIDEO_ASPECT_RATIO_4_3_PAN,
    MM_VIDEO_ASPECT_RATIO_4_3_FULL,
    MM_VIDEO_ASPECT_RATIO_16_9_WIDE
}MMT_AV_VideoAspectRatio_E;

typedef enum _m_av_sdvideomode_e
{
    MM_VIDEO_MODE_AUTO,
    MM_VIDEO_MODE_PAL,
    MM_VIDEO_MODE_NTSC,
}MMT_AV_SDVideoMode_E;

typedef enum _m_av_hdresolution_e
{
    MM_AV_RESOLUTION_AUTO,
    MM_AV_RESOLUTION_AUTO_N,
    MM_AV_RESOLUTION_AUTO_P,
    MM_AV_RESOLUTION_1080p_A,
    MM_AV_RESOLUTION_1080p_N,
    MM_AV_RESOLUTION_1080p_P,
    MM_AV_RESOLUTION_1080i_A,
    MM_AV_RESOLUTION_1080i_N,
    MM_AV_RESOLUTION_1080i_P,
    MM_AV_RESOLUTION_720p_A,
    MM_AV_RESOLUTION_720p_N,//10
    MM_AV_RESOLUTION_720p_P,
    MM_AV_RESOLUTION_576p_P,
    MM_AV_RESOLUTION_576i_P,
    MM_AV_RESOLUTION_480p_N,
    MM_AV_RESOLUTION_480i_N,
}MMT_AV_HDResolution_E;

typedef enum _m_av_audiomedia_tpye_e
{
    MM_AUDIO_MEDIA_TYPE_ES,
    MM_AUDIO_MEDIA_TYPE_PCM,
    MM_AUDIO_MEDIA_TYPE_MP3,
    MM_AUDIO_MEDIA_TYPE_UNKNOW
}MMT_AV_AudioMediaTpye_E;

typedef struct _m_av_media_Pcm_t
{
    MBT_U32 freq;/* ������*/
    MBT_U8 endian;/* �ߵ��ֽ�˳��,1��ʾ���ֽ���ǰ��0��ʾ���ֽ���ǰ*/
    MBT_U8 channel;/*0��ʾ˫ͨ����1��ʾ��ͨ��*/
    MBT_U8 u8DataPrecision;/*���ݵ�����8����16��8��ʾ8λ�����ݣ�16��ʾ16λ������*/
}MST_AV_MediaPcm_T;

typedef enum _m_av_StereoMode_e
{
    MM_AUD_MONO_RIGHT = 1,
    MM_AUD_MONO_LEFT = 2,
    MM_AUD_DUAL_STEREO = 3
}MMT_AV_StereoMode_E;

typedef enum _m_av_voutMode_e
{
    MM_AV_VOUT_AUTO = 0,
    MM_AV_VOUT_YUV = 1,
    MM_AV_VOUT_YC = 2
} MMT_AV_VOutMode_E;


typedef enum _m_av_playrecspeed_e
{
    MM_AV_PLAYSPEED_ID_NONE,
    MM_AV_PLAYSPEED_ID_REV128,/*128���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV64,/*64���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV32,/*32���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV16, /*16���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV08, /*8���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV04, /*4���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV02, /*2���ٿ���*/
    MM_AV_PLAYSPEED_ID_REV01,/*1������*/
    MM_AV_PLAYSPEED_ID_ZERO,	 
    MM_AV_PLAYSPEED_ID_FWD01, /*1���ٽ�*/
    MM_AV_PLAYSPEED_ID_FWD02, /*2���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD04, /*4���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD08, /*8���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD16,/*16���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD32,/*32���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD64,/*64���ٿ��*/
    MM_AV_PLAYSPEED_ID_FWD128,/*128���ٿ��*/
    MM_AV_PLAYSPEED_ID_END_SLOW_REV2,/*1/4��������*/
    MM_AV_PLAYSPEED_ID_SLOW_REV1,/*1/2��������*/
    MM_AV_PLAYSPEED_ID_SLOW_FWD1,/*1/2��������*/
    MM_AV_PLAYSPEED_ID_SLOW_FWD2/*1/4��������*/
}MMT_AV_PlayRecSpeed_E;

typedef enum _m_av_seekflags_e
{
    MM_AV_PLAY_SEEK_SET=0,
    MM_AV_PLAY_SEEK_CUR,
    MM_AV_PLAY_SEEK_END
} MMT_AV_SeekFlags_E;

typedef enum
{
    ///Decode error
    MM_AV_PLAY_DECODE_ERROR   = 0x00000001,
    ///First frame event
    MM_AV_PLAY_FIRST_FRAME    = 0x00000002,
    ///One frame decoded event
    MM_AV_PLAY_DECODE_ONE_FRAME = 0x00000004,
} MMT_AV_Event_e;

/*-----------------------Module Variables-------------------------*/
#define DEFAULT_BRIGHTNESS      50
#define DEFAULT_CONSTRACT       50
#define DEFAULT_SATURATION      50
#define DEFAULT_SHARPNESS       50
#define DEFAULT_HUE             19

#define M_PLAYREC_MAX_PIDS 16
/*-----------------------Module Functions-------------------------*/
/*
*����Ƶ���Žӿ�
*/
/*
*�����Ӳ���ģ����������Ƶ���ŵ�����ֹͣ��PID���¡�
*�κνӿڶ����ܱ����룬�ײ����ʵ�������Ӳ���ģ����ӿڵĻ�����ʣ�
*���ܴ��������ӿ�ͬʱ��ִ�е������
*/

/*
*˵������������Ƶ��
*���������
*			u32PidNumber: pstPidList�а�����PID ������
*			pstPidList: Ҫ���ŵ�PID�б�ÿ��PID�ж�Ӧ��ý�����͡�
*          GetFirstFram:��һ֡����ʱ���ô˻ص�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_Start(MBT_U32 u32PidNumber,MST_AV_StreamData_T * pstPidList,MBT_BOOL bAudioOpen,MBT_VOID(* GetFram)(MMT_AV_Event_e eEvent));

/*
*˵������ʼ��ǰ��Ŀ��¼�ơ�¼�����ɵ��ļ����ֵײ��Զ����ɡ������ļ����ֲ��ɸ����Ѿ����ڵ��ļ���
*���������
*                         pstrMountName��¼�ƽ�Ŀ����ŵ�Ŀ¼��
*                         strFileName:¼�ƽ�Ŀ����ŵ�Ŀ¼�����֡��ڲ���ʱͬ�������Ŀ¼�����ִ����ײ㡣
*			u32PidNumber: pstPidList�а�����PID ������
*			pstPidList: Ҫ���ŵ�PID�б�ÿ��PID�ж�Ӧ��ý�����͡�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/

extern MMT_STB_ErrorCode_E BLSLMF_AV_StartRecPrg(MBT_CHAR * pstrMountName,MBT_CHAR * stringFileDir,MST_AV_StreamData_T * pstPidList,MBT_U32 u32PidNumber);

/*
*˵����������ǰ��Ŀ��¼�ơ�
*���������
*                        �ޡ�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/

extern MMT_STB_ErrorCode_E BLSLMF_AV_StopRecPrg(MBT_VOID);

/*
*˵������ͣ��ǰ��Ŀ��¼�ơ�
*���������
*                        �ޡ�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_PauseRecPrg(MBT_VOID);

/*
*˵�������¿�ʼ¼�Ƶ�ǰ�Ѿ���ͣ¼�ƵĽ�Ŀ��
*���������
*                         �ޡ�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_ResumeRecPrg(MBT_VOID);

/*
*˵��������һ���Ѿ�¼�ƺõĽ�Ŀ�ļ���
*���������
*                         strDirPathName��Ҫ���ŵ��ļ����ڵ�Ŀ¼��
*                         strFileName��Ҫ���ŵ��ļ����֡�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_PlayRecFile(char * strDirPathName,char * strFileName);

/*
*˵����ֹͣ����һ���Ѿ�¼�ƺõĽ�Ŀ�ļ���
*���������
*                         �ޡ�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_StopPlayRecFile(MBT_VOID);


/*
*˵������ȡ��ǰ���ڲ��ŵ��Ѿ�¼�ƵĽ�Ŀ������Ϣ�������ǰû�в���¼�Ƶ����򷵻ش���
*���������
*                         �ޡ�
*�������:
*			pu32StreamTotalTimeS�����ڲ��ŵ������ܵ�ʱ�䡣
*			pu32StreamTotalTimeS�����ڲ��ŵ������Ѿ����ŵ�ʱ�䡣
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_GetCurRecPrgInfo(MBT_U32 *pu32StreamTotalTimeS,MBT_U32 *pu32TimePlayedS);

/*
*˵�������ò��Ų���һ���Ѿ�¼�ƺõĽ�Ŀ�ļ������ʡ�
*���������
*                         stSpeed���������ʡ�
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_PlayRecSetSpeed(MMT_AV_PlayRecSpeed_E stSpeed);

/*
*˵������λ����ʱ�䡣
*���������
*                         u32SeekS������ʱ�䣬��λ���롣
*                         stSeekMode��ʱ����㷽ʽ 
*�������:
*			�ޡ�
*		���أ�
*		        BLSM_NO_ERROR:�ɹ���
*	                 ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E BLSLMF_AV_PlayRecFileSeek( MBT_U32 u32SeekS, MMT_AV_SeekFlags_E stSeekMode);
/*
*˵����ע����Ƶ״̬�ı�ص�������������Ƶ������ʽ���߱ȸı�ʱ���ô˻ص�������֪ͨ�ϲ㡣
*���������
*videoModeChangeCall������Ŀ������ʽ���߱ȷ����仯ʱ���ô˻ص�֪ͨ�ϲ㡣
*�������:
*			�ޡ�
*		���أ�
*		         �ޡ�
*/
extern MBT_VOID MLMF_AV_VideoModeNotify(MBT_VOID (*videoModeChangeCall)(
                                                                                                                                                  MMT_AV_SDVideoMode_E stVideoMode,
                                                                                                                                                  MMT_AV_VideoAspectRatio_E stVideoAspect));

/*
*˵����ֹͣ���ڲ��ŵ�AV��
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ�
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_Stop(MBT_VOID);

/*
*˵���������Ƶframebuffer��
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_VideoClear(MBT_VOID);

/*
*˵��������Ƶ��������ע�����ݣ�����ע������ݡ�
*���������
*			Data: ע������ݡ�
*			Len: Data���ȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_VideoInjectFrame(MBT_U8 *data, MBT_U32 len);

/*
*˵����ֹͣ��Ƶ����ע�롣
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_VideoInjectStop(MBT_VOID);

/*
*˵��������һ���ڴ���Ƶ���ݡ�
*���������
*			MediaType: ����ý�����ݵ����ͣ������뿴MMT_AV_AudioMediaTpye_E��
*			Data: ý�����ݡ�
*			Len: ���ݳ��ȡ�
*Param: ���ŵ���ز����������PCM���������MST_AV_MediaPcm_T�ṹ������ΪMBT_ NULL��
*AudioPlayCompleteCall :��Ƶ���ݲ�������ô˺���֪ͨ�ϲ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_AudioPlayData(
                                                            MMT_AV_AudioMediaTpye_E MediaType,
                                                            MBT_U8* data,
                                                            MBT_U32 len,
                                                            MBT_VOID *param,
                                                            MBT_VOID (*AudioPlayCompleteCall)(MBT_U8* data )
                                                            );

/*
*˵����ֹͣ����һ���ڴ���Ƶ���ݡ�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_AudioPlayDataStop(MBT_VOID);

/*
*˵������ͣ����һ���ڴ���Ƶ���ݡ���MLMF_AV_AudioPlayDataResume�ɶԵ��á�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_AudioPlayDataPause(MBT_VOID);

/*
*˵�������¿�ʼ����һ���ڴ���Ƶ���ݡ���MLMF_AV_AudioPlayDataPause�ɶԵ��á�
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
    


extern MMT_STB_ErrorCode_E MLMF_AV_AudioPlayDataResume(MBT_VOID);

/*
*˵�����ڲ��ı�VIDEO PIDS������£��ı�AUDIO PIDS�������ڶ����԰����������
*���������
*			PidsNum��PID�ĸ�����
*			PLAYREC_StreamData��AUDIO ��PID��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_PlayChangeAudioPids(MBT_U32 PidsNum,MST_AV_StreamData_T *PLAYREC_StreamData);

/*
*��Ƶͼ��ӿ�
*/
/*
*˵����������Ƶ�ĵ�����ʽ��
*���������
*			Mode: ������ʽ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetVideoMode(MMT_AV_SDVideoMode_E mode);

/*
*˵����������Ƶ��������
*���������
*			mode: �������������ģʽ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/

extern MMT_STB_ErrorCode_E MLMF_AV_SetAudioMode(MMT_AV_StereoMode_E mode);
/*
*˵����������Ƶ��λ�ô�С����ԭ��Ƶ�ǻָ�����ǰȫ����״̬��
*���������
*Port_X����ʾ��ͼ���x ���ꡣ
*Port_Y����ʾ��ͼ���y ���ꡣ
*Width����ʾ����Ƶ�����,����ֵΪ0���ʾ�ָ�ȫ����
*Height����ʾ����Ƶ�����,����ֵΪ0���ʾ�ָ�ȫ����
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetVideoWindow(	MBT_S32 Port_X,
																MBT_S32 Port_Y,
																MBT_U32 Width,
																MBT_U32 Height);

/*
*˵��������Ƶ���Ŵ��ڡ����ܴ����ظ��򿪵�������ײ���Ҫ���쵱ǰ״̬������Ѿ�����ֱ�ӷ�����ȷ��
*���������
*�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_OpenVideoWindow (MBT_VOID);


/*
*˵�����ر���Ƶ���Ŵ��ڡ����ܴ����ظ��رյ�������ײ���Ҫ���쵱ǰ״̬������Ѿ��ر���ֱ�ӷ�����ȷ��
*���������
*�ޡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_CloseVideoWindow (MBT_VOID);

/*
*˵����������Ƶ����״̬��
*���������
*			Flag����������� =1��������=0���ָ�������
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ�
*			����ţ�ʧ��
*/

extern MMT_STB_ErrorCode_E MLMF_AV_SetMute(MBT_BOOL bMute);
/*
*˵�������ø�������ӿڵ���ʾģʽ���ֱ��ʣ�,���������ר�ã�����ɰѴ˺����ſա�
*���������
*			stResolution: ��ʾģʽ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetHDOutVideoMode(MMT_AV_HDResolution_E stResolution);

/*
*˵����������Ƶ�ı���ģʽ��16:9����4��3��
*���������
*			Mode: ���õı���ģʽ��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetAspectRatio(MMT_AV_VideoAspectRatio_E mode);

/*
*˵�������õڶ����ģʽ���е�CPU YUV�����S-video������ýţ�
*������Ҫ��������л������������Ҫ������Էſա�
*���������
*			stVoutMode:������͡�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ��
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetAuxVoutMode(MMT_AV_VOutMode_E stVoutMode);


/*
*˵��������ͼ������ȡ�
*���������
*			Brightness: Ҫ���õ�����ֵ(0~255)��
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetBrightness(MBT_U8 Brightness);

/*
*˵��������ͼ��ĶԱȶ�(0~255)��
*���������
*			Contrast: ͼ��ĶԱȶȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetContrast(MBT_U8 Contrast);

/*
*˵��������ͼ��ı��Ͷ�(0~255)��
*���������
*			Saturation: ͼ��ı��Ͷȡ�
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ��
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetSaturation(MBT_U8 Saturation);

/*
*˵��������ͼ��������(0~235)��
*���������
*           Sharpness: ͼ�������ȡ�
*�������:
*           �ޡ�
*       ���أ�
*       MM_NO_ERROR:�ɹ���
*           ����ţ�ʧ��
*/
MMT_STB_ErrorCode_E MLMF_AV_SetSharpness(MBT_U8 Sharpness);

/*
*˵��������ͼ���ɫ��(0~235)��
*���������
*           Hue: ͼ���ɫ����
*�������:
*           �ޡ�
*       ���أ�
*       MM_NO_ERROR:�ɹ���
*           ����ţ�ʧ��
*/
MMT_STB_ErrorCode_E MLMF_AV_SetHue(MBT_U8 Hue);

/*
*˵������ȡ����Ƶ��������״̬�������Ƶ����������Ƶ��������û�ڹ���������0���������һ���������ڹ������򷵻�1��
*���������
*			�ޡ�
*�������:
*			�ޡ�
*		���أ�
*			������״̬��1��ʾ�н������ڹ�����0��ʾ��������û�ڹ���״̬��
*/
extern MBT_U8 MLMF_AV_GetAVState(MBT_VOID);

/*
*˵��������������С��
*���������
*			Volume: ���������������С��0-100����
*�������:
*			�ޡ�
*		���أ�
*		MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_AV_SetVolume(MBT_U32 volume);

/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__ERRORCODE_H__ */

/*----------------------End of mlm_avctr.h-------------------------*/

