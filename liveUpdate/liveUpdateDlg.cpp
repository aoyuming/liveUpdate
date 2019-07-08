
// liveUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "liveUpdate.h"
#include "liveUpdateDlg.h"
#include "afxdialogex.h"
#include  <afxinet.h>
#include "BindStatusCallback.h"
#include <assert.h>

#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

//删除临时文件
void DeleteTempFile(CString fileName)
{
	if (PathFileExists(fileName))
	{
		//删除
		CFile tempFile;
		tempFile.Remove(fileName);
	}
}

//下载远程文件
UINT downRemoteFile(LPVOID lpParam)
{
	////主程序安装路径
	//CString appPath = GetAppPath();
	//CString mainAppPath = appPath + _T("\\Burster.exe.temp");
	//CString versionPath = appPath + _T("\\version.txt");

	//CCallback callBack;
	//callBack.m_bUseTimeout = FALSE;
	//callBack.m_MainDlg = (CliveUpdateDlg*)(AfxGetApp()->GetMainWnd());

	////加载本地文件
	//CString bursterRemoteUrl, updateMsg;
	//FILE* file = NULL;
	//fopen_s(&file, versionPath, "rb");
	//if (NULL == file)
	//	return MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("更新失败, 本地没有配置文件"), _T("提示"), MB_OK);

	//char buf[64];
	//fscanf_s(file, "远程分组器地址=%s", buf, 64);
	//fclose(file);

	//CString szUrl;
	//szUrl.Format(_T("?abc=%d"), time(NULL)); // 生成随机URL
	//bursterRemoteUrl = buf;
	//bursterRemoteUrl += szUrl;
	//HRESULT ret = URLDownloadToFile(NULL, bursterRemoteUrl, mainAppPath, 0, &callBack);

	//if (S_OK == ret)//下载完毕
	//{
	//	//删除旧文件
	//	DeleteTempFile(appPath + _T("\\Burster.exe"));

	//	//改名
	//	CFile::Rename(mainAppPath, appPath + _T("\\Burster.exe"));

	//	//隐藏本程序
	//	callBack.m_MainDlg->ShowWindow(FALSE);

	//	//启动
	//	WinExec(appPath + _T("\\Burster.exe"), WM_SHOWWINDOW);
	//}
	//else
	//{
	//	MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("下载远程文件失败"), _T("提示"), MB_OK);
	//}

	////退出本程序
	//callBack.m_MainDlg->SendMessage(WM_CLOSE);
	return 0;
}


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CliveUpdateDlg 对话框



CliveUpdateDlg::CliveUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LIVEUPDATE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CliveUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_DownProgress);
	DDX_Control(pDX, ID_STATIC, m_OutPut);
}

BEGIN_MESSAGE_MAP(CliveUpdateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CliveUpdateDlg::OnBnClickedOk)
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CliveUpdateDlg 消息处理程序

BOOL CliveUpdateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	::CreateMutex(NULL, TRUE, "分组器自动更新程序");//字符串里面的内容可以随便改.他只是一个名字
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		exit(0);
		return false;
	}

	if (!LoadProjectManifest(_T("project.manifest")))
		return FALSE;

	//生成差异文件列表
	createDownList(m_LoaclAllFileVect, m_DownFileVect);


	////获取命令行参数 如果不是调用程序特定传入的参数“-XXXX”，则停止运行 2010/1/17
	//int CommandLineCount = 0;
	//LPWSTR * m_lpCommandLine = ::CommandLineToArgvW(GetCommandLineW(), &CommandLineCount);
	//BOOL result = FALSE;
	//CString cmdStr;
	////获取参数行命令，并将UNICODE转化成ASCI进行判断
	//for (int i = CommandLineCount - 1; i >= 0; i--)
	//{
	//	cmdStr = m_lpCommandLine[i];
	//	if (cmdStr == m_LaunchParameters || m_LaunchParameters == _T("null"))//是否是启动参数
	//	{
	//		result = TRUE;
	//		break;
	//	}
	//}

	////释放内存
	//GlobalFree(HGLOBAL(m_lpCommandLine));
	//if (!result)
	//{
	//	exit(0);
	//	return FALSE;
	//}


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	m_pThread = AfxBeginThread(downRemoteFile, this);

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//比较版本信息 
int CliveUpdateDlg::compareVersion(int v1, int v2, int v3, int v4, int v5, int v6)
{
	int version1 = v1 * 100 + v2 * 10 + v3;
	int version2 = v4 * 100 + v5 * 10 + v6;
	return version1 - version2;
}

