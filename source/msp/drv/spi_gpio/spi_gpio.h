#ifndef __SPI_GPIO_H_
#define __SPI_GPIO_H_

/*�������ͨ����*/
#define D_SPI_GPIO_MAX_CH 4

#define     SPI_GPIO_GROUP_NUM          13
#define     SPI_GPIO_BIT_NUM            8

#define     SPI_GPIO_DIR_REG            0x400

#define     SPI_GPIO_OUTPUT             1

typedef struct hiSPI_GPIO_CH_ATTR
{
    HI_BOOL bUsedFlag; /*�Ƿ�ʹ�ã�1-ʹ�ã�0-����*/
    HI_U32  u32SpiCsGpio; /*CS����ʾ�ڼ���GPIO����0��ʼ*/
    HI_U32  u32SpiCsBit; /*��ʾGPIO�еĵڼ�bit����0��ʼ�����7*/
    HI_U32  u32SpiClkGpio; /*ʱ�ӣ���ʾ�ڼ���GPIO����0��ʼ*/
    HI_U32  u32SpiClkBit; /*��ʾGPIO�еĵڼ�bit����0��ʼ�����7*/
    HI_U32  u32SpiDIGpio; /*DI���������ء���ʾ�ڼ���GPIO����0��ʼ*/
    HI_U32  u32SpiDIBit; /*��ʾGPIO�еĵڼ�bit����0��ʼ�����7*/
    HI_U32  u32SpiDOGpio; /*DO,����д������ʾ�ڼ���GPIO����0��ʼ*/
    HI_U32  u32SpiDOBit; /*��ʾGPIO�еĵڼ�bit����0��ʼ�����7*/
}HI_SPI_GPIO_CH_ATTR;

#endif
