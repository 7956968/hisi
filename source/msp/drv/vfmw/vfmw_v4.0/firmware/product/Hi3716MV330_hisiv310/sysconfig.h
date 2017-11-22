
#ifndef __VFMW_SYSCONFIG_HEADER__
#define __VFMW_SYSCONFIG_HEADER__

//#define TWO_VDH

/* ����֧�ֵ�ͨ����Ŀ */
#ifdef CFG_MAX_CHAN_NUM
#define MAX_CHAN_NUM CFG_MAX_CHAN_NUM
#else
#define MAX_CHAN_NUM 32
#endif

#define VDH_ID (0)

/*��֧�ֵ�VDH����*/
#if defined(TWO_VDH)
#define MAX_VDH_NUM 2
#else
#define MAX_VDH_NUM 1
#endif

#define HI_SOC_REG_BASE_TDE         (0x10120000L)
#define HI_SOC_REG_BASE_VDH         (0x10130000L)
#define HI_SOC_REG_BASE_DNR         (0x10450000L)
#define HI_SOC_REG_BASE_SCD         (0x1013c000L)
#define HI_SOC_REG_BASE_BPD         (0x1013d000L)
#define HI_SOC_REG_BASE_CRG    		(0x101F5000L)



/* ϵͳ���ƼĴ�����ַ��λ�� */
//#define SCD_RESET_REG_PHY_ADDR   0x0   //scd
/* Ӳ������IP�ӿڼĴ�����ַ */
//#define VDM_REG_PHY_ADDR       0xff320000    // VDM0 �Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
//#define VDM_REG_PHY_ADDR_1     0xff320000    // VDM1�Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
//#define DNR_REG_PHY_ADDR       0x10450000    //DNR

#define VDM_REG_PHY_ADDR       HI_SOC_REG_BASE_VDH //0xf8c30000    // VDM0 �Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
#define VDM_REG_PHY_ADDR_1     HI_SOC_REG_BASE_VDH //0xf8c30000    // VDM1�Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
#define DNR_REG_PHY_ADDR       HI_SOC_REG_BASE_DNR    //DNR


#define DNR_RESET_REG_PHY_ADDR  (HI_SOC_REG_BASE_CRG + 0x70) //(HI_SOC_REG_BASE_CRG+0x64)//0x101f5064   // DNR
#define DNR_RESET_REG_CTRL_BIT  1

#define SCD_REG_PHY_ADDR       HI_SOC_REG_BASE_SCD    // scd �Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
#define SCD_REG_PHY_ADDR_1     HI_SOC_REG_BASE_SCD    // scd �Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ

#define SCD_RESET_REG_PHY_ADDR  (HI_SOC_REG_BASE_CRG+0x78)    //scd
#define SCD_RESET_REG_CTRL_BIT  5  //  1

#define SCD_RESET_OK_REG_PHY_ADDR   (HI_SOC_REG_BASE_CRG+0x174)  // reset_ok//3716cv200
#define SCD_RESET_OK_REG_CTRL_BIT  1 //3716cv200

#define VDM_RESET_REG_PHY_ADDR  (HI_SOC_REG_BASE_CRG+0x78)   // mfd_rst_reg//3716cv200
#define VDM_RESET_REG_CTRL_BIT  6 //3716cv200

#define VDM_RESET_OK_REG_PHY_ADDR  (HI_SOC_REG_BASE_CRG+0x174)   // reset_ok//3716cv200
#define VDM_RESET_OK_REG_CTRL_BIT  2 //3716cv200

#define VDM_RESET_REG_PHY_ADDR_1  (HI_SOC_REG_BASE_CRG+0x78)   // VDM1
#define VDM_RESET_REG_CTRL_BIT_1  4

//#define FOD_REG_PHY_ADDR       0x10150000    // fod �Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ
#define BPD_REG_PHY_ADDR       HI_SOC_REG_BASE_BPD //0xf8c60000
//#define BTL_REG_PHY_ADDR       0xf8d00000   //0x10450000 // 0x101b0000   //BTL�Ĵ���ӳ�䵽ARM�ռ�ĵ�ַ����ʼ��ַ����
#define SYSTEM_REG_RANGE       (1024*1024)

/* �жϺ� */
#define VDM_INT_NUM            (38+32)
#define VDM_INT_NUM_1          (38+32)
#define SCD_INT_NUM            (41+32)
#define BPD_INT_NUM            (39+32)
#define DNR_INT_NUM            (61+32)
//#define BTL_INT_NUM            (103+32)
/* ���Ե����Ͽɱ���Ƶ����ʹ�õ�memoryԤ�� */
#define BOARD_MEM_BASE_ADDR    0x88000000
//#define BOARD_MEM_BASE_ADDR    0x08000000
#define BOARD_MEM_TOTAL_SIZE   (72*1024*1024)

#endif
