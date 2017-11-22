#ifndef  BROWSER_HDI_H
#define  BROWSER_HDI_H

#define  BROWSER_SWITCH_FLAG
#define  BROWSER_NAVIGATOR_FLAG
#define  BROWSER_AD_FLAG

#ifdef  BROWSER_SWITCH_FLAG

/*++++++++++++++++++++BROWSER PORTING INTERFACE HEADER FILE (V 4.1)++++++++++++++++++++++++*/
extern void MLMF_Print(const char* fmt, ...);

/*-------------------------------------������������---------------------------------------*/
typedef  unsigned char 	 BROWSER_U8;
typedef  unsigned short	 BROWSER_U16;
typedef  unsigned long		 BROWSER_U32;
typedef  signed char 	 	 BROWSER_I8;
typedef  signed short	 	 BROWSER_I16;
typedef  signed long		 BROWSER_I32;

typedef  enum
{
	BROWSER_FALSE,
	BROWSER_TRUE
}BROWSER_BOOLEAN_T;

typedef enum
{
	BROWSER_STRING_LEFT_ALIGN,						/*�ı������*/
	BROWSER_STRING_RIGHT_ALIGN,						/*�ı��Ҷ���*/
	BROWSER_STRING_CENTER_ALIGN,					/*�ı����ж���*/
	BROWSER_STRING_ALIGN_NUM
}BROWSER_STRING_ALIGN_MODE_T;

typedef enum
{
	BROWSER_STRING_DYNAMIC_NONE,					/*�ı�������ʾ*/	
	BROWSER_STRING_DYNAMIC_HSCROLL,				/*�ı����ҵ��������ʾ*/	
	BROWSER_STRING_DYNAMIC_VSCROLL,					/*�ı����µ��Ϻ���������ʾ*/	
	BROWSER_STRING_DYNAMIC_VSCROLL2,				/*�ı����µ��������ı�������Ծ������ʾ*/	
	BROWSER_STRING_DYNAMIC_NUM
}BROWSER_STRING_DYNAMIC_MODE_T;

typedef enum
{
	BROWSER_FONT_26,									
	BROWSER_FONT_32,
	BROWSER_FONT_38,
	BROWSER_FONT_NUM
}BROWSER_FONT_SIZE_T;

typedef  enum
{
	BROWSER_KEY_UP,							/*������*/		
	BROWSER_KEY_DOWN,						/*������*/	
	BROWSER_KEY_LEFT,							/*������*/	
	BROWSER_KEY_RIGHT,						/*������*/	
	BROWSER_KEY_SELECT,						/*����ѡ��*/	
	BROWSER_KEY_PRIVATE_00,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_01,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_02,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_03,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_04,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_05,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_06,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_07,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_08,					/*�û��Զ��尴��*/
	BROWSER_KEY_PRIVATE_09,					/*�û��Զ��尴��*/
	BROWSER_KEY_NUM
}BROWSER_KEY_T;

typedef  enum
{
	BROWSER_FOCUS_NORMAL,							/*�ؼ������������ʾ*/
	BROWSER_FOCUS_ACTIVE,							/*�ؼ�����򼤻�̬��ʾ*/
	BROWSER_FOCUS_HIDE								/*���ؿؼ������*/
}BROWSER_FOCUS_STATE_T;

typedef  enum
{
	BROWSER_TEXT_LANGUAGE_MYANMAR,
	BROWSER_TEXT_LANGUAGE_ENGLISH,
	BROWSER_TEXT_LANGUAGE_NUM
}BROWSER_TEXT_LANGUAGE_T;

typedef struct
{
	BROWSER_U8  i_a;
	BROWSER_U8  i_r;
	BROWSER_U8  i_g;
	BROWSER_U8  i_b;
}BrowserColourARGB_T;

typedef  struct
{
	BROWSER_U32  i_x;
	BROWSER_U32  i_y;
	BROWSER_U32  i_width;
	BROWSER_U32  i_height;
}BrowserArea_T;

