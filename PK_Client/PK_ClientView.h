
// PK_ClientView.h : CPK_ClientView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "PK_Sock.h"
class CPK_ClientView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CPK_ClientView();
	DECLARE_DYNCREATE(CPK_ClientView)

public:
	enum{ IDD = IDD_PK_CLIENT_FORM };

// Ư���Դϴ�.
public:
	CPK_ClientDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CPK_ClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	static UINT Thread_Reicive(LPVOID wParam); 
protected:
	TCPSocketPtr m_pClientSock;
// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString m_strSendMsg;
	CListBox m_listCtMsg; 
	bool m_bIsConnect;
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtLogin();
};

#ifndef _DEBUG  // PK_ClientView.cpp�� ����� ����
inline CPK_ClientDoc* CPK_ClientView::GetDocument() const
   { return reinterpret_cast<CPK_ClientDoc*>(m_pDocument); }
#endif

