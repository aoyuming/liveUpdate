// VersionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VersionDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CVersionDlg �Ի���

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


// CVersionDlg ��Ϣ�������


BOOL CVersionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);//ʹ������������ǰ��

	GetDlgItem(IDC_EDIT1)->SetWindowText(m_UpdateMsg);
	((CEdit*)GetDlgItem(IDC_EDIT1))->SetSel(1, false);
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0, _T("���Ǽ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1, _T("ÿ�ܼ�����"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2, _T("�Ӳ�������"));
	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel((int)m_Mode);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//����
void CVersionDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isUpdate = true;
	SendMessage(WM_CLOSE);
	if (m_MainWindow)
	{
		CString s;
		GetDlgItem(IDC_COMBO1)->GetWindowText(s);
		COPYDATASTRUCT cpd;                     // ��COPYDATASTRUCT�ṹ��ֵ
		cpd.dwData = 0;
		cpd.cbData = s.GetLength();
		cpd.lpData = (void*)s.GetBufferSetLength(cpd.cbData);
		m_MainWindow->SendMessage(WM_COPYDATA, NULL, (LPARAM)&cpd);// ����
		m_MainWindow->SendMessage(WM_CLOSE);
		m_MainWindow = NULL;
	}
}

//ȡ��
void CVersionDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_isUpdate = false;
	SendMessage(WM_CLOSE);
}


void CVersionDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_MainWindow)
	{
		CString s;
		GetDlgItem(IDC_COMBO1)->GetWindowText(s);
		COPYDATASTRUCT cpd;                     // ��COPYDATASTRUCT�ṹ��ֵ
		cpd.dwData = 0;
		cpd.cbData = s.GetLength();
		cpd.lpData = (void*)s.GetBufferSetLength(cpd.cbData);
		m_MainWindow->SendMessage(WM_COPYDATA, NULL, (LPARAM)&cpd);// ����
	}

	CDialogEx::OnClose();
}
