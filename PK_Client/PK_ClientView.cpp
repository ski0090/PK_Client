
// PK_ClientView.cpp : CPK_ClientView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "PK_Client.h"
#endif

#include "PK_ClientDoc.h"
#include "PK_ClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPK_ClientView

IMPLEMENT_DYNCREATE(CPK_ClientView, CFormView)

BEGIN_MESSAGE_MAP(CPK_ClientView, CFormView)
	ON_BN_CLICKED(IDC_BT_SEND, &CPK_ClientView::OnBnClickedBtSend)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_LOGIN, &CPK_ClientView::OnBnClickedBtLogin)
END_MESSAGE_MAP()

// CPK_ClientView 생성/소멸

CPK_ClientView::CPK_ClientView()
	: CFormView(CPK_ClientView::IDD)
	, m_strSendMsg(_T(""))
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CPK_ClientView::~CPK_ClientView()
{
}

void CPK_ClientView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ED_SEND, m_strSendMsg);
	DDX_Control(pDX, IDC_LIST_MSG, m_listCtMsg);
}

BOOL CPK_ClientView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CPK_ClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	//평강: 소켓 라이브러리 초기화
	SocketUtil::StaticInit();
}


// CPK_ClientView 진단

#ifdef _DEBUG
void CPK_ClientView::AssertValid() const
{
	CFormView::AssertValid();
}

void CPK_ClientView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

UINT CPK_ClientView::Thread_Reicive(LPVOID wParam)
{
	auto pThis = (CPK_ClientView*)wParam;
	while (true)
	{
		TCHAR szMsg[1024];
		ZeroMemory(szMsg, 1024);
		PK_CriticalSection pkCritical;
		if (0<pThis->m_pClientSock->Receive(szMsg, 1024))
		{
			pThis->m_listCtMsg.AddString(szMsg);
		}
	}
}

CPK_ClientDoc* CPK_ClientView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPK_ClientDoc)));
	return (CPK_ClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CPK_ClientView 메시지 처리기


void CPK_ClientView::OnBnClickedBtSend()
{
	if (!m_bIsConnect)
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------서버 연결 안됨-------------"));

	UpdateData(TRUE);
	m_pClientSock->Send(m_strSendMsg.GetBuffer(0),m_strSendMsg.GetLength());
	m_strSendMsg=_T("");
	UpdateData(FALSE);
}


void CPK_ClientView::OnDestroy()
{
	CFormView::OnDestroy();

	//평강: 소켓 라이브러리 릴리스
	SocketUtil::CleanUp();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CPK_ClientView::OnBnClickedBtLogin()
{
	m_pClientSock = SocketUtil::CreateTCPSocket(INET);
	SocketAddressPtr pAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:9200");
	if (m_pClientSock->Connect(*pAddress) != NO_ERROR)
	{
		m_pClientSock.reset();
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------서버 접속 실패-------------"));
	}
	else
	{
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------서버 접속 성공-------------"));
		m_pClientSock->SetNonBlockingMode(TRUE);
		AfxBeginThread(Thread_Reicive,this);
		m_bIsConnect = true;
	}
}
