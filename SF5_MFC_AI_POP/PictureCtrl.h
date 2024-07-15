#pragma once

#include "color.h"

class CPictureControl : public CStatic
{
public:
    CPictureControl();
    virtual ~CPictureControl();

protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

