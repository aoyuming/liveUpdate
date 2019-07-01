
// liveUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "liveUpdate.h"
#include "liveUpdateDlg.h"
#include "afxdialogex.h"
#include  <afxinet.h>
#include "BindStatusCallback.h"

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
	//主程序安装路径
	CString appPath = GetAppPath();
	CString mainAppPath = appPath + _T("\\Burster.exe.temp");
	CString versionPath = appPath + _T("\\version.txt");

	CCallback callBack;
	callBack.m_bUseTimeout = FALSE;
	callBack.m_MainDlg = (CliveUpdateDlg*)(AfxGetApp()->GetMainWnd());

	//加载本地文件
	CString bursterRemoteUrl, updateMsg;
	FILE* file = NULL;
	fopen_s(&file, versionPath, "rb");
	if (NULL == file)
		return MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("更新失败, 本地没有配置文件"), _T("提示"), MB_OK);

	char buf[64];
	fscanf_s(file, "远程分组器地址=%s", buf, 64);
	fclose(file);

	CString szUrl;
	szUrl.Format(_T("?abc=%d"), time(NULL)); // 生成随机URL
	bursterRemoteUrl = buf;
	bursterRemoteUrl += szUrl;
	HRESULT ret = URLDownloadToFile(NULL, bursterRemoteUrl, mainAppPath, 0, &callBack);

	if (S_OK == ret)//下载完毕
	{
		//删除旧文件
		DeleteTempFile(appPath + _T("\\Burster.exe"));

		//改名
		CFile::Rename(mainAppPath, appPath + _T("\\Burster.exe"));

		//隐藏本程序
		callBack.m_MainDlg->ShowWindow(FALSE);

		//启动
		WinExec(appPath + _T("\\Burster.exe"), WM_SHOWWINDOW);
	}
	else
	{
		MessageBox(AfxGetApp()->GetMainWnd()->m_hWnd, _T("下载远程文件失败"), _T("提示"), MB_OK);
	}

	//退出本程序
	callBack.m_MainDlg->SendMessage(WM_CLOSE);
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

	/*CWnd* cwnd = FindWindow(_T("fzq"), NULL);
	if (!cwnd)
	{
		exit(0);
		return false;
	}*/

	//获取命令行参数 如果不是调用程序特定传入的参数“-XXXX”，则停止运行 2010/1/17
	/////////////////////////////////////////////////////////////////////////
	int CommandLineCount = 0;
	LPWSTR * m_lpCommandLine = ::CommandLineToArgvW(GetCommandLineW(), &CommandLineCount);
	BOOL result = FALSE;
	CString cmdStr;
	//获取参数行命令，并将UNICODE转化成ASCI进行判断
	for (int i = CommandLineCount - 1; i >= 0; i--)
	{
		cmdStr = m_lpCommandLine[i];
		if (cmdStr == _T("fzq_com"))
		{
			result = TRUE;
			break;
		}
	}

	//释放内存
	GlobalFree(HGLOBAL(m_lpCommandLine));
	if (!result)
	{
		exit(0);
		return FALSE;
	}


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