typedef  struct
{
	BROWSER_U8  *i_gif_picture_buffer_p; 		/*GIFͼƬ���ݻ���*/	
	BROWSER_U32  i_gif_picture_buffer_len;  		/*GIFͼƬ���ݻ��泤��*/	
	BROWSER_U32  i_gif_picture_width;      			/*GIFͼƬ���*/
	BROWSER_U32  i_gif_picture_heigt;   			/*GIFͼƬ�߶�*/ 
	BROWSER_U8  *i_decode_buffer_p;  			/*GIFͼƬ���뻺��*/ 
	BROWSER_U32  i_decode_buffer_len;			/*GIFͼƬ���뻺�泤��*/ 
}BrowserGifPicture_T;

typedef  struct
{
	BROWSER_U32  i_gif_frame_num;    		/*GIFͼƬ֡��*/	
	BROWSER_U32  i_gif_width;      			/*GIFͼƬ���*/
	BROWSER_U32  i_gif_heigt;   			 	/*GIFͼƬ�߶�*/
	BROWSER_U16  i_gif_background_color;	/*GIFͼƬ����ɫ���������⴦���̶�Ϊ��ɫ*/
}BrowserGifContent_T;

typedef  struct
{
	BROWSER_U32  i_gif_frame_delay_ten_ms;      	/*GIFͼƬ֡��ʱ����λ: 10΢��*/	
	BROWSER_U8*  i_gif_frame_buf_p;  			/*GIFͼƬ������λͼ���ݻ���*/	
}BrowserGifFrame_T;

typedef void (*BrowserTaskFunc_T) (void);
typedef void (*BrowserFilterCallback_T) (BROWSER_U8*  p_data_p, BROWSER_U32  p_data_length);

/*------------------------------------------------------------------------------------------*/

/*----------------------------------------�궨��-----------------------------------------*/

#define  BROWSER_HANDLE	BROWSER_U32					/*���*/
#define  BROWSER_INVALID_HANDLE		0x00000000			/*��Ч���*/
#define  BROWSER_TIMEOUT_INFINITY		0xFFFFFFFF			/*��ʱ���õȴ�*/
#define  BROWSER_TIMEOUT_IMMEDIATE	0x00000000			/*��ʱ�����˳�*/

/*------------------------------------------------------------------------------------------*/

/*---------------------------���½ӿ���BROWSER LIB �ṩ��USER-------------------------------------*/

#ifdef  BROWSER_NAVIGATOR_FLAG

/*++++
 *����:  �����ж���ֲ�����ʱ�������ӿ�
 *����:  
 *1. p_browser_pid  -  �����ts��pid
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_StbVersionInit(BROWSER_U32  p_browser_pid);


/*++++
 *����:  �������ȡ���ݣ������������
 *����:  
 *1. p_browser_data_p  -  �����ԭʼ���ݰ�
 *2. p_browser_data_size  -  �����ԭʼ���ݴ�С
 *3. p_browser_version  -  �����ԭʼ���ݰ汾��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Init(BROWSER_U8* p_browser_data_p,  BROWSER_U32  p_browser_data_size, BROWSER_U32  p_browser_version);


/*++++
 *����:  ��ֹ����������ͷ������Դ
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Term(void);



/*++++
 *����:  ��ȡ�ⲿ������ֵ
 *����:  ��
 *1. p_key  -  �ⲿ���ݰ�����ֵ
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_SendKey(const BROWSER_KEY_T p_key);

/*++++
 *����:  ��ʼ��������ڴ�
 *����:  
 *1. p_os_mem_start_p  -  ������ڴ���ʼ��ַ
 *2. p_os_mem_size  -  ������ڴ����ݴ�С
 *����ֵ:  ��
 */
void  Browser_Os_Mem_Init(BROWSER_U8  *p_os_mem_start_p, BROWSER_U32  p_os_mem_size);

#endif

