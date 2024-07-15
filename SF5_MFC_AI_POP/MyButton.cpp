#include "pch.h"
#include "MyButton.h"

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
    : m_bgColor(RGB(64, 70, 76)) // 기본 배경색 설정
{
    SetButtonFont(_T("나눔스퀘어"), 10);
}

CMyButton::~CMyButton()
{
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
    ON_WM_PAINT()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CMyButton::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    // 버튼의 클라이언트 영역 가져오기
    CRect rect;
    GetClientRect(rect);

    // 배경색 설정
    dc.FillSolidRect(rect, m_bgColor); // m_bgColor 변수를 사용하여 배경색 설정

    // 텍스트 출력 (기본 텍스트 출력)
    CString strText;
    GetWindowText(strText);

    // 폰트 설정 적용
    dc.SelectObject(&m_font);

    // 텍스트 출력
    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(RGB(255, 255, 255)); // 흰색 텍스트
    dc.DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    // 테두리 그리기
    dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT); // 예시로 테두리 스타일을 RAISED로 지정
}

BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
    // 배경 지우기 비활성화
    return TRUE;
}

void CMyButton::SetBackgroundColor(COLORREF color)
{
    m_bgColor = color;
    Invalidate(); // 버튼을 다시 그리도록 강제로 호출
}

void CMyButton::SetButtonFont(const CString& fontName, int fontSize)
{
    m_font.DeleteObject();
    m_font.CreatePointFont(fontSize * 10, fontName); // 폰트 생성
}
