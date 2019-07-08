
// liveUpdateDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>

using namespace std;

// CliveUpdateDlg 对话框
class CliveUpdateDlg : public CDialogEx
{
// 构造
public:
	CliveUpdateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIVEUPDATE_DIALOG };
#endif

	protected:                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	struct NODE
	{
		int size;
		CString md5;
		CString fileUrl;
	};

	CWinThread*	m_pThread;

	//加载项目配置文件
	BOOL LoadProjectManifest(CString filePath);

	CString m_LaunchParameters;//启动参数
	CString m_Name;//主程序名字
	CString m_PackUrl;//远程项目根目录url
	unsigned int m_DetailedCount;//清单数量
	CString m_ProjectUrl;//远程配置文件url
	CString m_VersionUrl;//远程版本文件url
	CString m_UpdateMsg;//更新内容

	//需要下载的列表
	std::vector<NODE> m_DownFileVect;

	//本地本版本所有文件的列表
	std::vector<NODE> m_LoaclAllFileVect;

	//比较版本信息
	int compareVersion(int v1, int v2, int v3, int v4, int v5, int v6);

	//生成下载列表
	void createDownList(const vector<NODE>& localVect, std::vector<NODE>& downVect);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	// 下载进度
	CProgressCtrl m_DownProgress;
	afx_msg void OnClose();
	CStatic m_OutPut;
};
