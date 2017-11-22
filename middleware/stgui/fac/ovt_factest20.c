/*zero create 20150715*/

/*local header*/
#include "mlm_type.h"
#include "mlm_uart.h"
#include "mlm_msg.h"
#include "mlm_task.h"
#include "mlm_system.h"
#include "mlm_flash.h"
#include "mlm_tuner.h"
#include "mlm_demux.h"
#include "mlm_avctr.h"
#include "mlm_image.h"
#include "mlm_usb.h"
//#include "envopt.h"


#include "nvm_api.h"
/*std C lib header*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
/*sys header*/

typedef enum
{
    OVT_FAC_SUCCESS=0,
    OVT_FAC_ERR
}OVT_FAC_RET;

typedef enum
{
	OVTFAC_UART_0,
	OVTFAC_UART_1
}OVTFAC_UART_t;

typedef enum
{
    TEST_TUNER_HIGFREQ = 0x0A,
    TEST_TUNER_MIDFREQ = 0x09,
    TEST_TUNER_LOWFREQ = 0x01,
    TEST_CVBS = 0x02,
    TEST_YPBPR = 0x03,
    TEST_HDMI = 0x04,
    TEST_MAXVOL = 0x05,
    TEST_2M = 0x0C,
    TEST_2M_SEND = 0x13,
    TEST_2M_RECEIVE = 0x14,
    TEST_NARROWBAND = 0x15,
    TEST_IC = 0x0F,
    TEST_USB = 0x0B,
    TEST_USB2 = 0x2B,
    TEST_ETHERNET = 0x0D,
    TEST_MUTE =0x07, 
    TEST_KEY = 0x0E,
    TEST_LED = 0x11,
    TEST_INFRARED = 0x10,
    TEST_VOLTAGE = 0x19,
    TEST_POWEROFF = 0x12,
    TEST_STBRESET = 0xE3, //����ͨ����֪ͨ�����лָ�����
    TEST_UNKNOWN = 0x99,
    TEST_OVER = 0xFA,
    //����Ϊ2.0�汾�¼�
    TEST_S_TERMINAL = 0x06,
    TEST_WRITE_MAC = 0x16,
    TEST_WRITE_SERIAL = 0x17,
    TEST_ACQ_CARD = 0x18,
    TEST_TOTAL_RESULT = 0x20,
    TEST_VOL_LEFT_MUTE = 0x21,
    TEST_VOL_LEFT_MAX = 0x22,
    TEST_VOL_RIGHT_MUTE = 0x23,
    TEST_VOL_RIGHT_MAX = 0x24,
    TEST_VOL_SPDIF_MUTE = 0x25,
    TEST_VOL_SPDIF_MAX = 0x26,
    //add 20150813
    TEST_RESET_TEST_FLAG = 0x27,
    //add 20150909
    TEST_POLAR_0_v=0x28,//����0V��ѹ
    TEST_POLAR_13_v=0x29,//����13V��ѹ
    TEST_POLAR_18_v=0x2a,//����18V��ѹ

	TEST_HDMI_MAX_VOL=0xe1,
	TEST_HDMI_MIN_VOL=0xdf,
}FACTEST_ITEM;


typedef struct
{
	OVTFAC_UART_t 	uart;
	unsigned char  	*pData;
	unsigned short 	len;
	unsigned short 	ReadLen;
}OVTFAC_UART_IO_t;

typedef struct 
{
    unsigned long MsgType;
    unsigned char *Msg_Data;
    unsigned long Data_long1;
    unsigned long Data_long2;
    unsigned short Data_short1;
    unsigned short Data_short2;
    unsigned char Data_char1;
    unsigned char Data_char2;
    unsigned char Data_char3;
    unsigned char Data_char4;
    int			Data_int8;
} OVTFAC_MSG_t;


typedef struct {
	unsigned short 			timeout;  //����
	unsigned long 			freq;  					//Ƶ��
	unsigned long			symbol;					/*������*/
	unsigned char			qam; 					/*Ƶ��ĵ��Ʒ�ʽ*/
	unsigned char			condition_SignalLevel;	/*�ź�ǿ���Ƿ���Ч*/
	unsigned char			SignalLevel_con;		/*�ź�ǿ��*/
	unsigned char			condition_QualityLevel;	/*�ź������Ƿ���Ч*/
	unsigned char			QualityLevel_con;		/*�ź�����*/
	unsigned char			condition_BER;			/*�������Ƿ���Ч*/
	unsigned int			BER_con;				/*������*/
}FAC_TUNNER_CONDITION;//TUNER ���Բ���

static FAC_TUNNER_CONDITION Tunner_Condition;


typedef struct {
	unsigned char	error_code;
	unsigned int	QualityLevel;
	unsigned int	SignalLevel;
	unsigned int	BER;
	unsigned short	Use_Time;
	unsigned int	reserved;
}FAC_TUNNER_RESULT;

static FAC_TUNNER_RESULT Tunner_Ret;

#define GET_LONG1(x) (unsigned char)(x>>24)
#define GET_LONG2(x) (unsigned char)((x&0x00ff0000)>>16)
#define GET_LONG3(x) (unsigned char)((x&0x0000ff00)>>8)
#define GET_LONG4(x) (unsigned char)(x&0x000000ff)
#define GET_SHORTH(x) (unsigned char)(x>>8)
#define GET_SHORTL(x) (unsigned char)(x&0x00ff)
#define MAKE_SHORT(a,b) ((a<<8)|b)
#define MAKE_LONG(a,b,c,d) ((a<<24)|b<<16|c<<8|d)


/*globle value*/
#define FAC_SERIAL_LEN 32 /*��ǰ�˷��͹��������к�32���ֽ�*/
unsigned char facSerLen=0;
unsigned char facSer[FAC_SERIAL_LEN]={0};/*����ı�Ŀǰ�ĺ����ṹ������ֱ��ʹ��ȫ�ֱ���*/
int msg_id=0;
int check_msg_id=0;
char unsigned chn_flag=0;/*���к�ʶ���·����ַ��ŵ�����Э��*/
int ovt_fac_front_panel_test_ok=0;

/*STB OPERATION:���ǰ����Ƿ���������������success���쳣����err*/
char facFPOK=0;
char facVideoOK=0;/*��Ƶ�����֮����ʾ����ɫ��*/



/*end golble value*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*init platform envioment*/
/*STB PORTING: ��ʼ��USB��һ�������ֱ�ӷ���success*/
static OVT_FAC_RET ovt_fac_pla_usb_init(void)
{
    return OVT_FAC_SUCCESS;
}


void ovt_fac_pla_usb_printf( const char *fmt, ...)
{
	return ;
	int fbBinfile=-1;
	unsigned int iLen = 0;
	static char 	bPtr[200];
	unsigned int	iWriteSize = 0;
	unsigned int	bCloseFile = 1;
	va_list argp;
	char szPathName[250] = "";
	//app_get_usb_root_path(szPathName,200);
	strcat(szPathName,"/usb_printf.txt");

	//fbBinfile = open(szPathName, O_RDWR | O_CREAT | O_APPEND);
	if ( fbBinfile < 0 ) 
	{
		bCloseFile = 0;
		goto EXPORTDB_EXIT;
	}
	memset(bPtr, 0x00, 200);

	if(bPtr == NULL)
	{
		goto EXPORTDB_EXIT;
	}
	
	va_start(argp, fmt);
	vsprintf(bPtr, fmt, argp);
	va_end(argp);
	
	iWriteSize = strlen(bPtr);
	//iLen = write(fbBinfile, bPtr, iWriteSize);
	if(iLen != iWriteSize)
	{
		goto EXPORTDB_EXIT;
	}
	
	EXPORTDB_EXIT:
	if ( bCloseFile == 1 )
	{
	//	close(fbBinfile);
	}
}


/*STB PORTING: ���ڳ�ʼ�����Ұ�ϵͳ�����Ĵ�ӡ����ر�*/
static OVT_FAC_RET ovt_fac_pla_uart_init(void)
{
	static char init=0;
	if(1==init)
	{
		return OVT_FAC_SUCCESS;
	}
	else
	{
		init = 1;
	}
	//mtos_close_printk();
	MLMF_Uart_SetParam(115200,0,0,0);
    return OVT_FAC_SUCCESS;
}


