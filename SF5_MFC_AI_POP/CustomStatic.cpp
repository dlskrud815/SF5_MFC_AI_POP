// CustomStatic.cpp

#include "pch.h"
#include "CustomStatic.h"

CCustomStatic::CCustomStatic()
{
}

CCustomStatic::~CCustomStatic()
{
}

BEGIN_MESSAGE_MAP(CCustomStatic, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CCustomStatic::OnPaint()
{
    CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트
    CRect rect;
    GetClientRect(&rect);

    // 배경색 채우기
    dc.FillSolidRect(rect, RGB(0, 0, 0)); // 검정색 배경으로 채움

    // 테두리 그리기
    CBrush brushWhite(RGB(64, 70, 76));
    CPen penWhite(PS_SOLID, 2, RGB(255, 255, 255));
    CPen* pOldPen = dc.SelectObject(&penWhite);
    CBrush* pOldBrush = dc.SelectObject(&brushWhite);

    dc.Rectangle(rect);

    dc.SelectObject(pOldPen);
    dc.SelectObject(pOldBrush);
}
