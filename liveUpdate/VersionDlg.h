#pragma once


// CVersionDlg �Ի���

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CString m_UpdateMsg;//������Ϣ
	bool& m_isUpdate;//�Ƿ����
	CWnd* m_MainWindow;//�����򴰿ھ��
	CVersionDlg(CString msg, bool& update, CWnd* mainWind = NULL, CWnd* pParent = NULL);   // ��׼���캯��
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
};
