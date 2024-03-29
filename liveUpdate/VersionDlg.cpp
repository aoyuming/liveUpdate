// VersionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VersionDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CVersionDlg 对话框

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CString msg, bool& update, UpdateMode mode, CWnd* mainWind, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VERSION_DLG, pParent),
	m_UpdateMsg(msg),
	m_isUpdate(update),
	m_MainWindow(mainWind),
	m_Mode(mode)
{

}

CVersionDlg::~CVersionDlg()
{
}

void CVersionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVersionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CVersionDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CVersionDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVersionDlg 消息处理程序


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//使窗口总是在最前面

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_UpdateMsg);
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(1, false);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0, _T("总是检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1, _T("每周检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2, _T("从不检查更新"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel((int)m_Mode);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//更新
void CVersionDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isUpdate = true;
	SendMessage(WM_CLOSE);
	if (m_MainWindow)
	{
		CString s;
		GetDlgItem(IDC_COMBO1)->GetWindowText(s);
		COPYDATASTRUCT cpd;                     // 给COPYDATASTRUCT结构赋值
		cpd.dwData = 0;
		cpd.cbData = s.GetLength();
		cpd.lpData = (void*)s.GetBufferSetLength(cpd.cbData);
		m_MainWindow->SendMessage(WM_COPYDATA, NULL, (LPARAM)&cpd);// 发送
		m_MainWindow->SendMessage(WM_CLOSE);
		m_MainWindow = NULL;
	}
}

//取消
void CVersionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isUpdate = false;
	SendMessage(WM_CLOSE);
}


void CVersionDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_MainWindow)
	{
		CString s;
		GetDlgItem(IDC_COMBO1)->GetWindowText(s);
		COPYDATASTRUCT cpd;                     // 给COPYDATASTRUCT结构赋值
		cpd.dwData = 0;
		cpd.cbData = s.GetLength();
		cpd.lpData = (void*)s.GetBufferSetLength(cpd.cbData);
		m_MainWindow->SendMessage(WM_COPYDATA, NULL, (LPARAM)&cpd);// 发送
	}

	CDialogEx::OnClose();
}
