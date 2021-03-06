
/***********************************************************************************
 *
 *	Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
 *
 *	This program is confidential and proprietary to Hisilicon Technologies Co., Ltd.
 *	(Hisilicon), and may not be copied, reproduced, modified, disclosed to
 *	others, published or used, in whole or in part, without the express prior
 *	written permission of Hisilicon.
 *
 * FileName: alps.h
 * Description: 
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16   w54542    NULL                Create this file.
 ***********************************************************************************/
#ifndef __TMX7070X_H__
#define __TMX7070X_H__

#include "hi_type.h"

#define TMX7070X_RF_MIN 51000
#define TMX7070X_RF_MAX 864000  //kHz

#define TMX7070X_STEPS_DEF    62500 //Hz
#define TMX7070X_IF_DEF 36125   //kHz

extern HI_S32 tmx7070x_set_tuner(HI_U32 u32TunerPort, HI_UNF_TUNER_I2cChannel_E enI2cChannel, HI_U32 puRF);

#endif

