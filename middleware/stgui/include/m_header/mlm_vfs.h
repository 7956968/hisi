/*****************************************************************************
* Copyright (c) 2010,MDVB
* All rights reserved.
* 
* FileName ��mlm_ffs.h
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
* 		Date ��2010-08-14
*		Record : Create File 
****************************************************************************/
#ifndef	__MLM_VFS_H__ /* ��ֹͷ�ļ����ظ����� */
#define	__MLM_VFS_H__

#include <time.h>
/*----------------------Standard Include-------------------------*/

/*---------------------User-defined Include----------------------*/

/* C++ support */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*------------------------Module Macors---------------------------*/
#define MET_VFS_INVALID_HANDLE 0xffffffff
/*------------------------Module Constants---------------------------*/

/*------------------------Module Types---------------------------*/
typedef MBT_U32 MET_VFS_FileHandle_T;
typedef MBT_U32 MET_VFS_DirHandle_T;


typedef enum _m_vfs_filemode_e
{
    MM_VFS_OPEN_MODE_OpenRead = 1,/*"rb" Opens for reading. If the file does not exist or cannot be found, the fopen call fails*/
    MM_VFS_OPEN_MODE_OpenWrite = 2,/*"ab" Opens for writing at the end of the file (appending) without removing the EOF marker before writing new data to the file; creates the file first if it doesn��t exist. */
    MM_VFS_OPEN_MODE_OpenReadWrite = 4,/*"rb+" Opens for both reading and writing. (The file must exist.)*/
    MM_VFS_OPEN_MODE_OpenCreate = 8,/*"wb+" Opens an empty file for both reading and writing. If the given file exists, its contents are destroyed.*/
    MM_VFS_OPEN_MODE_OpenExsit = 16,/*"rb" Opens for reading. If the file does not exist or cannot be found, the fopen call fails*/
    MM_VFS_OPEN_MODE_OpenAppend = 32/*"ab+" Opens for reading and appending; the appending operation includes the removal of the EOF marker before new data is written to the file and the EOF marker is restored after writing is complete; creates the file first if it doesn��t exist.*/
} MMT_VFS_FileMode_E;

typedef enum _m_vfs_fileseek_e
{
    MM_VFS_SEEK_SET,
    MM_VFS_SEEK_CUR,
    MM_VFS_SEEK_END
} MMT_FFS_FileSeek_E;

/*�ļ�ϵͳ�ڵ���Ϣ*/
typedef struct _m_vfs_dir_t
{
    MBT_U8 		flag; 			/*0��ʾ�Ǹ��ļ��ڵ�,1��ʾ��Ŀ¼�ڵ�*/
    MBT_CHAR         name[256]; 		/*�ڵ����ַ���,�������ϼ�·��*/
    MBT_VOID 	*param; 			/*��������*/
}MST_VFS_DIR_T;

typedef enum _m_vfs_fileattribute_e
{
    MM_VFS_MODE_R = 0x01,
    MM_VFS_MODE_W = 0x02,
    MM_VFS_MODE_RW = (MM_VFS_MODE_R|MM_VFS_MODE_W),
    MM_VFS_MODE_DIR= 0x04,
    MM_VFS_MODE_HIDDEN = 0x08
} MMT_VFS_FileAttribute_E;


/*�ļ�����Ϣ*/
typedef struct _m_vfs_fileinfo_t
{
    time_t stFileLastAccessTime;/*�ļ�����ʱ�䣬���ش�1970��1��1��0ʱ0��0�뿪ʼ��ʱ������*/
    time_t stFileLastModifyTime;/*�ļ�����޸ĵ�ʱ�䣬ʱ��˵��ͬ��*/
    time_t stFileLastChangeMode;/*�ļ��ڵ�״̬��һ�α��޸ĵ�ģʽ*/
    MBT_U32 u32size;/*�ļ���С��ByteΪ��λ*/
    MMT_VFS_FileAttribute_E st_mode;/*�ļ���ȡģʽ*/
}MST_VFS_FileInfo_T;

/*-----------------------Module Variables-------------------------*/

/*-----------------------Module Functions-------------------------*/
/*
*˵�������ļ���
*���������
*	   fileName: ����ϵͳ֧���ļ������ַ�����
*	   u32OpenMode: �򿪷�ʽ��
*�������:
*	    �ޡ�
*���أ�
*		�ɹ������ص��ļ����,�����ʾ��Ч�����
*		ʧ�ܣ�MM_INVALID_HANDLE��
*/
extern MET_VFS_FileHandle_T MLMF_VFS_File_Open(const MBT_CHAR *fileName,MMT_VFS_FileMode_E stOpenMode);

/*
*˵�����ر��ļ���
*���������
*		   fileHandle: �ļ������
*�������:
*			�ޡ�
*	    ���أ�
*		   MM_NO_ERROR:�ɹ���
*		   ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_File_Close(MET_VFS_FileHandle_T fileHandle);

/*
*˵����ɾ��һ���ļ���
*���������
*	   fileName: �ļ��ľ���·����
*�������:
*			�ޡ�
*	    ���أ�
*		   MM_NO_ERROR:�ɹ���
*		   ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_File_Remove(const MBT_CHAR *fileName);

/*
*˵������ָ�����ļ��ж�ȡָ�����ȵ����ݡ�
*���������
*	   fileHandle: ָ�����ļ������
*	   buffer: ����data�ĵ�ַ��
*	   len: data���ȡ�
*�������:
*			�ޡ�
*	    ���أ�
*		    �������ֽ�����
*/
extern MBT_U32 MLMF_VFS_File_Read(MET_VFS_FileHandle_T fileHandle, MBT_U8 *buffer, MBT_U32 len);

