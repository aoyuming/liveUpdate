#pragma once
#include <vector>
using namespace std;

struct NODE
{
	int size;
	CString md5;
	CString fileUrl;
};

// 需要下载的文件列表
extern vector<NODE> g_DownList;

//需要下载的文件总大小
extern int g_DownAllFileSize;

//当前已下载的字节
extern int g_CurDownByte;

//获取app安装路径
CString GetAppPath();

//文件路径转url
void pathConverUrl(CString& str);

//计算所有下载文件的大小的总和
int downFileAllSize(const vector<NODE>& downList);

//创建多级文件夹
BOOL CreateMultiDirectory(CString strPath);

//删除文件夹目录(非空)
bool DeleteDirectory(CString DirName);

//下载远程文件
UINT downRemoteFile(LPVOID lpParam);

void findDirAllFile(std::vector<CString>& vFilePathList, CString strDir);

