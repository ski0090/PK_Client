
// PK_ClientView.h : CPK_ClientView 클래스의 인터페이스
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "PK_Sock.h"
class CPK_ClientView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CPK_ClientView();
	DECLARE_DYNCREATE(CPK_ClientView)

public:
	enum{ IDD = IDD_PK_CLIENT_FORM };

// 특성입니다.
public:
	CPK_ClientDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CPK_ClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	static UINT Thread_Reicive(LPVOID wParam); 
protected:
	TCPSocketPtr m_pClientSock;
// 생성된 메시지 맵 함수
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

#ifndef _DEBUG  // PK_ClientView.cpp의 디버그 버전
inline CPK_ClientDoc* CPK_ClientView::GetDocument() const
   { return reinterpret_cast<CPK_ClientDoc*>(m_pDocument); }
#endif