/*
*˵������ָ�����ļ���д��ָ�����ȵ����ݡ�
*���������
*	   fileHandle: ָ�����ļ������
*	   buffer: Ҫд������ݻ��档
*	   len: buffer����ĳ��ȡ�
*�������:
*			�ޡ�
*	    ���أ�
*		    д����ֽ�����
*/
extern MBT_U32 MLMF_VFS_File_Write(MET_VFS_FileHandle_T fileHandle, const MBT_CHAR *buffer, MBT_U32 len);

/*
*˵�����ƶ��ļ�ָ�롣
*���������
*	    fileHandle: ָ�����ļ������
*	    type: �ƶ���ʽ��
*	    s32Offset: Ҫ�ƶ����ֽ�����
*�������:
*			�ޡ�
*	    ���أ�
*		   MM_NO_ERROR:�ɹ���
*		   ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_File_Seek(MET_VFS_FileHandle_T fileHandle, MBT_S32 s32Offset,MMT_FFS_FileSeek_E type);

/*
*˵������ȡ�ļ���ǰ��λ�á�
*���������
*	    fileHandle: ָ�����ļ������
*�������:
*	          �ޡ�
*	    ���أ�
*	          ��ǰ���ļ�λ�á�
*/
extern MBT_U32 MLMF_VFS_File_Tell(MET_VFS_FileHandle_T fileHandle);


/*
*˵�������������е�����д��洢�豸��
*���������
*	    fileHandle: ָ�����ļ������
*�������:
*			�ޡ�
*	    ���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_File_Flush(MET_VFS_FileHandle_T fileHandle);

/*
*˵��������ָ����Ŀ¼��
*���������
*	    dirPath: Ŀ¼�ľ���·����
*�������:
*			�ޡ�
*	    ���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Create(const MBT_CHAR *dirPath);

/*
*˵����ɾ��ָ����Ŀ¼(Ŀ¼Ϊ��ʱ����ɾ��)��
*���������
*	    dirPath: Ŀ¼�ľ���·����
*�������:
*			�ޡ�
*���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Remove(const MBT_CHAR *dirPath);

/*
*˵��:  ���ļ�ϵͳ�е�Ŀ¼,��ȡĿ¼�ľ����
*���������
*	    dirPath: Ŀ¼�ľ���·����
*�������:
*			�ޡ�
*���أ�
*		   Ŀ¼������
*/
extern MET_VFS_DirHandle_T MLMF_VFS_Dir_Open(const MBT_CHAR *dirPath);


/*
*˵�����ر�ָ�������Ŀ¼��
*���������
*	    dirHandle: Ŀ¼�����
*�������:
*			�ޡ�
*���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Close(MET_VFS_DirHandle_T dirHandle);

/*
*˵������λĿ¼��ָ��,ʹ��ָ��Ŀ¼���һ����¼��
*���������
*	    dirHandle: Ŀ¼�����
*�������:
*			�ޡ�
*���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Rewind(MET_VFS_DirHandle_T dirHandle);

/*
*˵������ȡָ��Ŀ¼����һ���ڵ��������Ϣ��
*���������
*	    dirHandle: Ŀ¼�����
*�������:
*	    dirNode: ���صĽڵ���Ϣ�ṹ��
*���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Read(MET_VFS_DirHandle_T dirHandle, MST_VFS_DIR_T *dirNode);

/*
*˵�����޸��ļ���Ŀ¼������(·��),��·�������Ǿ���·����
*���������
*	    oldPath: �޸�ǰ����(·��)��
*	    newPath: �޸ĺ�����(·��)��
*�������:
*			�ޡ�
*���أ�
*		    MM_NO_ERROR:�ɹ���
*		    ����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_Rename(const MBT_CHAR *oldPath, const MBT_CHAR *newPath);

/*
*˵������ȡ�ļ��������Ϣ�������ļ�����ʱ�䣨���ش�1970��1��1��0ʱ0��0�뿪ʼ��ʱ�����������ļ�����޸ĵ�ʱ�䣬�ļ�I�ڵ�״̬����޸ĵ�ʱ�䣬�ļ���С����ȡģʽ�ȡ�
*���������
*			filePath: ����������ļ��ľ���·����
*			fileInfo: ��������������ļ��������Ϣ��
*�������:
*			�ޡ�
*		���أ�
*			MM_NO_ERROR:�ɹ���
*			����ţ�ʧ�ܡ�
*/
extern MMT_STB_ErrorCode_E MLMF_VFS_Dir_GetStat(const MBT_CHAR *filePath,MST_VFS_FileInfo_T *fileInfo);



extern MMT_STB_ErrorCode_E MLMF_VFS_Init(MBT_CHAR *rootPath,MBT_U8 partIndex,MBT_CHAR *partNum);



extern MMT_STB_ErrorCode_E MLMF_VFS_UnInit(MBT_VOID);

/*------------------------Module Classes--------------------------*/

/* End C++ support */
#ifdef __cplusplus
}
#endif/* __cplusplus */

#endif /* #ifndef	__MLM_VFS_H__ */

/*----------------------End of mlm_ffs.h-------------------------*/

