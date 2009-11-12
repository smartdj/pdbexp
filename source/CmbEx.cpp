///////////////////////////////////////////////////////////////////////////////
// 文件名：  CmbEx.cpp
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    PDB Explorer ComboBox实现
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "CmbEx.h"

PDL_DEFINE_WINCLASS(CLbEx)

CLbEx& CLbEx::operator=(__in HWND hWnd)
{
    LListBox::Attach(hWnd);
    LSubclassWnd::SubclassWindow(hWnd);
    return *this;
}

PDL_BEGIN_MSGMAP(CLbEx)
    PROCESS_LBUTTONDBLCLK(OnLButtonDblClk)
PDL_END_MSGMAP(LSubclassWnd)

void CLbEx::OnLButtonDblClk(UINT uFlags, int x, int y, BOOL& bHandled)
{
    ::SendMessage(GetParent(), WM_COMMAND, IDOK, (LPARAM)m_hWnd);
}

PDL_DEFINE_WINCLASS(CCmbEx)

BOOL CCmbEx::Create(
    __in PCTSTR lpWindowName,
    __in DWORD dwStyle,
    __in LPCRECT lpRect,
    __in HWND hWndParent,
    __in UINT nID,
    __in PVOID lpParam)
{
    BOOL bRet = LComboBox::Create(lpWindowName, dwStyle, lpRect, hWndParent,
        nID, lpParam);
    LSubclassWnd::SubclassWindow(m_hWnd);

    m_list = GetListBox();
    m_edit = GetEdit();

    return bRet;
}

int CCmbEx::FindString(__in LPCTSTR lpszString)
{
    // FindString调用ListBox的实现，因为系统默认的函数竟会自动创建找不到的项目
    return m_list.FindString(0, lpszString);
}

PDL_BEGIN_MSGMAP(CCmbEx)
    PROCESS_COMMAND(OnCommand)
    PROCESS_KEYDOWN(OnKeyDown)
PDL_END_MSGMAP(LSubclassWnd)

void CCmbEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, BOOL& bHandled)
{
    if (VK_RETURN == nChar)
        ::SendMessage(GetParent(), WM_COMMAND, IDOK, (LPARAM)m_hWnd);
    else
        bHandled = FALSE;
}

void CCmbEx::OnCommand(
    WORD wNotifyCode,
    WORD wID,
    HWND hWndCtrl,
    BOOL& bHandled)
{
    if (EN_UPDATE == wNotifyCode)
    {
        LString str;
        m_edit.GetWindowText(&str);
        int len = str.GetLength();
        if (0 < len)
        {
            int idx = FindString(str);
            if (CB_ERR != idx)
            {
                // 必须借助ListBox实现，因为CB_SETCURSEL会影响Edit的行为
                m_list.SetCurSel(idx);
                m_edit.SetSel(len, -1);
            }
        }
    }
    else if (IDOK == wID)
    {
        ::SendMessage(GetParent(), WM_COMMAND, IDOK, (LPARAM)m_hWnd);
    }
    else
    {
        bHandled = FALSE;
    }
}