//---------GIF���뺯���ӿ�------------
/*++++
 *����:  GIF���ݷ���
 *����:  
 *1. p_gif_picture_p  -  GIFԭʼ���ݼ�����BUFFER
 *����ֵ: 
 *BrowserGifContent_T
 */
BrowserGifContent_T  Browser_Gif_Decode_Content(BrowserGifPicture_T*  p_gif_picture_p);  

/*++++
 *����:  GIF֡���ݻ�ȡ
 *����:  
 *1. p_gif_picture_p  -  GIFԭʼ���ݼ�����BUFFER
 *2. p_gif_frame_index  -  ��Ҫ������֡�����������±��1��ʼ
 *����ֵ: 
 *BrowserGifFrame_T
 */
BrowserGifFrame_T  Browser_Gif_Decode_Frame(BrowserGifPicture_T*  p_gif_picture_p, BROWSER_U32  p_gif_frame_index);  


/*------------------------���Ͻӿ���BROWSER LIB �ṩ��USER----------------------------------------*/


/*------------------------���½ӿ�����USER PORTING �ṩΪBROWSER LIB----------------------------------------*/

//---------���⺯���ӿ�------------
/*++++
 *����:  ����������
 *����:  ��
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Os_CreateMutex(void);

/*++++
 *����:  ɾ��������
 *����:  
 *1. p_handle  -  �Ѵ����Ļ�����
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T   Browser_Os_DeleteMutex(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ��������
 *����:  
 *1. p_handle  -  �Ѵ����Ļ�����
 *����ֵ:  ��
 */
void   Browser_Os_LockMutex(BROWSER_HANDLE  p_handle);

/*++++
 *����:  �����ͷ�
 *����:  
 *1. p_handle  -  �Ѵ����Ļ�����
 *����ֵ:  ��
 */
void   Browser_Os_ReleaseMutex(BROWSER_HANDLE  p_handle);

//---------�ź��������ӿ�------------
/*++++
 *����:  �����ź������
 *����:  
 *1. p_init_val  -  BROWSER_TRUE : ������ʼֵ1��BROWSER_FALSE : ������ʼֵ0.
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Os_CreateSemaphore(const BROWSER_BOOLEAN_T  p_init_val);

/*++++
 *����:  ɾ���ź������
 *����:  
 *1. p_handle  -  �Ѵ������ź������
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T   Browser_Os_DeleteSemaphore(BROWSER_HANDLE  p_handle);

/*++++
 *����:  �����ź���
 *����:  
 *1. p_handle  -  �Ѵ������ź������
 *����ֵ:  ��
 */
BROWSER_BOOLEAN_T   Browser_Os_SemaphoreSignal(BROWSER_HANDLE  p_handle);

/*++++
 *����:  �ȴ��ź���
 *����:  
 *1. p_handle  -  �Ѵ������ź������
 *2. p_wait_timeout_ms  -  ��ʱ�ȴ�ʱ�䣬��λΪ����
 *����ֵ:  ��
 */
BROWSER_BOOLEAN_T   Browser_Os_SemaphoreWait(BROWSER_HANDLE  p_handle, const BROWSER_U32  p_wait_timeout_ms);

//---------��Ϣ���к����ӿ�------------
/*++++
 *����:  ������Ϣ���о��
 *����:  
 *1. p_element_size  -  ��Ϣ�������ݴ�С.
 *2. p_element_num  -  ��Ϣ��������Ϣ�ĸ���.
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Os_CreateMessageQueue(const BROWSER_U32  p_element_size, const BROWSER_U32  p_element_num);

/*++++
 *����:  ɾ����Ϣ���о��
 *����:  
 *1. p_handle  -  �Ѵ�������Ϣ���о��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T   Browser_Os_DeleteMessageQueue(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ��һ���Ѵ�������Ϣ����������һ����Ϣ
 *����:  
 *1. p_handle  -  �Ѵ�������Ϣ���о��
 *����ֵ:  
 *1. NULL  -  ʧ��
 *2. OTHERS  -  �ɹ�
 */
void*  Browser_Os_MessageQueueClaim(BROWSER_HANDLE  p_handle);