/*STB PORTING: �Ӵ�����������ݣ�����*/
int ovt_fac_pla_uart_read(unsigned char *read_buf,int read_num)
{
	MMT_STB_ErrorCode_E iRet=0;
	MBT_U32 rLen;
	unsigned char *pcharPos = read_buf;

	iRet = MLMF_Uart_Read(pcharPos,read_num,&rLen,1000);
	//MLMF_Print("++ovt_fac_pla_uart_read %d [%x]\n",rLen,read_buf[0]);
    return read_num;
}

/*STB PORTING:������д����;��Ҫ��֤���������ݷ��ͳɹ�*/
int ovt_fac_pla_uart_write(unsigned char *write_buf,int write_num)
{
	MBT_U32 rLen;
	MLMF_Uart_Write(write_buf,write_num,&rLen,3000);
	return rLen;
}


/*STB PORTING:��մ���������������*/
int ovt_fac_pla_uart_clean(void)
{
	MLMF_Uart_Flush();
    return 0;
}

/*STB PORTING:������Ϣ����*/
static OVT_FAC_RET ovt_fac_pla_msg_create(int *msg_id,char *msg_name,int msg_num,int msg_size)
{
	MMT_STB_ErrorCode_E iRet=0;
    iRet = MLMF_Msg_Create(msg_size,msg_num,(MET_Msg_T *)msg_id);
	if(MM_NO_ERROR==iRet)
	{
    	return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB PORTING:������Ϣ*/
static OVT_FAC_RET ovt_fac_pla_msg_send(int msg_id,void *msg_data,int msg_size)
{
	MMT_STB_ErrorCode_E iRet=0;
    iRet = MLMF_Msg_SendMsg(msg_id,msg_data,msg_size,MM_SYS_TIME_INIFIE);
    if(MM_NO_ERROR==iRet)
	{
    	return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB PORTING:������Ϣ*/
static OVT_FAC_RET ovt_fac_pla_msg_receive(int msg_id,void *msg_data,int msg_size,int time_out)
{
	MMT_STB_ErrorCode_E iRet=0;
    iRet = MLMF_Msg_WaitMsg(msg_id,msg_data,msg_size,time_out);
	if(MM_NO_ERROR==iRet)
	{
    	return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB PORTING:��������*/
static OVT_FAC_RET ovt_fac_pla_task_create(char *task_name,int *task_id,void (*entry_func)(void *))
{
	static char taskPri=FAC_MONITOR_PROCESS_PRIORITY;
    MMT_STB_ErrorCode_E iRet=0;
	
    iRet = MLMF_Task_Create(entry_func,NULL,32*1024,taskPri,(MET_Task_T *)task_id,task_name);
    if(MM_NO_ERROR==iRet)
	{
		taskPri++;
    	return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB PORTING:��ʱ����λ����*/
static void ovt_fac_pla_task_delay(unsigned int msec)
{
    MLMF_Task_Sleep(msec);
}

/*STB PORTING:��ǰϵͳʱ�䣬��λ����*/
unsigned long ovt_fac_pla_ticket_time_get(void)
{
    return MLMF_SYS_GetMS();
}


static char const ab_month_name[][4] =
  {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

void fac_strptime(char *__s, struct tm *__timeptr)
{
	/*"%b %d %G %H:%M:%S"  ->  __DATE__ __TIME__ ��ʽ */
	/*Apr 11 2017 11:32:57*/
	int i = 0; 
	char * p = __s;
	for(i = 0; i < 12; i++)
	{
		if(strncmp(p,ab_month_name[i],3) == 0)
		{
			__timeptr->tm_mon = i+1;
			break;
		}
	}

 	p = __s +4;/*ǰ�����ֽ�Ϊ�·���д�����ĸ��ֽ�Ϊ�ո�*/
	if(*p == ' ') /*�������ֻ��һ���ַ����ǵ�����ַ��ǿո�*/
	{
		p++;
	}

	__timeptr->tm_mday = strtol(p,NULL,0);
	while(*p != ' ')
	{
		p++;
	}
	p++; /*�Թ��ո�*/

	__timeptr->tm_year= strtol(p,NULL,0);
	while(*p != ' ')
	{
		p++;
	}
	p++; /*�Թ��ո�*/
	
	if(*p == '0')
	{
		__timeptr->tm_hour= strtol(p + 1,NULL,0);
	}else
	{
		__timeptr->tm_hour= strtol(p,NULL,0);
	}
	p +=3; /*ʱ���붼�������ַ����м���ð��:����*/

	if(*p == '0')
	{
		__timeptr->tm_min= strtol(p + 1,NULL,0);
	}else
	{
		__timeptr->tm_min= strtol(p,NULL,0);
	}
	p +=3;

	if(*p == '0')
	{
		__timeptr->tm_sec= strtol(p + 1,NULL,0);
	}else
	{
		__timeptr->tm_sec= strtol(p,NULL,0);
	}
}

/*
������ΪӲ���汾��
*/
unsigned int ovt_fac_pla_hv_get(void)
{
	unsigned int _ver_value=0;
	char *pBuildTime = NULL;
	char 	date_buf[100] = {0};
	
	pBuildTime = ProjectBuildDateTime();
	
	struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));  
    fac_strptime(pBuildTime,tmp_time);  

	sprintf(date_buf, "%d%02d%02d", tmp_time->tm_year, tmp_time->tm_mon, tmp_time->tm_mday);
	_ver_value = strtol(date_buf,NULL,0);
    free(tmp_time);
	
	return _ver_value;
}

/*
ʱ����Ϊ����汾��
*/
unsigned int ovt_fac_pla_sv_get(void)
{
	unsigned int _ver_value=0;
	char *pBuildTime = NULL;
	char 	time_buf[100] = {0};
	
	pBuildTime = ProjectBuildDateTime();
	
	struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));  
    fac_strptime(pBuildTime,tmp_time);  

	sprintf(time_buf, "%d%02d%02d", tmp_time->tm_hour, tmp_time->tm_min, tmp_time->tm_sec);
	_ver_value = strtol(time_buf,NULL,0);

    free(tmp_time);
	
	return _ver_value;
}

/*STB PORTING:��ȡ��ʱ���кţ�16�ֽڣ���Ч����OVT_FAC_SUCCESS����Ч����OVT_FAC_ERR*/
OVT_FAC_RET ovt_fac_pla_sn_get(unsigned char *sn)
{
	unsigned char tmpBuf[20]={0};
	MMT_STB_ErrorCode_E Error = MM_NO_ERROR;
	
	Error = MLMF_Flash_Read(FACTORY_SN_BASE_ADDR,(MBT_VOID*)tmpBuf,20);
	//MLMF_Print("ovt_fac_pla_sn_get Error=%d tmpBuf[0]=%d\n",Error,tmpBuf[0]);
	if((MM_NO_ERROR == Error) &&(tmpBuf[0]==0x5a))
	{
		memcpy(sn,&tmpBuf[1],16);
		return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB PORTING:������ʱ���кţ�16�ֽڣ���Ч����OVT_FAC_SUCCESS����Ч����OVT_FAC_ERR*/
OVT_FAC_RET ovt_fac_pla_sn_set(unsigned char *sn)
{
	unsigned char tmpBuf[20]={0};
	MMT_STB_ErrorCode_E Error = MM_NO_ERROR;
	
	tmpBuf[0]=0x5a;
	memcpy(&tmpBuf[1],sn,16);
	
	//Error = MLMF_Flash_Write(FACTORY_SN_BASE_ADDR,(MBT_VOID*)tmpBuf,20);
	Error = FLASHF_ForceSaveFile(FACTORY_SN_BASE_ADDR,tmpBuf,20);
	//MLMF_Print("FLASHF_ForceSaveFile Error=%d tmpBuf[0]=%d\n",Error,tmpBuf[0]);
	return OVT_FAC_SUCCESS;
}


/*end init platform envioment*/

/*fac test function*/


/*STB OPERATION:�ڵװ�����Ļ����ʾ���к�,��ʾ�����ַ�����������Ϊ0,1,2,3*/
extern MBT_S32  WGUIF_FNTGetTxtWidth( MBT_S32 iChNum, MBT_CHAR *pcText );
extern MBT_S32 WGUIF_GetFontHeight(MBT_VOID);
extern MBT_VOID WGUIF_SetFontHeight(MBT_S32 s32FontHeight );

extern MBT_S32  WGUIF_FNTSetTxtMiddle(MBT_S32 s32RectX, MBT_S32 s32RectY, MBT_S32 s32RectWidth, MBT_S32 s32RectHeight, MBT_S32 s32FontWidth, MBT_S32 s32FontHeight, MBT_CHAR *pcText, MBT_U32 u32FrontColor);
extern MMT_STB_ErrorCode_E  WGUIF_ReFreshLayer(MBT_VOID);
static OVT_FAC_RET ovt_fac_fn_show_sn(unsigned char sn)
{
	MBT_S32 s32FontWidth = 0;
	MBT_S32 s32FontHeight = 0;
	MBT_CHAR me_DateStr[30] = { 0 };
	sprintf(me_DateStr,"%d",sn);
	s32FontWidth = WGUIF_FNTGetTxtWidth(strlen(me_DateStr), me_DateStr);
	s32FontHeight = WGUIF_GetFontHeight();
	WGUIF_SetFontHeight(56);
	WGUIF_FNTSetTxtMiddle(500,300,280,120,s32FontWidth,56,me_DateStr,0xFFFFFFFF);
	WGUIF_ReFreshLayer();
	WGUIF_SetFontHeight(s32FontHeight);
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:���OSD����ʾ*/
extern MBT_VOID WGUIF_ClsFullScreen(MBT_VOID);
static OVT_FAC_RET ovt_fac_fn_clean_screen(void)
{
	WGUIF_ClsFullScreen();
	WGUIF_ReFreshLayer();
    return OVT_FAC_SUCCESS;
}

static unsigned int HEX2Decimal(unsigned int number)
{
    int arry[8] = {0};
    int i = 0;
    long sum = 0;
    
    arry[0] = (number >> 28) * 10000000;
    arry[1] = (number >> 24 & 0xF) * 1000000;
    arry[2] = (number >> 20 & 0xF) * 100000;
    arry[3] = (number >> 16 & 0xF) * 10000;
    arry[4] = (number >> 12 & 0xF) * 1000;
    arry[5] = (number >> 8 & 0xF) * 100;
    arry[6] = (number >> 4 & 0xF) * 10; 
    arry[7] = (number & 0xF);

    for(i = 0; i < 8; i++)
    {   
        sum += arry[i];
    }   

    return sum;
}

static OVT_FAC_RET ovtFACTUNER_GetLock(MBT_U32 Device, MBT_U32 timeout )
{
	OVT_FAC_RET _OvtErr = OVT_FAC_ERR;
	unsigned long time1 = 0, time2 = 0;
	
	time1=ovt_fac_pla_ticket_time_get();

	time2=ovt_fac_pla_ticket_time_get();
	while (time2 - time1 < timeout)
	{
		time2=ovt_fac_pla_ticket_time_get();
		if (MM_TUNER_LOCKED == MLMF_Tuner_GetLockStatus(Device))
		{
			_OvtErr = OVT_FAC_SUCCESS;
			break;
		}
		ovt_fac_pla_task_delay(100);
	}
	
	return _OvtErr;

}

static void ovt_fac_fn_lockfreq( FAC_TUNNER_CONDITION *Tunner_Condition, FAC_TUNNER_RESULT *Test_Result_info )
{
	unsigned long time1 = 0, time2 = 0;
	MBT_U32 signal_strength=0;
	MBT_U32 signal_quality=0;
	MBT_U32 signal_ber=0;
	
	time1=ovt_fac_pla_ticket_time_get();

	if (MM_NO_ERROR != MLMF_Tuner_Lock(0,Tunner_Condition->freq,Tunner_Condition->symbol,Tunner_Condition->qam,NULL))
	{	
		//��Ƶʧ�ܽ����Ϊ0xFF
		Test_Result_info->error_code = 0xFF;
		time2=ovt_fac_pla_ticket_time_get();
		Test_Result_info->Use_Time = time2 - time1;
		return;
	}

	if(OVT_FAC_SUCCESS == ovtFACTUNER_GetLock(0, Tunner_Condition->timeout))
	{
		Test_Result_info->error_code = 0;
	}
	else
	{
		//��ʱ�����Ϊ0xFE
		Test_Result_info->error_code = 0xFE;
	}
	
	if( MM_NO_ERROR != MLMF_Tuner_GetStatus(0, &signal_strength,&signal_quality, &signal_ber))
	{
		Test_Result_info->QualityLevel = 0;
		Test_Result_info->SignalLevel = 0;
		Test_Result_info->BER = 0;
		time2=ovt_fac_pla_ticket_time_get();
		Test_Result_info->Use_Time = time2 -time1;
		return ;
	}
	else
	{
		Test_Result_info->QualityLevel = signal_strength;
		Test_Result_info->SignalLevel = signal_quality;
		Test_Result_info->BER = signal_ber;
		time2=ovt_fac_pla_ticket_time_get();
		Test_Result_info->Use_Time = time2 -time1;
		return ;
	}


	
}

static OVT_FAC_RET  ovt_fac_fn_ic(void)
{
	static MBT_U8 pbATR[33]; //TR_SMC_ATR_LEN_MAX=33
	MBT_U32 msgATRLen;
	if(MM_NO_ERROR != MLMF_SmartCard_Reset(0, pbATR, &msgATRLen))
	{
		return OVT_FAC_ERR;
	}
	return OVT_FAC_SUCCESS;
}


/*STB OPERATION:����ָ����һ�׽�Ŀ*/
static OVT_FAC_RET ovt_fac_fn_cvbs(void)
{
	MST_AV_StreamData_T PidList[3];

	PidList[0].Type = MM_STREAMTYPE_MP2V;/*other channel v116,a117,p118*/
	PidList[0].Pid = 100;
	PidList[1].Type = MM_STREAMTYPE_MP1A;
	PidList[1].Pid = 101;
	PidList[2].Type = MM_STREAMTYPE_PCR;
	PidList[2].Pid = 102;
	
	MLMF_Tuner_Lock(0,115,6875,MM_TUNER_QAM_64,NULL);//lock freq
	MLMF_AV_Start(3,PidList,MM_TRUE,MM_NULL);//play pro
	//��ʽ�������Ƶ�����ڻص��д򿪣����Բ�����Ҫ�Լ�����Ƶ����
	MLMF_AV_OpenVideoWindow();
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:�����������������*/
static OVT_FAC_RET ovt_fac_fn_left_vol_max(void)
{
	MLMF_AV_SetVolume(100);
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:�����������������*/
static OVT_FAC_RET ovt_fac_fn_right_vol_max(void)
{
	MLMF_AV_SetVolume(100);
    return OVT_FAC_SUCCESS;
}


/*STB OPERATION:������С����*/
static OVT_FAC_RET ovt_fac_fn_vol_min(void)
{
	MLMF_AV_SetMute(MM_TRUE);
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:����ָ����һ�׽�Ŀ*/
static OVT_FAC_RET ovt_fac_fn_hdmi(void)
{
    return OVT_FAC_SUCCESS;
}


/*STB OPERATION:port 3����0V��ѹ*/
static OVT_FAC_RET ovt_fac_fn_polar_0(void)
{
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:port 2����13V��ѹ*/
/*��Ҫ���ݹ���������Ե�ѹ�Ŀ����޸�*/
static OVT_FAC_RET ovt_fac_fn_polar_13(void)
{
	MLMF_Tuner_Lock(0,115,6875,MM_TUNER_QAM_64,NULL);
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:port 2����18V��ѹ*/
/*��Ҫ���ݹ���������Ե�ѹ�Ŀ����޸�*/
static OVT_FAC_RET ovt_fac_fn_polar_18(void)
{
	MLMF_Tuner_Lock(0,115,6875,MM_TUNER_QAM_64,NULL);
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:���USB�˿��Ƿ���������������success���쳣����err*/
static OVT_FAC_RET ovt_fac_fn_usb(void)
{
	MBT_U32 devNum=0;
	devNum = MLMF_USB_GetDevVolNum();
	if(devNum>=1)
	{
		return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}

/*STB OPERATION:���USB�˿�2�Ƿ���������������success���쳣����err*/
static OVT_FAC_RET ovt_fac_fn_usb2(void)
{
	MBT_U32 devNum=0;
	devNum = MLMF_USB_GetDevVolNum();
	if(devNum>=2)
	{
		return OVT_FAC_SUCCESS;
	}
	else
	{
		return OVT_FAC_ERR;
	}
}


static OVT_FAC_RET ovt_fac_fn_fpanel(void)
{
	char tryTime = 30;
	if(facFPOK)
	{
		return OVT_FAC_SUCCESS;
	}
	else
	{
		while(tryTime--)
		{
			ovt_fac_pla_task_delay(1000);
			if(facFPOK)
			{
				return OVT_FAC_SUCCESS;
			}
		}
	}
	return OVT_FAC_ERR;
}


/*STB OPERATION:�ָ��������ã���Ҫ��������Ϊ�������и����ָ����Ҫ�����з��أ���������success���쳣����err*/
static OVT_FAC_RET ovt_fac_fn_default(void)
{
    return OVT_FAC_SUCCESS;
}

/*STB OPERATION:��д���кţ���������success���쳣����err*/
static OVT_FAC_RET ovt_fac_fn_ser_write(void)
{
    return OVT_FAC_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*common function*/
unsigned char ovt_fac_cmm_crc(unsigned char *data,int data_len)
{
    unsigned char xorResult;
    unsigned short i;

    xorResult = data[0];

    for(i=1;i<data_len;i++)
    {
        xorResult = xorResult ^ data[i];
    }
    return xorResult;
}

/*end common function*/

/*fac test cmd*/
static OVT_FAC_RET ovt_fac_cmd_test_begin(void)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};
    unsigned int HwVer,SwVer;

    HwVer = 1;
    SwVer = 1;
    SwVer = ovt_fac_pla_sv_get();
	HwVer = ovt_fac_pla_hv_get();
		
    pPacket.uart = OVTFAC_UART_0;
    pPacket.pData = pPacket_buf;
    pPacket.len = 13;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0x00;
    pPacket.pData[4] = (unsigned char)((HwVer>>24)&0xff);
    pPacket.pData[5] = (unsigned char)((HwVer>>16)&0xff);
    pPacket.pData[6] = (unsigned char)((HwVer>>8)&0xff);
    pPacket.pData[7] = (unsigned char)(HwVer&0xff);
    pPacket.pData[8] = (unsigned char)((SwVer>>24)&0xff);
    pPacket.pData[9] = (unsigned char)((SwVer>>16)&0xff);
    pPacket.pData[10] = (unsigned char)((SwVer>>8)&0xff);
    pPacket.pData[11] = (unsigned char)(SwVer&0xff);
    pPacket.pData[12] = ovt_fac_cmm_crc(pPacket.pData, pPacket.pData[2] - 1);

    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);
    return OVT_FAC_SUCCESS;
} 

static OVT_FAC_RET ovt_fac_cmd_no_sn(void)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 0x16;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0xfe;
    pPacket.pData[4] = 0x00;
    pPacket.pData[0x14] = ovt_fac_cmm_crc(pPacket.pData+5, 15);
    pPacket.pData[0x15] = ovt_fac_cmm_crc(pPacket.pData, pPacket.pData[2] - 1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);

    return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_have_sn(unsigned char *sn)
{
	OVTFAC_UART_IO_t pPacket;
	unsigned char 	pPacket_buf[100] = {0};
	unsigned char i=0;

	memset(pPacket_buf,0,sizeof(pPacket_buf));
	pPacket.pData = pPacket_buf;
	pPacket.len = 0x16;
	pPacket.pData[0] = 0xBB;
	pPacket.pData[1] = 0xA0;
	pPacket.pData[2] = pPacket.len;
	pPacket.pData[3] = 0xfe;
	pPacket.pData[4] = 0x01;

	for(i = 0; i < 16; i++)
	{
		pPacket.pData[i + 5] = sn[i];
	}
	pPacket.pData[0x14] = ovt_fac_cmm_crc(pPacket.pData+5, 15);
	pPacket.pData[0x15] = ovt_fac_cmm_crc(pPacket.pData, pPacket.pData[2] - 1);
	ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);

	return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_get_sn(void)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 6;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0xfc;
    pPacket.pData[4] = 0xfd;
    pPacket.pData[5] = ovt_fac_cmm_crc(pPacket.pData, pPacket.len-1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);

    return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_collect_sn(void)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 5;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0xfb;
    pPacket.pData[4] = ovt_fac_cmm_crc(pPacket.pData, pPacket.len-1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);

    return OVT_FAC_SUCCESS;
}


static OVT_FAC_RET ovt_fac_cmd_stb_ready(void)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 5;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0xe7;
    pPacket.pData[4] = ovt_fac_cmm_crc(pPacket.pData, pPacket.len-1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);

    return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_test_reply(unsigned char test_type)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 6;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0xfc;
    pPacket.pData[4] = test_type;
    pPacket.pData[5] = ovt_fac_cmm_crc(pPacket.pData, pPacket.len-1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);
    return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_capture_begin(unsigned char test_type)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned char 	pPacket_buf[100] = {0};

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;
    pPacket.len = 9;
    pPacket.pData[0] = 0xBB;
    pPacket.pData[1] = 0xA0;
    pPacket.pData[2] = pPacket.len;
    pPacket.pData[3] = 0x48;
    pPacket.pData[4] = test_type;//item type
    pPacket.pData[5] = 0; //err code
    pPacket.pData[6] = 0x3; //use time H
    pPacket.pData[7] = 0xe8;//use time L
    pPacket.pData[8] = ovt_fac_cmm_crc(pPacket.pData, pPacket.len-1);
    ovt_fac_pla_uart_write(pPacket.pData,pPacket.len);
    return OVT_FAC_SUCCESS;
}

static OVT_FAC_RET ovt_fac_cmd_result_reply(unsigned char test_type,unsigned char err_code)
{
    OVTFAC_UART_IO_t pPacket;
    unsigned short length = 0;
    unsigned char 	pPacket_buf[100] = {0};
    unsigned char ret=0;

    memset(pPacket_buf,0,sizeof(pPacket_buf));
    pPacket.pData = pPacket_buf;

    pPacket.pData[length] = 0xBB;
    pPacket.pData[length+1] = 0xA0;
    pPacket.pData[length+2] = 0;

    switch(test_type)
    {
    	case TEST_TUNER_LOWFREQ:
		     {
                length = 3;
                pPacket.pData[length++] = 0x31; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = GET_SHORTH(Tunner_Ret.Use_Time); //5
				pPacket.pData[length++] = GET_SHORTL(Tunner_Ret.Use_Time); //6
				pPacket.pData[length++] = Tunner_Ret.SignalLevel; //7
				pPacket.pData[length++] = Tunner_Ret.QualityLevel; //8
				pPacket.pData[length++] = GET_LONG1(Tunner_Ret.BER); //9
				pPacket.pData[length++] = GET_LONG2(Tunner_Ret.BER); //10
				pPacket.pData[length++] = GET_LONG3(Tunner_Ret.BER); //11 
				pPacket.pData[length++] = GET_LONG4(Tunner_Ret.BER); //12
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  		
            }
            break;
		case TEST_TUNER_MIDFREQ:
			{
                length = 3;
                pPacket.pData[length++] = 0x39; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = GET_SHORTH(Tunner_Ret.Use_Time); //5
				pPacket.pData[length++] = GET_SHORTL(Tunner_Ret.Use_Time); //6
				pPacket.pData[length++] = Tunner_Ret.SignalLevel; //7
				pPacket.pData[length++] = Tunner_Ret.QualityLevel; //8
				pPacket.pData[length++] = GET_LONG1(Tunner_Ret.BER); //9
				pPacket.pData[length++] = GET_LONG2(Tunner_Ret.BER); //10
				pPacket.pData[length++] = GET_LONG3(Tunner_Ret.BER); //11 
				pPacket.pData[length++] = GET_LONG4(Tunner_Ret.BER); //12
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  		
            }
            break;
		case TEST_TUNER_HIGFREQ:
            {
                length = 3;
                pPacket.pData[length++] = 0x3a; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = GET_SHORTH(Tunner_Ret.Use_Time); //5
				pPacket.pData[length++] = GET_SHORTL(Tunner_Ret.Use_Time); //6
				pPacket.pData[length++] = Tunner_Ret.SignalLevel; //7
				pPacket.pData[length++] = Tunner_Ret.QualityLevel; //8
				pPacket.pData[length++] = GET_LONG1(Tunner_Ret.BER); //9
				pPacket.pData[length++] = GET_LONG2(Tunner_Ret.BER); //10
				pPacket.pData[length++] = GET_LONG3(Tunner_Ret.BER); //11 
				pPacket.pData[length++] = GET_LONG4(Tunner_Ret.BER); //12
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  		
            }
            break;
		case TEST_IC:
			{
				length = 3;
				pPacket.pData[length++] = 0x3f; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
				pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length); 
			}
			 break;
        case TEST_CVBS:
            {
                length = 3;
                pPacket.pData[length++] = 0x32; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
        case TEST_POLAR_0_v:
            {
                length = 3;
                pPacket.pData[length++] = 0x58; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
         case TEST_POLAR_13_v:
            {
                length = 3;
                pPacket.pData[length++] = 0x59; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
        case TEST_POLAR_18_v:
            {
                length = 3;
                pPacket.pData[length++] = 0x5a; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
         case TEST_HDMI:
            {
                length = 3;
                pPacket.pData[length++] = 0x34; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
         case TEST_VOL_LEFT_MAX:
            {
                length = 3;
                pPacket.pData[length++] = 0x51; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
		 case TEST_VOL_RIGHT_MAX:
            {
                length = 3;
                pPacket.pData[length++] = 0x52; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
         case TEST_VOL_LEFT_MUTE:
            {
                length = 3;
                pPacket.pData[length++] = 0x54; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
		case TEST_HDMI_MAX_VOL:
			{
                length = 3;
                pPacket.pData[length++] = 0xe2; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
			break;
		case TEST_HDMI_MIN_VOL:
			{
                length = 3;
                pPacket.pData[length++] = 0xe0; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
			break;
         case TEST_USB:
            {
                length = 3;
                pPacket.pData[length++] = 0x3b; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
		 case TEST_USB2:
            {
                length = 3;
                pPacket.pData[length++] = 0x5b; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
            break;
		case TEST_KEY:
			{
                length = 3;
                pPacket.pData[length++] = 0x3e; // 3
                pPacket.pData[length++] = err_code;
                pPacket.pData[length++] = 0x3; // 5
                pPacket.pData[length++] = 0xe8;  // 6
                pPacket.pData[2] = length+1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
            }
			break;
        case TEST_STBRESET:
           {
                 length = 3;
                 pPacket.pData[length++] = 0xe4;
                 pPacket.pData[length++] = err_code;
                 pPacket.pData[2] = length + 1;
                 pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);
           }
        break;
        case TEST_OVER:
            {
                length = 3;
                pPacket.pData[length++] = 0xf2;
                pPacket.pData[2] = length + 1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);
            }
            break;
       case TEST_RESET_TEST_FLAG:
            {
                length = 3;
                pPacket.pData[length++] = 0x57;
                pPacket.pData[length++] = err_code;
                pPacket.pData[2] = length + 1;
                pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);
            }
            break;
		case TEST_WRITE_SERIAL:
			{
				unsigned char i;
				length = 3;
				pPacket.pData[length++] = 0x47; // 3
				pPacket.pData[length++] = 1;//(unsigned char)TOTAL_info->error_code;
				for(i = 0; i < FAC_SERIAL_LEN; i++)
				{
					pPacket.pData[length++] = facSer[i];
				}
				pPacket.pData[2] = length+1;
				pPacket.pData[length] = ovt_fac_cmm_crc(pPacket.pData, length);  
			}
			break;
        default:
            break;
    }
    ret = ovt_fac_pla_uart_write(pPacket.pData, pPacket.pData[2]);
    ovt_fac_pla_usb_printf("ovt_fac_cmd_result_reply ret=%d msgType=%d err=%d len=%d [%x %x %x %x %x %x %x %x %x %x ]\n",ret,test_type,err_code,pPacket.pData[2],pPacket.pData[0],pPacket.pData[1],pPacket.pData[2],pPacket.pData[3],pPacket.pData[4],pPacket.pData[5],pPacket.pData[6],pPacket.pData[7],pPacket.pData[8],pPacket.pData[9]);
    return OVT_FAC_SUCCESS;
}

/*����һ����Ч�����ݰ�ͷ��������Ч����*/
static OVT_FAC_RET ovt_fac_cmd_response_head(unsigned int time_out)
{
    unsigned char 	pPacket_buf[1] = {0};
    unsigned long star_time=0;
    unsigned long cur_time=0;

    star_time = ovt_fac_pla_ticket_time_get();

    while(1)
    {
        if(0==ovt_fac_pla_uart_read(pPacket_buf,1))
        {
            cur_time = ovt_fac_pla_ticket_time_get();
            if((cur_time-star_time)>=time_out)
            {
                ovt_fac_pla_usb_printf("ovt_fac_cmd_response_head time out faile. cur_time=%d star_time=%d time_out=%d\n",cur_time,star_time,time_out);
                return OVT_FAC_ERR;
            }
            else
            {
                ovt_fac_pla_task_delay(50);
                continue;
            }
        }
        if(0xbb==pPacket_buf[0])
        {
            if(0==ovt_fac_pla_uart_read(pPacket_buf,1))
            {
                return OVT_FAC_ERR;
            }
            else if(0xa0==pPacket_buf[0])
            {
                return OVT_FAC_SUCCESS;
            }
        }
        pPacket_buf[0] = 0;
        //ovt_fac_pla_uart_read(pPacket_buf,1);
    }
    return OVT_FAC_ERR;
}

static unsigned char ovt_fac_cmd_response_data_len(void)
{
    unsigned char 	pPacket_buf[1] = {0};

    ovt_fac_pla_uart_read(pPacket_buf,1);
    if(pPacket_buf[0]>=5)
    {
        return pPacket_buf[0];
    }
    else
    {
        return 0;
    }
}

static OVT_FAC_RET ovt_fac_cmd_response_data(unsigned char *buf ,int data_len)
{
    ovt_fac_pla_uart_read(buf,data_len);
    return OVT_FAC_SUCCESS;
}
/*end fac test cmd*/


void ovt_fac_fn_test_task(void *pPara)
{
    OVTFAC_MSG_t			rec_msg;
    OVT_FAC_RET                 ret;

    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task success.\n");
    while(1)
    {
        memset(&rec_msg,0,sizeof(rec_msg));
        if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(msg_id,(void *)&rec_msg,sizeof(rec_msg),1000))
        {
            ovt_fac_pla_task_delay(3000);
            continue;
        }
        ovt_fac_cmd_test_reply(rec_msg.MsgType);//send cmd 6

        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task msg=%d\n",rec_msg.MsgType);
        switch(rec_msg.MsgType)
        {
			case TEST_TUNER_HIGFREQ:
    		case TEST_TUNER_MIDFREQ:
			case TEST_TUNER_LOWFREQ:
				{
					ovt_fac_fn_lockfreq(&Tunner_Condition,&Tunner_Ret);
					ovt_fac_cmd_result_reply(rec_msg.MsgType,Tunner_Ret.error_code);
				}
				break;
			case TEST_IC:
				{
					ret = ovt_fac_fn_ic();
					ovt_fac_cmd_result_reply(TEST_IC,ret);
				}
				break;
            case TEST_CVBS:
                {
                    ovt_fac_fn_cvbs();
                    ovt_fac_pla_task_delay(2000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))//wait forever
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_CVBS capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_CVBS capture  reply=%d.\n",rec_msg.Data_long2);
                    ovt_fac_cmd_result_reply(TEST_CVBS,rec_msg.Data_long2);
                }
                break;
            case TEST_POLAR_0_v:
                {
                    ovt_fac_fn_polar_0();
                    ovt_fac_pla_task_delay(2500);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),3000))//wait forever
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_0_v capture get no reply and try again.\n");
			   ovt_fac_cmd_capture_begin(TEST_POLAR_0_v);
			    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),5000))
			    {
			    	ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_0_v capture get no reply.\n");
                        	continue;
			    }
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_0_v capture  reply=%d.\n",rec_msg.Data_long2);
                    ovt_fac_cmd_result_reply(TEST_POLAR_0_v,rec_msg.Data_long2);
                }
                break;
            case TEST_POLAR_13_v:
                {
                    ovt_fac_fn_polar_13();
                    ovt_fac_pla_task_delay(2500);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),3000))//wait forever
                    {  
                        ovt_fac_pla_usb_printf("TEST_POLAR_13_v TEST_POLAR_13_v capture get no reply. and try aggin\n");
                        ovt_fac_cmd_capture_begin(TEST_POLAR_13_v);
			    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),5000))
			    {
			    	ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_13_v capture get no reply.\n");
                        	continue;
			    }
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_13_v capture  reply=%d.\n",rec_msg.Data_long2);
                    ovt_fac_cmd_result_reply(TEST_POLAR_13_v,rec_msg.Data_long2);
                }
                break;
            case TEST_POLAR_18_v:
                {
                    ovt_fac_fn_polar_18();
                    ovt_fac_pla_task_delay(2500);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),3000))//wait forever
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_18_v capture get no reply.\n");
                         ovt_fac_cmd_capture_begin(TEST_POLAR_18_v);
			    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),5000))
			    {
			    	ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_18_v capture get no reply.\n");
                        	continue;
			    }
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_POLAR_18_v capture  reply=%d.\n",rec_msg.Data_long2);
                    ovt_fac_cmd_result_reply(TEST_POLAR_18_v,rec_msg.Data_long2);
                }
                break;
            case TEST_HDMI:
                {
                    ovt_fac_fn_hdmi();
					//Hdmi����ʱû���л���Ŀ������Ҫ�ȴ�
                    //ovt_fac_pla_task_delay(3000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                   {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI capture  reply=%d.\n",rec_msg.Data_long2);   
                    ovt_fac_cmd_result_reply(TEST_HDMI,rec_msg.Data_long2);
					facVideoOK=1;
                }
                break;
            case TEST_VOL_LEFT_MAX:
                {
                    ovt_fac_fn_left_vol_max();
                    ovt_fac_pla_task_delay(1000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_VOL_LEFT_MAX capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_VOL_LEFT_MAX capture  reply=%d.\n",rec_msg.Data_long2);  
                    ovt_fac_cmd_result_reply(TEST_VOL_LEFT_MAX,rec_msg.Data_long2);
                }
                break;
			 case TEST_VOL_RIGHT_MAX:
                {
                    ovt_fac_fn_right_vol_max();
                    ovt_fac_pla_task_delay(1000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_VOL_RIGHT_MAX capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_VOL_RIGHT_MAX capture  reply=%d.\n",rec_msg.Data_long2);  
                    ovt_fac_cmd_result_reply(TEST_VOL_RIGHT_MAX,rec_msg.Data_long2);
                }
                break;
            case TEST_VOL_LEFT_MUTE:
               {
                    ovt_fac_fn_vol_min();
                    ovt_fac_pla_task_delay(1000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                   {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_MUTE capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_MUTE capture  reply=%d.\n",rec_msg.Data_long2);  
                    ovt_fac_cmd_result_reply(TEST_VOL_LEFT_MUTE,rec_msg.Data_long2);
                }
                break;
			case TEST_HDMI_MAX_VOL:
				{
                    ovt_fac_fn_left_vol_max();
                    ovt_fac_pla_task_delay(1000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                    {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI_MAX_VOL capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI_MAX_VOL capture  reply=%d.\n",rec_msg.Data_long2);  
                    ovt_fac_cmd_result_reply(TEST_HDMI_MAX_VOL,rec_msg.Data_long2);
                }
				break;
			case TEST_HDMI_MIN_VOL:
				{
                    ovt_fac_fn_vol_min();
                    ovt_fac_pla_task_delay(1000);
                    ovt_fac_cmd_capture_begin(rec_msg.MsgType);
                    memset(&rec_msg,0,sizeof(rec_msg));
                    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_receive(check_msg_id,(void *)&rec_msg,sizeof(rec_msg),10000))
                   {  
                        ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI_MIN_VOL capture get no reply.\n");
                        continue;
                    }
                    ovt_fac_pla_usb_printf("ovt_fac_fn_test_task TEST_HDMI_MIN_VOL capture  reply=%d.\n",rec_msg.Data_long2);  
                    ovt_fac_cmd_result_reply(TEST_HDMI_MIN_VOL,rec_msg.Data_long2);
                }
				break;
            case TEST_USB:
                {
                    ret = ovt_fac_fn_usb();
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
                }
                break;
			case TEST_USB2:
                {
                    ret = ovt_fac_fn_usb2();
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
                }
                break;
			case TEST_KEY:
				{
					ret = ovt_fac_fn_fpanel();
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
				}
				break;
             case TEST_STBRESET:
                {
                    ret = ovt_fac_fn_default();
					if(OVT_FAC_SUCCESS == ret)
					{
						//�ָ�����0xff����ɹ���0x00����ʧ��
						ret =0xff;
					}
					else
					{
						ret =0x00;
					}
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
                }
                break;
             case TEST_OVER:
                {
                    ret = OVT_FAC_SUCCESS;
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);

		    		//�����꣬����������޷����ƣ������ַ�ʽ��
		    		#if 0
					MLMF_FP_Display("    ",4,MM_FALSE);          
				    MLMF_FP_SetLock(0);
				    MLMF_AV_CloseVideoWindow();
				    MLMF_FP_SetStandby(1);
				    MLMF_Task_Sleep(500);
					MLMF_SYS_EnterStandby(MM_STANDBY_MODE_NORMAL,0,PHY_POWER_KEY);
				    while(1)
				    {
				        //MLMF_Print("still alive\n");
				        MLMF_Task_Sleep(1000);
				    }
				    MLMF_SYS_WatchDog(1);
				    MLMF_FP_SetStandby(0);
				    //uif_SetSTBRunStatus(MM_TRUE);
				    MLMF_AV_SetVideoWindow(0,0,0,0);
					#endif
                }
                break;
             case TEST_RESET_TEST_FLAG:
                {
                    ret = OVT_FAC_SUCCESS;
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
                }
			 	break;
			 case TEST_WRITE_SERIAL:
			 	{
					ret = ovt_fac_fn_ser_write();
                    ovt_fac_cmd_result_reply(rec_msg.MsgType,ret);
			 	}
			 	break;
             default:
                break;
             
        }
    }
}

void ovt_fac_fn_monitor_task(void *pPara)
{
    unsigned char pre_test_cmd=0;
    unsigned char uart_buf[100];
    OVTFAC_MSG_t	send_msg={0};

	MLMF_Print("ovt_fac_fn_monitor_task success.\n");
    ovt_fac_pla_usb_printf("ovt_fac_fn_monitor_task success.\n");
    while(1)
    {
        if(OVT_FAC_SUCCESS!=ovt_fac_cmd_response_head(1000))
        {
            ovt_fac_pla_task_delay(50);
            continue;
        }
        uart_buf[0]=0xbb;
        uart_buf[1]=0xa0;
        uart_buf[2]=ovt_fac_cmd_response_data_len();

        if(uart_buf[2]<5)
        {
            continue;
        }
        ovt_fac_cmd_response_data(&uart_buf[3],uart_buf[2]-3);

        if(uart_buf[uart_buf[2]-1]!=ovt_fac_cmm_crc(uart_buf,uart_buf[2]-1))
        {
            continue;
        }
        if(pre_test_cmd==uart_buf[3])
        {
            ovt_fac_pla_usb_printf("ovt_fac_fn_monitor_task monitor the same type=%d\n",send_msg.MsgType);
            continue;
        }
        pre_test_cmd = uart_buf[3];

        send_msg.MsgType = uart_buf[3];
        ovt_fac_pla_usb_printf("ovt_fac_fn_monitor_task monitor type=%d\n",send_msg.MsgType);

        switch(send_msg.MsgType)
        {
        	case TEST_WRITE_SERIAL:
				{
					unsigned char i;
					facSerLen = uart_buf[4];
					for(i = 0; i < facSerLen; i++)
					{
						if (i >= FAC_SERIAL_LEN)
						{
							ovt_fac_pla_usb_printf(" TEST_WRITE_SERIAL too long \n");
							break;
						}
						facSer[i]= uart_buf[5+i];
					}
        		}
			case TEST_IC:
            case TEST_CVBS:
            case TEST_POLAR_0_v:
            case TEST_POLAR_13_v:
            case TEST_POLAR_18_v:
            case TEST_HDMI:
            case TEST_VOL_LEFT_MAX:
			case TEST_VOL_RIGHT_MAX:
            case TEST_VOL_LEFT_MUTE:
			case TEST_HDMI_MAX_VOL:
			case TEST_HDMI_MIN_VOL:
            case TEST_USB:
			case TEST_USB2:
			case TEST_KEY:
            case TEST_STBRESET:
            case TEST_OVER:
            case TEST_RESET_TEST_FLAG:
                {
                    ovt_fac_pla_msg_send(msg_id,(void *)&send_msg,sizeof(send_msg));
                }
                break;
            case TEST_ACQ_CARD:
                {
                    send_msg.Data_long1 = uart_buf[4];
                    send_msg.Data_long2 = uart_buf[5];
                    ovt_fac_pla_msg_send(check_msg_id,(void *)&send_msg,sizeof(send_msg));
                }
                break;
			case TEST_TUNER_HIGFREQ:
    		case TEST_TUNER_MIDFREQ:
    		case TEST_TUNER_LOWFREQ:
				{
					Tunner_Condition.timeout= MAKE_SHORT(uart_buf[4], uart_buf[5]);
					Tunner_Condition.freq = HEX2Decimal(MAKE_LONG(uart_buf[6], uart_buf[7], uart_buf[8], uart_buf[9])); //Ƶ��
					Tunner_Condition.symbol= HEX2Decimal(MAKE_LONG(uart_buf[10], uart_buf[11], uart_buf[12], uart_buf[13])); //����Գ				
					Tunner_Condition.qam = uart_buf[14] - 1; //���Ʒ�ʽ
					Tunner_Condition.condition_SignalLevel = uart_buf[15]; //
					Tunner_Condition.SignalLevel_con = uart_buf[16]; //
					Tunner_Condition.condition_QualityLevel = uart_buf[17]; //
					Tunner_Condition.QualityLevel_con = uart_buf[18]; //
					Tunner_Condition.condition_BER = uart_buf[19]; //
					Tunner_Condition.BER_con = MAKE_LONG(uart_buf[20], uart_buf[21], uart_buf[22], uart_buf[23] );
					ovt_fac_pla_msg_send(msg_id,(void *)&send_msg,sizeof(send_msg));
				}
				break;
             default:
                break;
        }
    }
}

void ovt_fac_fn_front_panel_task(void *pPara)
{
	while(1)
	{

	}
}


static OVT_FAC_RET ovt_fac_detect_server(void)
{
    int data_len=0;
    unsigned char uart_buf[64];

    memset(uart_buf,0,sizeof(uart_buf));
    ovt_fac_cmd_test_begin();

    //ovt_fac_pla_task_delay(500);    
    if(OVT_FAC_SUCCESS!=ovt_fac_cmd_response_head(10000))
    {
        ovt_fac_pla_usb_printf("ovt_fac_detect_server head fail.\n");
        return OVT_FAC_ERR;
    }
    data_len = ovt_fac_cmd_response_data_len();
    if(data_len<5)
    {
        ovt_fac_pla_usb_printf("ovt_fac_detect_server data len fail.\n");
        return OVT_FAC_ERR;
    }    
    else
    {
        uart_buf[0]=0xbb;
        uart_buf[1]=0xa0;
        uart_buf[2]=data_len;
        ovt_fac_cmd_response_data(&uart_buf[3],data_len-3);
        if((uart_buf[5]!=ovt_fac_cmm_crc(uart_buf,data_len-1))||uart_buf[3]!=0xff)
        {
            ovt_fac_pla_usb_printf("ovt_fac_detect_server crc fail.\n");
            return OVT_FAC_ERR;
        }
	chn_flag = uart_buf[4];/*�״����ֻ��ͨ����*/
    }
    return OVT_FAC_SUCCESS;
} 

static OVT_FAC_RET ovt_fac_check_sn(void)
{
    int data_len=0;
    unsigned char uart_buf[64];
    unsigned char 	sn_no[17] = {0};
    unsigned char sn_tmp[17]={0};

	if(OVT_FAC_ERR==ovt_fac_pla_sn_get(sn_tmp))
	{
	    ovt_fac_cmd_no_sn();

	    //ovt_fac_pla_task_delay(500);
	    if(OVT_FAC_SUCCESS!=ovt_fac_cmd_response_head(3000))
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_head head 1 fail.\n");
	        return OVT_FAC_ERR;
	    }
	    data_len = ovt_fac_cmd_response_data_len();
	    if(data_len<5)
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data_len data len1 fail.\n");
	        return OVT_FAC_ERR;
	    }    
	    else
	    {
	        uart_buf[0]=0xbb;
	        uart_buf[1]=0xa0;
	        uart_buf[2]=data_len;
	        ovt_fac_cmd_response_data(&uart_buf[3],data_len-3);
	        if(uart_buf[3]!=0xfd)
	        {
	            ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data data 1 fail.\n");
	            return OVT_FAC_ERR;
	        }
	        memcpy(sn_no,&uart_buf[4],16);/*ǰ�˷���17���ֽڣ�ֻ��ʾ1���ֽ�*/
			ovt_fac_pla_sn_set(sn_no);
	    }
	    ovt_fac_cmd_get_sn();
	    ovt_fac_fn_show_sn(chn_flag);
	    ovt_fac_pla_task_delay(1000);//
	    ovt_fac_cmd_collect_sn();
		//ovt_fac_pla_task_delay(7000);//
	    if(OVT_FAC_SUCCESS!=ovt_fac_cmd_response_head(80000))
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_head head fail.\n");
	        return OVT_FAC_ERR;
	    }
	    data_len = ovt_fac_cmd_response_data_len();
	    if(data_len<5)
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data_len data len fail.\n");
	        return OVT_FAC_ERR;
	    } 
	    else
	    {
	        uart_buf[0]=0xbb;
	        uart_buf[1]=0xa0;
	        uart_buf[2]=data_len;
	        ovt_fac_cmd_response_data(&uart_buf[3],data_len-3);
	        if(uart_buf[3]!=0xe8)//cmd 21
	        {
	            ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data data fail.\n");
	            return OVT_FAC_ERR;
	        }
	    }
	    ovt_fac_fn_clean_screen();
	    ovt_fac_cmd_stb_ready();
	    return OVT_FAC_SUCCESS;
	}
	else
	{
	    ovt_fac_fn_show_sn(chn_flag);
	    ovt_fac_pla_task_delay(1000);//
	    ovt_fac_cmd_have_sn(sn_tmp);
	    //ovt_fac_pla_task_delay(7000);//
	    if(OVT_FAC_SUCCESS!=ovt_fac_cmd_response_head(80000))
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_head head fail.\n");
	        return OVT_FAC_ERR;
	    }
	    data_len = ovt_fac_cmd_response_data_len();
	    if(data_len<5)
	    {
	        ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data_len data len fail.\n");
	        return OVT_FAC_ERR;
	    } 
	    else
	    {
	        uart_buf[0]=0xbb;
	        uart_buf[1]=0xa0;
	        uart_buf[2]=data_len;
	        ovt_fac_cmd_response_data(&uart_buf[3],data_len-3);
	        if(uart_buf[3]!=0xe8)//cmd 21
	        {
	            ovt_fac_pla_usb_printf("ovt_fac_cmd_response_data data fail.\n");
	            return OVT_FAC_ERR;
	        }
	    }
	    ovt_fac_fn_clean_screen();
	    ovt_fac_cmd_stb_ready();
	    return OVT_FAC_SUCCESS;
	}
} 



