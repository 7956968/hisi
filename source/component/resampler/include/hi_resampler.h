
/******************************************************************************
  Copyright (C), 2004-2014, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_unf_sound.h
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


#ifndef _HI_RESAMPLER_H_
#define _HI_RESAMPLER_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/**
 * \file
 * \brief Describes the information about the rate resample. CNcomment:�ṩ�ز����ӿ� CNend
 */   
    
/*************************** Structure Definition ****************************/
/** \addtogroup      SOUND */
/** @{ */  /** <!--  ��SOUND�� */

typedef void *HResampler;

/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API declaration *****************************/
/** \addtogroup      SOUND */
/** @{ */  /** <!--  ��SOUND�� */

/**
\brief Create resampler. CNcomment:�����ز���ģ�� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] inrate  rate before process .CNcomment:ԭ������ CNend
\param[in] outrate rate after  process .CNcomment:����֮����������� CNend
\param[in] chans   channeds after process .CNcomment:����֮����������� CNend
\retval ::the point of resampler handles. CNcomment: �ز������ָ�� CNend
\see \n
none.CNcomment:�� CNend
*/ 
HResampler HI_Resampler_Create(int inrate, int outrate, int chans);

/**
\brief Create resampler. CNcomment:�����ز���ģ�� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] inst  the point of resampler handles .CNcomment:�ز������ָ��  CNend
\param[in] inbuf the point of input buffer .CNcomment:����buffer��ָ�� CNend
\param[in] insamps   the size of input sample .CNcomment:�������ݲ���������� CNend
\param[in] outbuf the point of output buffer .CNcomment:���buffer��ָ�� CNend
\retval ::the size of output sample. CNcomment: ������ݲ���������� CNend
\see \n
none.CNcomment:�� CNend
*/ 

int  HI_Resampler_Process(HResampler inst, short *inbuf, int insamps, short *outbuf);

/**
\brief Free resampler. CNcomment:�����ز���ģ�� CNend
\attention \n
none. CNcomment:�� CNend
\param[in] inst  the point of resampler handles .CNcomment:�ز������ָ��  CNend
\retval ::None CNcomment: �� CNend
\see \n
none.CNcomment:�� CNend
*/ 

void HI_Resampler_Free(HResampler inst);

/** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */




#endif
