
// liveUpdate.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CliveUpdateApp: 
// �йش����ʵ�֣������ liveUpdate.cpp
//

class CliveUpdateApp : public CWinApp
{
public:
	CliveUpdateApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CliveUpdateApp theApp;