//生成下载列表
void CliveUpdateDlg::createDownList(const vector<NODE>& localVect, std::vector<NODE>& downVect)
{
	vector<NODE> diff;
	CString appPath = GetAppPath();

	//对比版本号
	//1：加载本地版本号
	FILE* pf = NULL;
	fopen_s(&pf, appPath + _T("\\version.manifest"), "rb");
	if (!pf)
		return;

	int v1, v2, v3;
	fscanf_s(pf, _T("版本:%d.%d.%d"), &v1, &v2, &v3);
	fclose(pf);

	//远程文件下载下来 存放的路径
	CString verTemp = appPath + _T("\\DownTemp\\version.manifest");
	CString proTemp = appPath + _T("\\DownTemp\\project.manifest");

	//如果没有该文件夹，就创建
	if (!PathFileExists(appPath + _T("\\DownTemp")))
		CreateDirectory(appPath + _T("\\DownTemp"), NULL);

	//下载远程version文件
	HRESULT ret = URLDownloadToFile(NULL, m_VersionUrl, verTemp,0, NULL);

	if (S_OK == ret)//下载完毕
	{
		//读取
		pf = NULL;
		fopen_s(&pf, verTemp, "rb");
		if (!pf)
			return;

		int v4, v5, v6;
		fscanf_s(pf, _T("版本:%d.%d.%d"), &v4, &v5, &v6);
		fclose(pf);

		if (compareVersion(v1, v2, v3, v4, v5, v6) < 0)
		{
			//读取新版本更新的东西
			CFile file;
			if (file.Open(verTemp, CFile::modeRead))
			{
				char* buf = new char[file.GetLength() + 1];
				file.Read(buf, file.GetLength());
				buf[file.GetLength()] = 0;
				file.Close();
				m_UpdateMsg = buf;
				delete []buf;
			}


			//读取远程文件列表信息
			ret = URLDownloadToFile(NULL, m_ProjectUrl, proTemp, 0, NULL);
			if (S_OK == ret)//下载完毕
			{
				//读取
				pf = NULL;
				fopen_s(&pf, proTemp, "rb");
				if (!pf)
					return;

				char buf[5][64];
				int count = 0;
				fscanf_s(pf, _T("远程packageUrl地址:%s"), buf[0], 64);
				fscanf_s(pf, _T("\r\n远程project.manifest地址:%s"), buf[1], 64);
				fscanf_s(pf, _T("\r\n远程version.manifest地址:%s"), buf[2], 64);
				fscanf_s(pf, _T("\r\n主程序名字:%s"), buf[3], 64);
				fscanf_s(pf, _T("\r\n启动参数:%s"), buf[4], 64);
				fscanf_s(pf, _T("\r\n清单文件数量:%d"), &count);

				//远程文件信息列表
				vector<NODE> remoteVect;
				for (int i = 0; i < count; ++i)
				{
					NODE node;
					char buff[2][64];
					fscanf_s(pf, _T("\r\n文件url:%s"), buff[0], 64);
					fscanf_s(pf, _T("\r\nMd5:%s"), buff[1], 64);
					fscanf_s(pf, _T("\r\nSize:%d"), &node.size);
					node.fileUrl = buff[0];
					node.md5 = buff[1];
					remoteVect.push_back(node);
				}

				//比较差异
				for (int i = 0; i < (int)remoteVect.size(); ++i)
				{
					BOOL down = TRUE;
					for (int j = 0; j < (int)localVect.size(); ++j)
					{
						if (remoteVect[i].fileUrl == localVect[j].fileUrl)
						{
							if (remoteVect[i].md5 == localVect[j].md5)
								down = FALSE;
							break;
						}
					}

					//下载
					if (down)
					{
						downVect.push_back(remoteVect[i]);
					}
				}

				fclose(pf);
			}
		}
	}
	else
	{
		MessageBox( _T("下载远程文件失败"), _T("提示"), MB_OK);
	}
}

//加载项目配置文件
BOOL CliveUpdateDlg::LoadProjectManifest(CString filePath)
{
	CString appPath = GetAppPath();
	FILE* pf = NULL;
	fopen_s(&pf, appPath + _T('\\') + filePath, "rb");
	if (NULL == pf)
	{
		MessageBox(_T("加载配置文件失败"));
		SendMessage(WM_CLOSE);
		return FALSE;
	}

	char buf[5][64];
	fscanf_s(pf, _T("远程packageUrl地址:%s"), buf[0], 64);
	fscanf_s(pf, _T("\r\n远程project.manifest地址:%s"), buf[1], 64);
	fscanf_s(pf, _T("\r\n远程version.manifest地址:%s"), buf[2], 64);
	fscanf_s(pf, _T("\r\n主程序名字:%s"), buf[3], 64);
	fscanf_s(pf, _T("\r\n启动参数:%s"), buf[4], 64);
	fscanf_s(pf, _T("\r\n清单文件数量:%d"), &m_DetailedCount);

	m_PackUrl = buf[0];
	m_ProjectUrl = buf[1];
	m_VersionUrl = buf[2];
	m_Name = buf[3];
	m_LaunchParameters = buf[4];

	for (int i = 0; i < (int)m_DetailedCount; ++i)
	{
		NODE node;
		char buff[2][64];
		fscanf_s(pf, _T("\r\n文件url:%s"), buff[0], 64);
		fscanf_s(pf, _T("\r\nMd5:%s"), buff[1], 64);
		fscanf_s(pf, _T("\r\nSize:%d"), &node.size);
		node.fileUrl = buff[0];
		node.md5 = buff[1];
		m_LoaclAllFileVect.push_back(node);
	}

	//关闭文件
	fclose(pf);
	return TRUE;
}


void CliveUpdateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CliveUpdateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CliveUpdateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//下载
void CliveUpdateDlg::OnBnClickedOk()
{

}

void CliveUpdateDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*if (m_pThread)
	{
		TerminateThread(m_pThread, 1);
		delete m_pThread;
		m_pThread = NULL;
	}*/

	//删除临时文件
	CString appPath = GetAppPath();
	DeleteTempFile(appPath + _T("\\Burster.exe.temp"));
	
	CDialogEx::OnClose();
}
