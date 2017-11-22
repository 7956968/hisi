/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_demux.h
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
#ifndef	__MLM_DENUX_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_DENUX_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/
#define MM_MAX_PTI_MASK_LEN 16
/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
typedef	MBT_U16		MET_PTI_PID_T;
typedef	MBT_U32		MET_PTI_FilterHandle_T;
typedef	MBT_U32		MET_DescramblerHandle;


#define MM_PTI_INVALIDHANDLE 0xffffffff
#define MM_PTI_INVALIDPID 0xe000
#define MM_PTI_INVALIDID 0xffff


/*channel ������*/
typedef enum _M_PTI_ChannelType_e
{
    MM_PTI_CHANNEL_PSI,
    MM_PTI_CHANNEL_VIDEO,
    MM_PTI_CHANNEL_AUDIO,
    MM_PTI_CHANNEL_PCR,
    MM_PTI_CHANNEL_TYPE_END
}MMT_PTI_ChannelType_E;

/*������Դ������*/
typedef enum _M_PTI_DataSrc_e
{
    MM_PTI_HW_INJECT,
    MM_PTI_SOFT_INJECT
}MMT_PTI_DataSrc_E;

/*filter���ṹ*/
typedef struct  _M_PTI_MaskData_t
{
    MBT_U8 filter[MM_MAX_PTI_MASK_LEN];
    MBT_U8 mask[MM_MAX_PTI_MASK_LEN];
    MBT_U32	filter_len;
} MST_PTI_MaskData_T;

typedef enum _m_pti_dsmmode_e
{
    MM_PTI_DSM_PES,
    MM_PTI_DSM_TS,
    MM_PTI_DSM_UNKNOWN
} MMT_PTI_DsmMode_E;

/*-----------------------Module Functions-------------------------*/
/*
*˵��������һ���ص��������ݵ���ʱͨ�����ô˻ص�������֪ͨ�ϲ㡣
*������������.���úù���������,ÿ������������,����ûص�����,
*������������.��Ϊ���ݵĽ��պܿ죬�������Ӵ�Ϊ���������Ч�ʣ�
*����һ�����ݿ����Ĺ��̣�����涨�ɵײ�������pbuff���ڴ棬����pDataCallbk������
*pbuff���ϲ�ӹܣ��ײ㲻�ͷ�pbuff���ϲ���������MLMF_PTI_FreeSectionBuffer�ͷš�
*���������
*	pDataCallbk�����յ������Ժ���Ҫ���õĻص��������˻ص��������ڼ��̵�ʱ����
*                                �ڵ��ô˻ص�����ʱ������ش���˵���ϲ�û�нӹܴ����ݣ�
*                               �ײ���Ҫ�����ͷ�buffer���ڴ�
*ִ���꣬��������ײ��������
*�������:
*			�ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
MMT_STB_ErrorCode_E MLMF_PTI_SetNotify(MMT_STB_ErrorCode_E(* pDataCallbk)(MET_PTI_PID_T stCurPid,MBT_U8 * pbuff,MBT_U32 u32Len,MET_PTI_FilterHandle_T stFileterHanel));



/*
*˵������ͨ���ϴ����������ˣ����������ӱ�
*���������
*		cHandle��ͨ�������
*�������:
*			pFilter�������ɹ��󷵻ص�filter����������ɹ��򷵻�MBT_NULL��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_PTI_CreateFilter(MET_PTI_PID_T pid,MBT_U32 u32BufSize,MET_PTI_FilterHandle_T * pFilter,MMT_PTI_DataSrc_E eDataType);

/*
*˵��������һ��filter��
*���������
*		cHandle��ͨ�������
*			vFilter�����������Ҫ���ٵ�filter�����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_PTI_DestroyFilter(MET_PTI_FilterHandle_T vFilter);

/*
*˵��������һ��filter�Ĺ��˲�����ͬʱע�����ݽ��յĻص�����.������������.���úù���������,ÿ������������,����ûص�����,������������.��Ϊ���ݵĽ��պܿ죬�������Ӵ�Ϊ���������Ч�ʣ�����һ�����ݿ����Ĺ��̣�����涨�ɵײ�������pbuff���ڴ棬����pDataCallbk������pbuff���ϲ�ӹܣ��ײ㲻�ͷ�pbuff���ϲ�������ͷš�
*���������
*			vFilter����Ҫ���õ�filter�����
*			MaskData�����˲����ṹ��
*pDataCallbk�����յ������Ժ���Ҫ���õĻص��������˻ص��������ڼ��̵�ʱ����ִ���꣬��������ײ��������
*�������:
*			�ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_PTI_SetFilter(MET_PTI_FilterHandle_T vFilter,MST_PTI_MaskData_T *MaskData);
/*
*˵��������һ��filter�������ݡ�
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_PTI_EnableFilter(MET_PTI_FilterHandle_T vFilter);

/*
*˵����ֹͣһ���������ݵ�filter��
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_PTI_DisableFilter(MET_PTI_FilterHandle_T vFilter);

/*
*˵�����ͷ���pDataCallbk�ص�������������pbuff�ڴ档
*���������
*			ChannelHandle��channel�ľ����
*			vFilter��filter�ľ����
*pbuff��Section���ݵ��ڴ�ָ�롣
*�������:
*			�ޡ�
*		����
*			MM_NO_ERROR:�ɹ���
*			�����:ʧ�ܡ�
*/
extern MBT_VOID MLMF_PTI_FreeSectionBuffer (MET_PTI_FilterHandle_T vFilter,MBT_U8 *pbuff);

