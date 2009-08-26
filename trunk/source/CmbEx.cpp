///////////////////////////////////////////////////////////////////////////////
// �ļ�����  CmbEx.cpp
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    PDB Explorer ComboBoxʵ��
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
    // FindString����ListBox��ʵ�֣���ΪϵͳĬ�ϵĺ��������Զ������Ҳ�������Ŀ
    return m_list.FindString(0, lpszString);
}

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
                // �������ListBoxʵ�֣���ΪCB_SETCURSEL��Ӱ��Edit����Ϊ
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
