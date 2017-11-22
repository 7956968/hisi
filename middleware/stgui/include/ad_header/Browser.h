#ifndef  BROWSER_H 
#define  BROWSER_H

#include  "BrowserDataParser.h"

#ifdef  BROWSER_SWITCH_FLAG

typedef  struct
{
	BROWSER_BOOLEAN_T  i_bg_disable_flag;					//�жϱ��������Ƿ�����ʾ
	BrowserArea_T  i_area;									//��ʾ����
	BrowserColourARGB_T  i_bg_colour;							//������ɫ
}BROWSER_WIDGET_BG_UI_T;

typedef  struct
{
	BROWSER_HANDLE  i_pic_handle;
	BROWSER_HANDLE  i_text_handle;
	BROWSER_WIDGET_BG_UI_T  i_bg_handle;
}BROWSER_WIDGET_UI_T;

typedef  struct
{
	BROWSER_U8  i_widget_num;								//PAGE��������WIDGET����
	BROWSER_WIDGET_UI_T  *i_widget_ui_p;					//�ؼ�UI��ʾ����
}BROWSER_PAGE_UI_T;

typedef  struct
{
	BROWSER_BOOLEAN_T  i_init_flag;							//��ʼ����־
	BROWSER_U32  i_browser_version;							//������汾��
	BrowserData_T  i_browser_data;							//��������ݽṹ
	BROWSER_U32  i_first_page_id;							//��һҳID
	BROWSER_U32  i_show_page_id;							//��ǰ������ʾ��PAGE ID
	BROWSER_U8  i_show_page_focus_widget_id;				//��ǰҳ�Ľ���ؼ�ID
	BROWSER_PAGE_UI_T  i_show_page_ui_handle;				//��ǰҳ����ʾUI
	BROWSER_HANDLE  i_msg_handle;
	BROWSER_HANDLE  i_task_handle;
}BROWSER_T;

typedef  struct
{
	BROWSER_U32  i_browser_ad_version;							//���汾��
	BrowserData_T  i_browser_ad_data;							//������ݽṹ
}BROWSER_AD_T;

void*  Browser_Os_Malloc(BROWSER_U32  p_buffer_size);
void  Browser_Os_Free(void*  p_buffer_p);

void*  Browser_Ad_Malloc(BROWSER_U32  p_buffer_size);
void  Browser_Ad_Free(void*  p_buffer_p);


#endif

#endif