/*++++
 *����:  �ͷŴ�һ���Ѵ�������Ϣ�������������Ϣ
 *����:  
 *1. p_handle  -  �Ѵ�������Ϣ���о��
 *2. p_msg_p  -  ���������Ϣ
 *����ֵ:  ��
 */
void   Browser_Os_MessageQueueRelease(BROWSER_HANDLE  p_handle,  void* p_msg_p);

/*++++
 *����:  ����һ�����������Ϣ
 *����:  
 *1. p_handle  -  �Ѵ�������Ϣ���о��
 *2. p_msg_p  -  ���������Ϣ
 *����ֵ:  ��
 */
BROWSER_BOOLEAN_T   Browser_Os_MessageQueueSend(BROWSER_HANDLE  p_handle,  void* p_msg_p);

/*++++
 *����:  ������Ϣ�����е���Ϣ
 *����:  
 *1. p_handle  -  �Ѵ�������Ϣ���о��
 *2. p_ticks  -  ��ʱ�ȴ�ʱ��
 *����ֵ:  
 *1. NULL  -  ʧ��
 *2. OTHERS  -  �ɹ�
 */
void*  Browser_Os_MessageQueueReceive(BROWSER_HANDLE  p_handle,  const BROWSER_U32  p_milli_seconds);

//---------�������ӿ�------------
/*++++
 *����:  ����������
 *����:  
 *1. p_task_name_p  -  ������.
 *2. p_task_func_p  -  ������ָ��.
 *3. p_stack_size  -  �����ջ��С
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Os_CreateTask(char*  p_task_name_p, BrowserTaskFunc_T  p_task_func_p, const BROWSER_U32  p_stack_size);

/*++++
 *����:  ɾ������������
 *����:  
 *1. p_handle  -  �Ѵ�����������
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T   Browser_Os_DeleteTask(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ��������
 *����:  
 *1. p_milli_seconds  -  ����ʱ��
 *����ֵ:  ��
 */
void  Browser_Os_TaskSleep(const BROWSER_U32  p_milli_seconds);


//---------�ڴ�������ӿ�------------
/*++++
 *����:  �����ڴ���չ�ӿ�
 *����:  
 *1. p_buffer_size  -  ��������ڴ��С
 *����ֵ:  
 *1. NULL  -  ʧ��
 *2. OTHERS  -  �ɹ�
 */
void*  Browser_Os_Malloc_Plus(const BROWSER_U32  p_buffer_size);  

/*++++
 *����:  �ͷ���������ڴ���չ�ӿ�
 *����:  
 *1. p_buffer_p  -  ��������ڴ�
 *����ֵ:  �� 
 */
void  Browser_Os_Free_Plus(void*  p_buffer_p);  


/*++++
 *����:  �ڴ����
 *����:  
 *1. p_buffer_p  -  ��������ڴ�
 *2. p_char  -  ���ֵ
 *3. p_buffer_size  -  �ڴ��С
 *����ֵ:  �� 
 */
void  Browser_Os_Memset(void*  p_buffer_p,  const BROWSER_U8  p_char,  const BROWSER_U32  p_buffer_size);  

/*++++
 *����:  �ڴ濽��
 *����:  
 *1. p_dest_buffer_p  -  Ŀ���ڴ�
 *2. p_source_buffer_p  -  Դ�ڴ�
 *3. p_buffer_size  -  �ڴ��С
 *����ֵ:  �� 
 */
void  Browser_Os_Memcpy(void*  p_dest_buffer_p,  const void* p_source_buffer_p,  const BROWSER_U32  p_buffer_size);  

//---------STB �˹������ӿ�------------

/*++++
 *����:  ����PID ��TABLE_ID��ȡ��Ӧ��SECTION����
 *����:  
 *1. p_pid  -  pid
 *2. p_table_id  -  table id
 *3. p_filter_callback  -  SECTION���ݻ�ȡ�ص�����
 *����ֵ: 
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Stb_SetFilter(BROWSER_U32  p_pid, BROWSER_U8  p_table_id, BrowserFilterCallback_T  p_filter_callback);

/*++++
 *����:  �ر�ָ����FILTER
 *����:  
 *1. p_filter_handle  -  filter ���
 *����ֵ: ��
 */
