///////////////////////////////////////////////////////////////////////////////
// 文件名：  CmbEx.h
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    PDB Explorer ComboBox类
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_ctrl.h>

class CLbEx : public LListBox, protected LSubclassWnd
{
    PDL_DECLARE_WINCLASS(CLbEx)
public:
    CLbEx& operator=(__in HWND hWnd);
protected:
    PDL_DECLARE_MSGMAP();
    DECLARE_LBUTTONDBLCLK_HANDLER(OnLButtonDblClk);
};

class CCmbEx : public LComboBox, protected LSubclassWnd
{
    PDL_DECLARE_WINCLASS(CCmbEx)
public:
    BOOL Create(__in PCTSTR lpWindowName, __in DWORD dwStyle,
        __in LPCRECT lpRect, __in HWND hWndParent, __in UINT nID,
        __in PVOID lpParam);
public:
    int FindString(__in LPCTSTR lpszString);
protected:
    PDL_DECLARE_MSGMAP();
    DECLARE_COMMAND_HANDLER(OnCommand);
    DECLARE_KEYDOWN_HANDLER(OnKeyDown);
private:
    CLbEx m_list;
    LEdit m_edit;
};
