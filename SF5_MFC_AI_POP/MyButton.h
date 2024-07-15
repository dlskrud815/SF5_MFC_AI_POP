#pragma once

class CMyButton : public CButton
{
    DECLARE_DYNAMIC(CMyButton)

public:
    CMyButton();
    virtual ~CMyButton();

protected:
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);

    DECLARE_MESSAGE_MAP()

private:
    COLORREF m_bgColor; // 배경색을 저장할 변수
    CFont m_font; // 버튼에 적용할 폰트

    void SetBgColor(COLORREF color); // 배경색 설정 함수

public:
    void SetBackgroundColor(COLORREF color); // 배경색 설정을 위한 공개 함수
};
