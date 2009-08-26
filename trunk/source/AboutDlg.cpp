///////////////////////////////////////////////////////////////////////////////
// �ļ�����  AboutDlg.cpp
// ����ʱ�䣺2007-11-12
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ���ڶԻ���ʵ��
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "AboutDlg.h"
#include "Version.h"
#include <ShellAPI.h>

#include "resource.h"

CAboutDlg::CAboutDlg(void) : LDialog(IDD_DLG_ABOUT)
{
}

///////////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnCommand(
    WORD wNotifyCode,
    WORD wID,
    HWND hWndCtrl,
    BOOL& bHandled)
{
    switch (wID)
    {
    case IDCANCEL:
    case IDOK:              OnOk();         break;
    case IDC_ST_HOMEPAGE:   OnHomePage();   break;
    case IDC_ST_MAIL:       OnMail();       break;
    default:
        bHandled = FALSE;
    }
}

void CAboutDlg::OnHomePage(void)
{
    ::ShellExecute(NULL, _T("open"), _T("http://www.titilima.cn"), NULL, NULL,
        SW_SHOWNORMAL);
}

void CAboutDlg::OnMail(void)
{
    ::ShellExecute(NULL, _T("open"), _T("mailto:titilima@163.com"), NULL, NULL,
        SW_SHOWNORMAL);
}

void CAboutDlg::OnOk(void)
{
    EndDialog(IDOK);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CAboutDlg::OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled)
{
    LWnd wndVer = GetDlgItem(IDC_ST_VER);
    wndVer.SetWindowText(PDBEXP_WNDCAPTION);
    wndVer.SizeToContent();

    m_stHome.Attach(GetDlgItem(IDC_ST_HOMEPAGE));
    m_stMail.Attach(GetDlgItem(IDC_ST_MAIL));
    return FALSE;
}

void CAboutDlg::OnLButtonDown(UINT uFlags, int x, int y, BOOL& bHandled)
{
    SendMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(x, y));
}