void  Browser_Stb_StopFilter(BROWSER_HANDLE  p_filter_handle);


#ifdef  BROWSER_NAVIGATOR_FLAG

//---------�����������ӿ�------------
/*++++
 *����:  ��ʼ���ײ�����
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Driver_Init(void);

/*++++
 *����:  ��ֹ�ײ�����
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Driver_Term(void);

//---------UI  �������ӿ�------------
/*++++
 *����:  ��������BROWSER�Ļ�ͼ����, �ײ�����Ӧ����ͼ���������ʾ��
                 ������ͼ�ӿ������Ϊ����ڸ������������꣬���������
                 ����Ļ�ľ�������
 *����:  
 *1. p_width  -  ������.
 *2. p_height  -  ����߶�.
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ui_CreateRegion(const BROWSER_U32  p_width,  const BROWSER_U32  p_height);


/*++++
 *����:  ��������ͼƬ���
 *����:  
 *1. p_pic_data_p  -  ͼƬ���ݣ���ʽΪGIF��BMP.
 *2. p_pic_data_size  -  ͼƬ���ݴ�С.
 *3. p_pic_area  -  ͼƬ��ʾ����.
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Ui_CreatePicture(BROWSER_U8  *p_pic_data_p,  const BROWSER_U32  p_pic_data_size,  BrowserArea_T  p_pic_area);

/*++++
 *����:  ɾ������ͼƬ���
 *����:  
 *1. p_handle  -  �Ѵ�����ͼƬ���.
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ui_DeletePicture(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ���������ı����
 *����:  
 *1. p_text_string_p  -  �ı��ַ���.
 *2. p_text_language  -  �ı���������.
 *3. p_text_string_length  -  �ı��ַ������ȣ������������ַ�.
 *4. p_text_area  -  �ı���ʾ����.
 *5. p_text_colour  -  �ı��ַ�����ɫ.
 *6. p_text_font_size  -  �ı������С.
 *7. p_text_align_mode  -  �ı����뷽ʽ.
 *8. p_text_dynamic_mode  -  �ı���̬Ч��.
 *����ֵ:  
 *1. BROWSER_INVALID_HANDLE  -  ��Ч�����ʧ��
 *2. OTHERS  -  ��Ч������ɹ�
 */
BROWSER_HANDLE  Browser_Ui_CreateText(char  *p_text_string_p,  
											const BROWSER_TEXT_LANGUAGE_T  p_text_language,
											const BROWSER_U32  p_text_string_length,  
											BrowserArea_T  p_text_area,  
											BrowserColourARGB_T  p_text_colour,  
											BROWSER_FONT_SIZE_T  p_text_font_size,
											BROWSER_STRING_ALIGN_MODE_T  p_text_align_mode,
											BROWSER_STRING_DYNAMIC_MODE_T  p_text_dynamic_mode
											);

/*++++
 *����:  ɾ�������ı����
 *����:  
 *1. p_handle  -  �Ѵ������ı����.
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ui_DeleteText(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ����ͼƬ
 *����:  
 *1. p_handle  -  �Ѵ����Ļ���ͼƬ���.
 *����ֵ:  ��
 */
void  Browser_Ui_DrawPicture(BROWSER_HANDLE  p_handle);

/*++++
 *����:  �����ı�
 *����:  
 *1. p_handle  -  �Ѵ����Ļ����ı����.
 *����ֵ:  ��
 */
void  Browser_Ui_DrawText(BROWSER_HANDLE  p_handle);

/*++++
 *����:  ���ƾ���
 *����:  
 *1. p_area  -  ��������.
 *2. p_colour  -  ������ɫ.
 *����ֵ:  ��
 */
void  Browser_Ui_FillRectangle(BrowserArea_T  p_area,  BrowserColourARGB_T  p_colour);

/*++++
 *����:  ���ƹ��,  �ù�괦��һ������ͼ��(����ͼ���������ͼ��)
 *����:  
 *1. p_area  -  �����������
 *2. p_focus_state  -  ���״̬�����Դ������������������״̬
 *����ֵ: �� 
 */
void  Browser_Ui_DrawFocus(BrowserArea_T  p_area,  BROWSER_FOCUS_STATE_T  p_focus_state);

/*++++
 *����:  UI��ʼ����
 *����:  ��
 *����ֵ:  �� 
 */
void  Browser_Ui_BeginDraw(void);

/*++++
 *����:  UI��ɻ���
 *����:  ��
 *����ֵ:  �� 
 */
void  Browser_Ui_EndDraw(void);

//--ע��:  ���еĻ�ͼ���ı��������Ҫͨ������Browser_Ui_BeginDraw() ��Browser_Ui_EndDraw()
//---------  ��ԣ����ܽ��ѻ��Ƶ�ͼƬ���ı������SCREEN��

//---------STB �˹������ӿ�------------
/*++++
 *����:  ����ǰ���ź�
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �����ɹ�
 *2. BROWSER_FALSE  -  ����ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Stb_LockTuner(void);

/*++++
 *����:  �ر�ǰ���ź�
 *����:  ��
 *����ֵ:  ��
 */
void  Browser_Stb_StopTuner(void);


/*++++
 *����:  ��ʾ�û��ȴ������ڽ���BROWSER
 *����:  ��
 *����ֵ:  ��
 */
void  Browser_Stb_WaitingNotice(void);


/*++++
 *����:  ��ʾ�û����ź�
 *����:  ��
 *����ֵ:  ��
 */
void  Browser_Stb_NoSignalNotice(void);

/*++++
 *����:  ��ʾ�û�������
 *����:  ��
 *����ֵ:  ��
 */
void  Browser_Stb_NoDataNotice(void);

#endif


/*------------------------���Ͻӿ�����USER PORTING �ṩΪBROWSER LIB----------------------------------------*/

/*------------------------���½ӿ��ǹ������ӿ�----------------------------------------*/
#ifdef  BROWSER_AD_FLAG

/*-------------------------------------�����������---------------------------------------*/
typedef  enum
{
	BROWSER_AD_NONE = 0xFF,
	BROWSER_AD_BOOT_SCREEN = 0,
	BROWSER_AD_BANNER = 1,
	BROWSER_AD_CHANNEL_LIST = 2,
	BROWSER_AD_VOL_BAR = 3,
	BROWSER_AD_EPG = 4,
	BROWSER_AD_NUM
}BROWSER_AD_POSITION_T;

typedef  struct
{
	BROWSER_U32  i_bmp_width;
	BROWSER_U32  i_bmp_height;
	BROWSER_U32  i_bmp_pitch;
	BROWSER_U32  i_bmp_bps;
	BROWSER_U8  *i_bmp_data_p;
}BROWSER_AD_BITMAP_T;

typedef  struct
{
	BROWSER_U32  i_year;
	BROWSER_U32  i_month;
	BROWSER_U32  i_day;
}BROWSER_AD_Date_T;

typedef  struct
{
	BROWSER_U32  i_hour;
	BROWSER_U32  i_min;
	BROWSER_U32  i_sec;
}BROWSER_AD_Time_T;


/*---------------------------���½ӿ���BROWSER LIB �ṩ��USER-------------------------------------*/
/*++++
 *����:  ������ݲ���
 *����:  
 *1. p_ad_pid  -  ���ts��pid
 *2. p_download_flag - �����������ݱ�־
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_Download(BROWSER_U32  p_ad_pid, BROWSER_BOOLEAN_T  p_download_flag);

/*++++
 *����:  ��ֹ��棬���ͷ������Դ
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_Term(void);

/*++++
 *����:  ��ʼ��������������ڴ�
 *����:  
 *1. p_ad_ts_mem_start_p  -  ������������ڴ���ʼ��ַ
 *2. p_ad_ts_mem_size  -  ������������ڴ����ݴ�С
 *����ֵ:  ��
 */
