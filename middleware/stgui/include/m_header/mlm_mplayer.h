#ifndef	_MLM_MPLAYER_H_
#define	_MLM_MPLAYER_H_

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mlm_type.h"

typedef enum MMT_MPLAYER_Status
{
	MPLAYER_STATUS_INIT,
	MPLAYER_STATUS_STARTING,
	MPLAYER_STATUS_READY,
	MPLAYER_STATUS_PLAYING,
	MPLAYER_STATUS_BUFFERING,
	MPLAYER_STATUS_PAUSED,
	MPLAYER_STATUS_SCANNING,
	MPLAYER_STATUS_DONE,
	MPLAYER_STATUS_ERROR,
	MPLAYER_STATUS_STOPPED,
	MPLAYER_STATUS_RESERVE
}MMT_MPLAYER_Status_e;

typedef enum MMT_MPLAYER
{
	MPLAYER_EVENT_EOF,/*���ŵ��ļ�β*/
	MPLAYER_EVENT_ERROR,/*�����ڲ�����*/
	MPLAYER_EVENT_RESV,/*����*/
	MPLAYER_EVENT_BUTT/*��Ч���¼�����*/
}MMT_MPLAYER_Event_e;

typedef enum
{
    MPLAYER_TYPE_VIDEO = 0,
    MPLAYER_TYPE_AUDIO,
    MPLAYER_TYPE_PHOTO,
    MPLAYER_TYPE_NUM
} MMT_MPLAYER_Type_e;

typedef enum _media_player_speed
{
    PLAY_SPEED_BACKWARD_32X	= -32,
    PLAY_SPEED_BACKWARD_16X	= -16,
    PLAY_SPEED_BACKWARD_8X	= -8,
    PLAY_SPEED_BACKWARD_4X	= -4,
    PLAY_SPEED_BACKWARD_2X	= -2,
    PLAY_SPEED_0 = 0,
    PLAY_SPEED_1X	        = 1,
    PLAY_SPEED_FORWARD_2X	= 2,
    PLAY_SPEED_FORWARD_4X	= 4,
    PLAY_SPEED_FORWARD_8X	= 8,
    PLAY_SPEED_FORWARD_16X	= 16,
    PLAY_SPEED_FORWARD_32X	= 32
}MEDIA_PLAYER_SPEED_e;


extern MMT_STB_ErrorCode_E MLMF_MPLAYERTerm(void);

/*��������*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_Start(MBT_CHAR *pFileName, MMT_MPLAYER_Type_e ePlayTpye);

/*ֹͣ����*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_Stop(MMT_MPLAYER_Type_e ePlayTpye);

/*��ͣ����*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_Pause(MMT_MPLAYER_Type_e ePlayTpye);

/*�ָ�����*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_Resume(MMT_MPLAYER_Type_e ePlayTpye);

/*����ǰ����λ�ö�λ��ָ��λ��*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_Seek(MBT_U32 uOffsetInMs, MMT_MPLAYER_Type_e ePlayTpye);

/*���ÿ�����˵��ٶ�*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_SetSpeed(MEDIA_PLAYER_SPEED_e eSpeed, MMT_MPLAYER_Type_e ePlayTpye);

/*�õ���ǰ������˵��ٶ�*/
extern MBT_S8 MLMF_MPLAYER_GetSpeed(MMT_MPLAYER_Type_e ePlayTpye);

/*��ȡ����ý��ĳ���ʱ��*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_GetDuration(MBT_U32 *pu32TimeInS, MMT_MPLAYER_Type_e ePlayTpye);

/*��ȡý�嵱ǰ���ŵ�ʱ���*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_GetCurrentTime(MBT_U32 *pTimeInS, MMT_MPLAYER_Type_e ePlayTpye);

/*��ȡ��ǰ��������״̬����֤��ʱ���Ա�����*/
extern MMT_MPLAYER_Status_e MLMF_MPLAYER_GetStatus(MMT_MPLAYER_Type_e ePlayTpye);

typedef MBT_VOID (*MLMF_MPLAYER_CallBack_Fuc)(MMT_MPLAYER_Status_e eEvent, MBT_U32 uParam1);

/*ע���¼��ص�����*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_RegisterEvent(MLMF_MPLAYER_CallBack_Fuc fCallBack);

/*��ע���¼��ص�����*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_UnRegisterEvent(MBT_VOID);

/*�õ�ͼƬ�ĳ�����Ϣ*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_GetPhotoInfo(MBT_U32 *pu32Width,MBT_U32 *pu32Height);

/*��ͼ*/
extern MMT_STB_ErrorCode_E MLMF_MPLAYER_ShowPicture(MBT_U32 x,MBT_U32 y,MBT_U32 u32Width,MBT_U32 u32Height);

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif
