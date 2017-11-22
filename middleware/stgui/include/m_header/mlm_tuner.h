/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_tuner.h
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
* 		Date ��2010-08-11
*		Record : Create File 
****************************************************************************/
#ifndef	__MLM_TUNER_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_TUNER_H__

/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/

/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
typedef enum _M_TunerStatus_e
{
	MM_TUNER_UNLOCKED,//δ����
	MM_TUNER_LOCKED//������
}MMT_TUNER_TunerStatus_E;


typedef enum _M_TpQamType_e
{
	MM_TUNER_QAM_16 = 0,
	MM_TUNER_QAM_32,
	MM_TUNER_QAM_64,
	MM_TUNER_QAM_128,
	MM_TUNER_QAM_256,
    MM_TUNER_QAM_MAX
}MMT_TUNER_QamType_E;




/*-----------------------Module Functions-------------------------*/
/*
*˵������һ��Ƶ�㣬Ϊ������ʽ���ã��˺���Ӧ�ڱ�֤�µ�Ƶ����������tuner�����뱣֤�˺��������󲻻��ٽ��յ���һ��Ƶ����κ�����������ǰ���·��أ����صȴ���Ƶ�������Ƶ���Ӧ��ͨ�����ûص���������֪�ϲ㡣
*���������
*u32TunerID: ���������tuner��ID �����ڶ��tuner����������ֻ��һ·tuner����0��
*u32Frenq�������������Ƶ�ʣ���λΪKHz��
*u32Sym������������Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875��
*stQam������������ǵ��Ʒ�ʽ��
*MBT_VOID (*TUNER_ScanCallback_T)(
*MMT_TUNER_TunerStatus_E stTunerState, 
*MBT_U32 u32TunerID��
*MBT_U32 iFrenq, 
*MBT_U32 iSym, 
*MMT_TUNER_QAMMode_E stQam)��Ϊ״̬���ص�����������������һ�ֳ����Ժ���ô˺������ص�ǰ״̬��ƽʱ����Ҫ��tuner״̬���м�أ�ÿ�������ӿ���һ�Σ��������ǰ״̬Ϊ����״̬�����κβ����������ǰ״̬Ϊδ����״̬����ô˺�����֪�ϲ�ͬʱ��������������Ƶ���˻ص�������ÿ�ε�����Ƶ����MLMF_Tuner_Lockʱ���п��ܸ�����������������£�
*u32TunerID: �������������MLMF_Tuner_Lock����ĵ�ID��
*u32Frenq�������������Ƶ�ʣ���λΪKHz������MLMF_Tuner_Lock����ĵ�Ƶ�ʡ�
*u32Sym������������Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875������MLMF_Tuner_Lock����ĵķ����ʡ�
*stQam������������ǵ��Ʒ�ʽ������MLMF_Tuner_Lock����ĵ��Ʒ�ʽ��
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Tuner_Lock(MBT_U32 u32TunerID, 
                                                MBT_U32 u32Frenq, 
                                                MBT_U32 u32Sym, 
                                                MMT_TUNER_QamType_E eQamType,
                                                MBT_VOID (*TUNER_ScanCallback_T)(
                                                                                    MMT_TUNER_TunerStatus_E stTunerState, 
                                                                                    MBT_U32 u32TunerID,
                                                                                    MBT_U32 u32Frenq, 
                                                                                    MBT_U32 u32Sym, 
                                                                                    MMT_TUNER_QamType_E eQamType)
                                                    );


/*
*˵������ȡtuner��״̬���������Ҫȡ��صĲ�����ֱ�Ӵ�MBT_NULL��
*���������
*			u32TunerID�����������tuner��ID��
*�������:
*			�ޡ�			
*		���أ�
*			����״̬����
*/                                                                                                                                            
extern MMT_TUNER_TunerStatus_E MLMF_Tuner_GetLockStatus(MBT_U32 u32TunerID);

/*
*˵������ȡtuner��״̬���������Ҫȡ��صĲ�����ֱ�Ӵ�MBT_NULL��
*���������
*			u32TunerID�����������tuner��ID��
*�������:
*			signal_ber����������� Ƶ���������, Bit error rate (x 10000000)��Ϊһ����������Ԫ�ص��������顣
*			Signal_berָ��ǰTUNER �����ʵ�ָ�롣��ָ��ָ��һ����������Ԫ�ص����飬����Ԫ�غ�������:
*				 signal_ber[0]:�����ʵ�������������
*				 signal_ber[1]:�����ʵ�����С�����ֳ���1000
*				 signal_ber[2]:������ָ������ȡ����ֵ(���ֵΪ����������Ϊ�����ʲ����ܴ���1) ����:������Ϊ2.156E-7��
*				 ��ô����Ԫ�ص�ȡֵ�ֱ�Ϊ 2��156��7��
*			signal_quality������������ź����� , Carrier to noise ratio (dB x 10)��
*			signal_strength������������ź�ǿ��, Power of the RF signal (dBm x 10)��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Tuner_GetStatus(MBT_U32 u32TunerID, MBT_U32 *signal_strength,MBT_U32 *signal_quality, MBT_U32 *signal_ber);


/*
*˵������ȡ��ǰƵ����Ϣ��
*���������
*			�ޡ�
*�������:
*u32TunerID: ���������tuner��ID �����ڶ��tuner����������ֻ��һ·tuner����0��
*u32Frenq�������������Ƶ�ʣ���λΪKHz��
*u32Sym������������Ƿ����ʣ���λ��Mbds��ͨ��Ϊ6875��
*stQam������������ǵ��Ʒ�ʽ��
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_Tuner_CurTrans(MBT_U32 u32TunerID, MBT_U32 * pu32Frenq, MBT_U32 * pu32Sym, MMT_TUNER_QamType_E* peQamType);


/*
u8Mute == 1  �ص����� 
u8Mute == 0  �򿪻��� 
����Ĭ���ǹص��ģ�����û������˴򿪣���Ҫ�Գ�ʼ��ʱ��
*/

extern MBT_VOID MLMF_Tuner_SetLoopOutMode(MBT_U8 u8Mute);


/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_TUNER_H__ */

/*----------------------End of mlm_tuner.h-------------------------*/