void  Browser_Ad_Ts_Mem_Init(BROWSER_U8  *p_ad_ts_mem_start_p, BROWSER_U32  p_ad_ts_mem_size);

/*++++
 *����:  ��ʼ������ڴ�
 *����:  
 *1. p_os_mem_start_p  -  ����ڴ���ʼ��ַ
 *2. p_os_mem_size  -  ����ڴ����ݴ�С
 *����ֵ:  ��
 */
void  Browser_Ad_Mem_Init(BROWSER_U8  *p_ad_mem_start_p, BROWSER_U32  p_ad_mem_size);

/*++++
 *����:  ��ʼ�����ͼƬ����
 *����:  
 *1. p_ad_pic_decode_mem_start_p  -  ���ͼƬ�����ڴ���ʼ��ַ
 *2. p_ad_pic_decode_mem_size  -  ���ͼƬ�����ڴ����ݴ�С
 *����ֵ:  ��
 */
void  Browser_Ad_Picture_Decode_Mem_Init(BROWSER_U8  *p_ad_pic_decode_mem_start_p, BROWSER_U32  p_ad_pic_decode_mem_size);

/*++++
 *����:  ��ȡָ�����λ�õ�ͼƬ����
 *����:  
 *1. p_ad_position  -  ���λ��ö��
 *����ֵ:  
 *1. BROWSER_AD_BITMAP_T  -  ���ͼƬ����
 */
BROWSER_AD_BITMAP_T  Browser_Ad_GetBitmap(BROWSER_AD_POSITION_T  p_ad_position);
BrowserGifPicture_T  Browser_Ad_GetGifInfo(BROWSER_AD_POSITION_T  p_ad_position);
BROWSER_BOOLEAN_T Browser_Ad_VersionChange(void);

/*------------------------���Ͻӿ���BROWSER LIB �ṩ��USER----------------------------------------*/


/*------------------------���½ӿ�����USER PORTING �ṩΪBROWSER LIB----------------------------------------*/
/*++++
 *����:  ����ǰ���ź�
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �����ɹ�
 *2. BROWSER_FALSE  -  ����ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_LockTuner(void);

/*++++
 *����:  �ر�ǰ���ź�
 *����:  ��
 *����ֵ:  ��
 */
void  Browser_Ad_StopTuner(void);

/*++++
 *����:  ��ȡϵͳ��ǰ���ں�ʱ��
 *����:  
 *1. p_ad_pic_decode_mem_start_p  -  ���ͼƬ�����ڴ���ʼ��ַ
 *2. p_ad_pic_decode_mem_size  -  ���ͼƬ�����ڴ����ݴ�С
 *����ֵ:  
 *1. BROWSER_TRUE  -  �����ɹ�
 *2. BROWSER_FALSE  -  ����ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_GetDateTime(BROWSER_AD_Date_T*  p_date_p,  BROWSER_AD_Time_T*  p_time_p);

/*++++
 *����:  ��ʼ���ײ�����
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_Driver_Init(void);

/*++++
 *����:  ��ֹ�ײ�����
 *����:  ��
 *����ֵ:  
 *1. BROWSER_TRUE  -  �ɹ�
 *2. BROWSER_FALSE  -  ʧ��
 */
BROWSER_BOOLEAN_T  Browser_Ad_Driver_Term(void);

BrowserGifFrame_T  Browser_Decode_Pic_Frame(BROWSER_U8 *PicBuf,BROWSER_U32 BufSize);
BrowserGifFrame_T  Browser_Decode_Gif_Frame(BrowserGifPicture_T*  p_gif_picture_p, BROWSER_U32  p_gif_frame_index);

#endif

/*------------------------���Ͻӿ��ǹ������ӿ�----------------------------------------*/



#endif

#endif

