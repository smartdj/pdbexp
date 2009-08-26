///////////////////////////////////////////////////////////////////////////////
// 文件名：  CmbEx.cpp
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    PDB Explorer ComboBox实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include "CmbEx.h"

CLbEx& CLbEx::operator=(__in HWND hWnd)
{
    SubclassWindow(hWnd, this);
    return *this;
}

void CLbEx::OnLButtonDblClk(UINT uFlags, int x, int y, BOOL& bHandled)
{
    ::SendMessage(GetParent(), WM_COMMAND, IDOK,
        reinterpret_cast<LPARAM>(m_hWnd));
}

BOOL CCmbEx::Create(__in LPCTSTR lpWindowName, __in DWORD dwStyle,
                    __in LPCRECT lpRect, __in HWND hWndParent, __in UINT nID,
                    __in LPVOID lpParam)
{
    BOOL bRet = LComboBox::Create(lpWindowName, dwStyle, lpRect, hWndParent,
        nID, lpParam);
    SubclassWindow(Detach(), this);

    m_list = GetListBox();
    m_edit = GetEdit();

    return bRet;
}

int CCmbEx::FindString(__in LPCTSTR lpszString)
{
    // FindString调用ListBox的实现，因为系统默认的函数竟会自动创建找不到的项目
    return m_list.FindString(0, lpszString);
}

void CCmbEx::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, BOOL& bHandled)
{
    if (VK_RETURN == nChar)
    {
        ::SendMessage(GetParent(), WM_COMMAND, IDOK,
            reinterpret_cast<LPARAM>(m_hWnd));
    }
    else
    {
        bHandled = FALSE;
    }
}

void CCmbEx::ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
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
        ::SendMessage(GetParent(), WM_COMMAND, IDOK,
            reinterpret_cast<LPARAM>(m_hWnd));
    }
    else
    {
        bHandled = FALSE;
    }
}