/*
*˵��:����һ����������������ָ���Ľ���ģʽ������
*���������
*	dsmHandle�������������
*	Encryptmode������ģʽ��
*�������:
*			�ޡ�
*	���أ�
*		MM_NO_ERROR:�ɹ���
*		����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerAllocate(MET_DescramblerHandle *dsmHandle, MMT_PTI_DsmMode_E encryptmode);

/*
*˵�����ͷ�ָ������Ľ�����ͨ����
*���������
*	dsmHandle�������������
*�������:
*			�ޡ�
*	����
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerFree(MET_DescramblerHandle dsmHandle);

/*
*˵��:Ϊָ������Ľ�����ͨ����һ��pid��
*���������
*dsmHandle�������������
*pid�������ŵ�����pid��
*�������:
*			�ޡ�
*���أ�
*	MM_NO_ERROR:�ɹ���
*����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetPid(MET_DescramblerHandle dsmHandle, MET_PTI_PID_T pid);

/*
*˵��:Ϊָ������Ľ�����ͨ��,����������֡�
*���������
*dsmHandle�������������
*key�������֡�
*keyLen�������ֳ��ȡ�
*�������:
*			�ޡ�
*���أ�
*MM_NO_ERROR���ɹ���
*����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetOddKey(MET_DescramblerHandle dsmHandle,MBT_U8 *key, MBT_S32 keyLen);

/*
*˵����Ϊָ������Ľ�����ͨ��,����ż�����֡�
*���������
*	dsmHandle�������������
*	key�������֡�
*	keyLen�������ֳ��ȡ�
*�������:
*			�ޡ�
*	���أ�
*		MM_NO_ERROR:�ɹ���
*	����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E  MLMF_PTI_DescramblerSetEvenKey(MET_DescramblerHandle dsmHandle,MBT_U8 *key, MBT_S32 keyLen);


extern MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedDescramblerSetKeys(MET_DescramblerHandle dsmHandle,MBT_U8 *OddKey,MBT_U8 *EvenKey,MBT_S32 keyLen);

extern MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSelectRootKey(MBT_U8 u8RootKeyID);

extern MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetContentKey(MBT_U32 u32KeyLen,MBT_U8 *pu8ContentKey);

extern MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetCommonKey(MBT_U16 u16Algorithm,MBT_U8 *pu8CommonKey);

extern MMT_STB_ErrorCode_E  MLMF_PTI_AdvancedSetTempSessionKey(MBT_U16 u16Algorithm,MBT_U8 *pu8EncrypedTSK);

/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_DENUX_H__ */

/*----------------------End of mlm_demux.h-------------------------*/

