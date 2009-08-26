///////////////////////////////////////////////////////////////////////////////
// �ļ�����  CmbEx.cpp
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    PDB Explorer ComboBoxʵ��
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
    // FindString����ListBox��ʵ�֣���ΪϵͳĬ�ϵĺ��������Զ������Ҳ�������Ŀ
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
                // �������ListBoxʵ�֣���ΪCB_SETCURSEL��Ӱ��Edit����Ϊ
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
