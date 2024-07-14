// CustomStatic.h

#pragma once
#include "afxwin.h"

class CCustomStatic : public CStatic
{
public:
    CCustomStatic();
    virtual ~CCustomStatic();

protected:
    DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnPaint();
};
