// VersionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VersionDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CVersionDlg 对话框

IMPLEMENT_DYNAMIC(CVersionDlg, CDialogEx)

CVersionDlg::CVersionDlg(CString msg, bool& update, CWnd* mainWind, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_VERSION_DLG, pParent),
	m_UpdateMsg(msg),
	m_isUpdate(update),
	m_MainWindow(mainWind)
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
END_MESSAGE_MAP()


// CVersionDlg 消息处理程序


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_UpdateMsg);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

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
		m_MainWindow->SendMessage(WM_CLOSE);
}

//取消
void CVersionDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_isUpdate = false;
	SendMessage(WM_CLOSE);
}
