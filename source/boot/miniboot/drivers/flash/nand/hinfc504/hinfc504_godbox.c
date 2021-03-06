/******************************************************************************
 *    Copyright (C) 2014 Hisilicon STB Development Dept
 *    All rights reserved.
 * ***
 *    Create by Czyong
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
******************************************************************************/

#include <asm/io.h>
#include <platform.h>

#include "hinfc504.h"

#define HI3712_HINFC600_PERIPHERY_REGBASE           (0x101F5000)
#define HI3712_PERI_CRG30                           (0x00B8)

#define HI3712_PERI_CRG30_CLK_EN                    (1U << 8)
#define HI3712_PERI_CRG30_CLK_SEL_215M              (1U << 1) /* 214.2MHz  */
#define HI3712_NF_SRST_REQ                          (1U << 0)

#define HINFC504_PERIPHERY_REGBASE                  (0x101F5000)

#define PERI_CRG30                                  (0x00B8)
#define PERI_CRG30_CLK_EN                           (1U << 8)
#define PERI_CRG30_CLK_SEL_99M                      (1U << 16)

/******************************************************************************/

void hinfc504_controller_enable(struct hinfc_host *host, int enable)
{
	unsigned long reg_val;
	reg_val = readl(CONFIG_HINFC504_REG_BASE_ADDRESS + HINFC504_VERSION);

	if (HINFC_VER_600 == reg_val) {
		reg_val = readl(HI3712_HINFC600_PERIPHERY_REGBASE + 
				HI3712_PERI_CRG30);
		if (enable)
			reg_val |= (HI3712_PERI_CRG30_CLK_EN 
				| HI3712_PERI_CRG30_CLK_SEL_215M);
		else
			reg_val &= ~HI3712_PERI_CRG30_CLK_EN;

		writel(reg_val, (HI3712_HINFC600_PERIPHERY_REGBASE + 
				HI3712_PERI_CRG30));
	} else {
		reg_val = readl(HINFC504_PERIPHERY_REGBASE + PERI_CRG30);
		if (enable)
			reg_val |= (PERI_CRG30_CLK_EN 
				| PERI_CRG30_CLK_SEL_99M);
		else
			reg_val &= ~PERI_CRG30_CLK_EN;

		writel(reg_val, (HINFC504_PERIPHERY_REGBASE + PERI_CRG30));
	}
}
