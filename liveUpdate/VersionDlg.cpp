// VersionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VersionDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CVersionDlg �Ի���

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


// CVersionDlg ��Ϣ��������


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_UpdateMsg);
	GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//����
void CVersionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_isUpdate = true;
	SendMessage(WM_CLOSE);
	if (m_MainWindow)
		m_MainWindow->SendMessage(WM_CLOSE);
}

//ȡ��
void CVersionDlg::OnBnClickedButton2()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	m_isUpdate = false;
	SendMessage(WM_CLOSE);
}