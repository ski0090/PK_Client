
// PK_ClientView.cpp : CPK_ClientView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CPK_ClientView ����/�Ҹ�

CPK_ClientView::CPK_ClientView()
	: CFormView(CPK_ClientView::IDD)
	, m_strSendMsg(_T(""))
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CFormView::PreCreateWindow(cs);
}

void CPK_ClientView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	//��: ���� ���̺귯�� �ʱ�ȭ
	SocketUtil::StaticInit();
}


// CPK_ClientView ����

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

CPK_ClientDoc* CPK_ClientView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPK_ClientDoc)));
	return (CPK_ClientDoc*)m_pDocument;
}
#endif //_DEBUG


// CPK_ClientView �޽��� ó����


void CPK_ClientView::OnBnClickedBtSend()
{
	if (!m_bIsConnect)
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------���� ���� �ȵ�-------------"));

	UpdateData(TRUE);
	m_pClientSock->Send(m_strSendMsg.GetBuffer(0),m_strSendMsg.GetLength());
	m_strSendMsg=_T("");
	UpdateData(FALSE);
}


void CPK_ClientView::OnDestroy()
{
	CFormView::OnDestroy();

	//��: ���� ���̺귯�� ������
	SocketUtil::CleanUp();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}


void CPK_ClientView::OnBnClickedBtLogin()
{
	m_pClientSock = SocketUtil::CreateTCPSocket(INET);
	SocketAddressPtr pAddress = SocketAddressFactory::CreateIPv4FromString("127.0.0.1:9200");
	if (m_pClientSock->Connect(*pAddress) != NO_ERROR)
	{
		m_pClientSock.reset();
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------���� ���� ����-------------"));
	}
	else
	{
		m_listCtMsg.InsertString(m_listCtMsg.GetCount(), _T("--------------���� ���� ����-------------"));
		m_pClientSock->SetNonBlockingMode(TRUE);
		AfxBeginThread(Thread_Reicive,this);
		m_bIsConnect = true;
	}
}