/*end fac test function*/
/*�Զ��������Գ������*/
void ovt_fac_main_init(void)
{
    int monitor_task_id=0;
    int test_task_id=0;
	//int fp_task_id=0;
    ovt_fac_pla_usb_printf("=======begint entern factory auto test============\n");
	
    if(OVT_FAC_SUCCESS!=ovt_fac_pla_usb_init())
    {
        ovt_fac_pla_usb_printf("ovt_fac_pla_usb_init fail.\n");
        return ;
    }
    ovt_fac_pla_usb_printf("ovt_fac_pla_usb_init success.\n");
    if(OVT_FAC_SUCCESS!=ovt_fac_pla_uart_init())
    {
        ovt_fac_pla_usb_printf("ovt_fac_pla_uart_init fail.\n");
        return ;
    }
    ovt_fac_pla_usb_printf("ovt_fac_pla_uart_init success.\n");
    if(OVT_FAC_SUCCESS!=ovt_fac_detect_server())
    {
        ovt_fac_pla_usb_printf("ovt_fac_detect_server fail.\n");
        return ;
    }
    ovt_fac_pla_usb_printf("ovt_fac_detect_server success.\n");
    if(OVT_FAC_SUCCESS!=ovt_fac_check_sn())
    {
        ovt_fac_pla_usb_printf("ovt_fac_check_sn fail.\n");
        return ;
    }
	
    ovt_fac_pla_usb_printf("ovt_fac_check_sn success.\n");
    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_create(&msg_id,"fac_main_msg",10,sizeof(OVTFAC_MSG_t)))
    {
        ovt_fac_pla_usb_printf("ovt_fac_pla_msg_create msg_id create fail.\n");
        return ;
    }
    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_create(&check_msg_id,"fac_result_msg",10,sizeof(OVTFAC_MSG_t)))
    {
        ovt_fac_pla_usb_printf("ovt_fac_pla_msg_create check_msg_id create fail.\n");
        return ;
    }

    ovt_fac_pla_task_create("fac_monitor_task",&monitor_task_id,ovt_fac_fn_monitor_task);
    ovt_fac_pla_task_create("fac_test_task",&test_task_id,ovt_fac_fn_test_task);
	//ovt_fac_pla_task_create("fac_front_panel_task",&fp_task_id,ovt_fac_fn_front_panel_task);
}

