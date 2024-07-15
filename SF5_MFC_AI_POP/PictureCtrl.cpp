#include "pch.h"
#include "PictureCtrl.h"

CPictureControl::CPictureControl()
{
}

CPictureControl::~CPictureControl()
{
}

BEGIN_MESSAGE_MAP(CPictureControl, CStatic)
    ON_WM_PAINT()
END_MESSAGE_MAP()

void CPictureControl::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    CRect rect;
    GetClientRect(&rect);

    // Create and select a blue brush for the background
    CBrush brush(GRAY1);
    dc.FillRect(&rect, &brush);

    // Create and select a red pen for the border
    CPen pen(PS_SOLID, 2, WHITE);
    CPen* pOldPen = dc.SelectObject(&pen);

    // Draw the border
    dc.Rectangle(rect);

    // Restore the old pen
    dc.SelectObject(pOldPen);
}