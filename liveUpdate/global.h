#pragma once
#include <vector>
using namespace std;

struct NODE
{
	int size;
	CString md5;
	CString fileUrl;
};

// ��Ҫ���ص��ļ��б�
extern vector<NODE> g_DownList;

//��Ҫ���ص��ļ��ܴ�С
extern int g_DownAllFileSize;

//��ǰ�����ص��ֽ�
extern int g_CurDownByte;

//��ȡapp��װ·��
CString GetAppPath();

//�ļ�·��תurl
void pathConverUrl(CString& str);

//�������������ļ��Ĵ�С���ܺ�
int downFileAllSize(const vector<NODE>& downList);

//�����༶�ļ���
BOOL CreateMultiDirectory(CString strPath);

//ɾ���ļ���Ŀ¼(�ǿ�)
bool DeleteDirectory(CString DirName);

//����Զ���ļ�
UINT downRemoteFile(LPVOID lpParam);

void findDirAllFile(std::vector<CString>& vFilePathList, CString strDir);