/*�����ϻ��������*/
void ovt_fac_aging_test(void)
{
	//uif_AgingTest
}

/*��ʾϵͳ��Ϣ���*/
void ovt_fac_sys_info(void)
{
	//uif_STB_Information
}


/*������ǰ�˶Խ�ǰ������STB��ʵ�ֵ�API�Ƿ�������*/
void ovt_fac_api_test(void)
{
	#if 0 /*test uart read & write*/
	{
		unsigned int sv;
		ovt_fac_pla_sv_get(&sv);
		MLMF_Print("++++sv %d\n",sv);
		unsigned char testBuf[]="lanqi fac test.\n";
		MLMF_Print("test uart \n");
		ovt_fac_pla_uart_init();
		MLMF_Print("test any other output \n");
		ovt_fac_pla_uart_write(testBuf,sizeof(testBuf));
		ovt_fac_pla_task_delay(5000);

		memset(testBuf,0,sizeof(testBuf));
		ovt_fac_pla_uart_read(testBuf,4);
		MLMF_Print("test any other output 2\n");
		ovt_fac_pla_uart_write(testBuf,4);
	}
	#endif

	#if 0/*test save & get tmp sn*/
	{
		MLMF_Print("+++tmp sn test begin.\n");
		OVT_FAC_RET ret;
		unsigned char u8SnBuf[16]={0};
		ret = ovt_fac_pla_sn_get(u8SnBuf);
		if(OVT_FAC_ERR==ret)
		{
			u8SnBuf[0]=0x01;
			u8SnBuf[1]=0x03;
			u8SnBuf[2]=0x05;
			u8SnBuf[3]=0x07;
			ovt_fac_pla_sn_set(u8SnBuf);
			memset(u8SnBuf,0,sizeof(u8SnBuf));
			ret = ovt_fac_pla_sn_get(u8SnBuf);
			MLMF_Print("+++get sn err and set first ret=%d. [%x %x %x %x]\n",ret,u8SnBuf[0],u8SnBuf[1],u8SnBuf[2],u8SnBuf[3]);
		}
		else
		{
			MLMF_Print("+++get sn success ret=%d. [%x %x %x %x]\n",ret,u8SnBuf[0],u8SnBuf[1],u8SnBuf[2],u8SnBuf[3]);
		}
		
	}
	#endif
	
	#if 1/*test clear screen & show sn no */
	{
		
	}
	#endif

	#if 0/*test cvbs & hdmi video & vol*/
	{
		MLMF_Print("+++cvbs & hdmi test begin++.\n");
		ovt_fac_fn_cvbs();
	}
	#endif

	#if 1/* test polar*/
	{
		MLMF_Print("+++polar test begin.\n");
		ovt_fac_fn_polar_13();
		ovt_fac_pla_task_delay(5000);
		ovt_fac_fn_polar_18();
	}
	#endif

	#if 0/*test usb*/
	{
		MLMF_Print("+++usb test begin.\n");
		OVT_FAC_RET ret;
		ret = ovt_fac_fn_usb();
		MLMF_Print("+++usb 0 %d\n",ret);
		ret = ovt_fac_fn_usb2();
		MLMF_Print("+++usb 1 %d\n",ret);
	}
	#endif

	#if 1/*test front panel*/
	{
		
	}
	#endif

	#if 1/*show sys info*/
	{
		
	}
	#endif

	#if 1/*aging test*/
	{
		
	}
	#endif

	#if 0/*test msg & task*/
	{
		MLMF_Print("+++begin test msg & task.\n");
		int monitor_task_id=0;
    	int test_task_id=0;
		if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_create(&msg_id,"fac_main_msg",10,sizeof(OVTFAC_MSG_t)))
	    {
	        MLMF_Print("ovt_fac_pla_msg_create msg_id create fail.\n");
	        return ;
	    }
	    if(OVT_FAC_SUCCESS!=ovt_fac_pla_msg_create(&check_msg_id,"fac_result_msg",10,sizeof(OVTFAC_MSG_t)))
	    {
	        MLMF_Print("ovt_fac_pla_msg_create check_msg_id create fail.\n");
	        return ;
	    }

	    if(OVT_FAC_SUCCESS!=ovt_fac_pla_task_create("fac_monitor_task",&monitor_task_id,ovt_fac_fn_monitor_task))
    	{
    		MLMF_Print("ovt_fac_pla_task_create fac_monitor_task create fail.\n");
			return ;
    	}
	    if(OVT_FAC_SUCCESS!=ovt_fac_pla_task_create("fac_test_task",&test_task_id,ovt_fac_fn_test_task))
    	{
    		MLMF_Print("ovt_fac_pla_task_create fac_test_task create fail.\n");
			return ;
    	}
	}
	#endif
}




