#ifndef  BROWSER_DATA_PARSER_H
#define  BROWSER_DATA_PARSER_H

#include "BrowserHDI.h"

#ifdef  BROWSER_SWITCH_FLAG

typedef enum
{
	BROWSER_LINK_FUNC_NONE,
	BROWSER_LINK_FUNC_WATCH_TV,
	BROWSER_LINK_FUNC_LISTEN_RADIO,
	BROWSER_LINK_FUNC_FULL_VIEW,
	BROWSER_LINK_FUNC_PLAY_VOD
}BROWSER_LINK_FUNC_T;

typedef  struct
{
	BROWSER_U8  i_index;											/*�ؼ����кţ����кŴ�0��ʼ*/
	BROWSER_U32  i_page_id;											/*�ؼ�������PAGE ID (��0  ��ʼ)*/
	BrowserArea_T  i_area;											/*�ؼ����꼰���(x, y, width, height)*/
	BROWSER_U32  i_link_page_id;									/*���ӵ�PAGE ID*/
	BROWSER_BOOLEAN_T  i_bg_colour_disable_flag;					/*����ɫ���ñ�־, TRUEΪ���ã�FALSEΪʹ��*/
	BROWSER_BOOLEAN_T  i_link_flag;									/*���ӱ�־(����PAGE ���߹���), TRUEΪ���ӣ�FALSEΪ������*/
	BROWSER_BOOLEAN_T  i_iframe_flag;								/*�Ƿ�������ʾIFRAME��TRUEΪ��ʾ��FALSE����ʾ*/
	BROWSER_BOOLEAN_T  i_pic_flag;									/*ͼƬ��־��TRUEΪ��ͼƬ���ݣ�FALSE��ͼƬ��*/
	BrowserColourARGB_T  i_fg_colour;									/*ǰ����ɫ����Ҫ��������ɫ*/
	BrowserColourARGB_T  i_bg_colour;									/*������ɫ*/
	BROWSER_LINK_FUNC_T  i_link_func;								/*���ӹ���*/
	BROWSER_U32  i_string_length;									/*�ַ����ֽڸ���*/
	char  *i_string_p;													/*�ַ���*/
	BROWSER_TEXT_LANGUAGE_T  i_string_language;					/*�ַ�������*/
	BROWSER_STRING_ALIGN_MODE_T  i_string_align;					/*�ַ������뷽ʽ*/
	BROWSER_STRING_DYNAMIC_MODE_T  i_string_dynamic_mode;		/*�ַ�����̬ģʽ*/
	BROWSER_FONT_SIZE_T  i_string_font_size;							/*�ַ�������ߴ�*/
	BROWSER_U32  i_pic_path_length;									/*ͼƬ����·���ַ����ֽ���*/
	char  *i_pic_path_string_p;										/*ͼƬ·���ַ���*/
	BROWSER_U32  i_pic_data_size;									/*ͼƬ���ݴ�С*/
	BROWSER_U8  *i_pic_data_p;										/*ͼƬ����*/
}BrowserWidget_T;	

typedef  struct
{
	BROWSER_U32  i_page_id;											/*PAGE ID (��0  ��ʼ)*/
	BROWSER_BOOLEAN_T  i_play_service_flag;							/*PLAY SERVICE ��־*/
	BROWSER_U16  i_service_id;										/*SERVICE ID*/
	BROWSER_U16  i_ts_id;											/*TRANSPORT STREM ID*/
	BROWSER_U32  i_widget_num;										/*�ؼ�����*/
	BrowserWidget_T  *i_widget_p;										/*�ؼ�����*/
}BrowserPage_T;

typedef  struct
{
	BROWSER_U32  i_width;											/*��������*/
	BROWSER_U32  i_height;											/*������߶�*/
	BROWSER_U32  i_widget_total_num;								/*����PAGEȫ���ؼ�����*/
	BROWSER_U32  i_page_num;										/*PAGE����*/
	BrowserPage_T  *i_page_p;										/*PAGE����*/
}BrowserData_T;


BROWSER_BOOLEAN_T  BrowserDataParser(BROWSER_U8  *p_data_p, BROWSER_U32 p_data_num, BrowserData_T  *p_browser_data_p);

#ifdef  BROWSER_AD_FLAG

BROWSER_BOOLEAN_T  Browser_Ad_DataParser(BROWSER_U8  *p_data_p, BROWSER_U32 p_data_num, BrowserData_T  *p_browser_data_p);

#endif

#endif

#endif

