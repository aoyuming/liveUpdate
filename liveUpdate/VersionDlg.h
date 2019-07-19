#pragma once


// CVersionDlg �Ի���
#include "global.h"
#include "agreement.h"

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CString m_UpdateMsg;//������Ϣ
	bool& m_isUpdate;//�Ƿ����
	CWnd* m_MainWindow;//�����򴰿ھ��
	UpdateMode m_Mode;
	CVersionDlg(CString msg, bool& update, UpdateMode mode, CWnd* mainWind = NULL, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVersionDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
};
