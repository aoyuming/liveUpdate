#pragma once


// CVersionDlg 对话框

class CVersionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVersionDlg)

public:
	CString m_UpdateMsg;//更新信息
	bool& m_isUpdate;//是否更新
	CWnd* m_MainWindow;//主程序窗口句柄
	CVersionDlg(CString msg, bool& update, CWnd* mainWind = NULL, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVersionDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VERSION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
