
// PK_Client.h : PK_Client ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CPK_ClientApp:
// �� Ŭ������ ������ ���ؼ��� PK_Client.cpp�� �����Ͻʽÿ�.
//

class CPK_ClientApp : public CWinApp
{
public:
	CPK_ClientApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPK_ClientApp theApp;
