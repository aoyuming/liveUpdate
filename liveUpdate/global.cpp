#include "stdafx.h"
#include "global.h"
#include "BindStatusCallback.h"

// 需要下载的文件列表
vector<NODE> g_DownList;

//需要下载的文件总大小
int g_DownAllFileSize = 0;

//当前已下载的字节
int g_CurDownByte = 0;

//获取app安装路径
CString GetAppPath()
{
	//本程序安装路径
	CString appPath;
	GetModuleFileName(NULL, appPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	appPath.ReleaseBuffer();
	int pos = appPath.ReverseFind('\\');
	appPath = appPath.Left(pos);
	return appPath;
}

//文件路径转url
void pathConverUrl(CString& str)
{
	if (str[0] == '\\')
		str = str.Right(str.GetLength() - 1);
	str.Replace('\\', '/');
}

//计算所有下载文件的大小的总和
int downFileAllSize(const vector<NODE>& downList)
{
	int size = 0;
	for (int i = 0; i < (int)downList.size(); ++i)
		size += downList[i].size;
	return size;
}

//创建多级文件夹
BOOL CreateMultiDirectory(CString strPath)
{
	CString strSubPath;
	CString strMsg;
	int nCount = 0;
	int nIndex = 0;

	//通过“\”来分割路径，从而创建各级的目录。
	do
	{
		nIndex = strPath.Find(_T("\\"), nIndex) + 1;
		nCount++;
	} while ((nIndex - 1) != -1);
	nIndex = 0;
	//检查，并创建目录
	while ((nCount - 1) >= 0)
	{
		nIndex = strPath.Find(_T("\\"), nIndex) + 1;
		if ((nIndex - 1) == -1)
			strSubPath = strPath;
		else
			strSubPath = strPath.Left(nIndex);

		if (!PathFileExists(strSubPath))// - 检查目录是否存在
		{
			if (!CreateDirectory(strSubPath, NULL))// -不存在则创建目录
			{
				strMsg.Format(_T("创建目录【%s】失败！"), strSubPath);
				AfxMessageBox(strMsg, MB_OK);
				return FALSE;
			}
		}
		nCount--;
	};
	return TRUE;
}

//删除文件夹目录(非空)
bool DeleteDirectory(CString DirName)
{
	CString PUBPATH;
	PUBPATH = DirName;
	CFileFind tempFind;
	DirName += "\\*.*";
	BOOL IsFinded = (BOOL)tempFind.FindFile(DirName);
	while (IsFinded)
	{
		IsFinded = (BOOL)tempFind.FindNextFile();
		if (!tempFind.IsDots())
		{
			CString strDirName;
			strDirName += PUBPATH;
			strDirName += "\\";
			strDirName += tempFind.GetFileName();
			if (tempFind.IsDirectory())
			{
				//strDirName += PUBPATH;
				DeleteDirectory(strDirName);
			}
			else
			{
				SetFileAttributes(strDirName, FILE_ATTRIBUTE_NORMAL); //去掉文件的系统和隐藏属性
				DeleteFile(strDirName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory(PUBPATH))
	{
		return false;
	}
	return true;
}

//递归搜索strdir目录下所有的文件
void findDirAllFile(std::vector<CString>& vFilePathList, CString strDir)
{
	CFileFind finder;
	BOOL isNotEmpty = finder.FindFile(strDir + _T("*.*"));//总文件夹，开始遍历 
	while (isNotEmpty)
	{
		isNotEmpty = finder.FindNextFile();//查找文件 
		CString filename = finder.GetFilePath();//获取文件的路径，可能是文件夹，可能是文件 
		if (!(finder.IsDirectory()))
		{
			//如果是文件则加入文件列表 
			vFilePathList.push_back(filename);//将一个文件路径加入容器 
		}
		else
		{
			//递归遍历用户文件夹，跳过非用户文件夹 
			if (!(finder.IsDots() || finder.IsHidden() || finder.IsSystem() || finder.IsTemporary() || finder.IsReadOnly()))
			{
				findDirAllFile(vFilePathList, filename + _T("/"));
			}
		}
	}
}

//下载远程文件
UINT downRemoteFile(LPVOID lpParam)
{
	if (g_DownList.size() == 0)
		return 0;

	CString appPath = GetAppPath();
	for (int i = 0; i < g_DownList.size(); ++i)
	{
		CCallback callBack;
		callBack.m_bUseTimeout = FALSE;
		callBack.m_MainDlg = (CliveUpdateDlg*)(AfxGetApp()->GetMainWnd());

		CString rdUrl, tempUrl = g_DownList[i].fileUrl;
		pathConverUrl(tempUrl);

		//主程序安装路径
		CString savePath = appPath + _T("\\DownTemp") + g_DownList[i].fileUrl;
		CString remoteUrl = callBack.m_MainDlg->m_PackUrl + tempUrl;
		rdUrl.Format(_T("?abc=%d"), time(NULL)); // 生成随机URL
		remoteUrl += rdUrl;

		int i1 = savePath.ReverseFind('\\');
		CString createPath = savePath.Left(i1);
		CreateMultiDirectory(createPath);

		HRESULT ret = URLDownloadToFile(NULL, remoteUrl, savePath, 0, &callBack);

		if (S_OK != ret)//下载失败
		{
			MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, remoteUrl + _T("文件, 下载失败"), _T("提示"), MB_OK);
			DeleteDirectory(appPath + _T("\\DownTemp"));
			exit(0);
		}
	}

	//已下载文件列表
	vector<CString> downlist;
	findDirAllFile(downlist, appPath + _T("\\DownTemp"));

	//把临时文件夹移动到主目录
	for (int i = 0; i < (int)downlist.size(); ++i)
	{
		CString s1 = downlist[i];
		s1.Replace("\\DownTemp", "");
		int i1 = s1.ReverseFind('\\');
		CString s2 = s1.Left(i1);
		CreateMultiDirectory(s2);
		if (PathFileExists(s1))
		{
			int i2 = s1.ReverseFind('.');
			if (i2 != -1)
			{
				CString hz = s1.Right(s1.GetLength() - i2);
				if (hz == ".dll")
					continue;
			}
			CFile::Remove(s1);
		}
		MoveFile(downlist[i], s1);
	}

	//删除临时文件夹
	DeleteDirectory(appPath + _T("\\DownTemp"));

	//启动主程序
	CliveUpdateDlg* dlg = (CliveUpdateDlg*)(AfxGetApp()->GetMainWnd());
	CString exePath = appPath + _T('\\') + dlg->m_MainExeFileName;

	WinExec(exePath, SW_SHOW);
	exit(0);
	return 0;
}
