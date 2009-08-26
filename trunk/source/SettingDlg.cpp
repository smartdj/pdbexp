///////////////////////////////////////////////////////////////////////////////
// �ļ�����  SettingDlg.cpp
// ����ʱ�䣺2007-11-12
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ���öԻ���ʵ��
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "SettingDlg.h"
#include <pdl_comdlg.h>

#include "resource.h"

CSettingDlg::CSettingDlg(__in LIniParser* pIni) : LDialog(IDD_DLG_SETTING)
{
    m_pIni = pIni;
}

///////////////////////////////////////////////////////////////////////////////

void CSettingDlg::OnCommand(
    WORD wNotifyCode,
    WORD wID,
    HWND hWndCtrl,
    BOOL& bHandled)
{
    switch (wID)
    {
    case IDCANCEL:      OnClose(bHandled);      break;
    case IDOK:          OnOk();                 break;
    case IDC_BTN_OPEN:  OnOpen();               break;
    default:
        bHandled = FALSE;
    }
}

void CSettingDlg::OnOk(void)
{
    TCHAR str[MAX_PATH];
    GetDlgItemText(IDC_EDT_TEMPLATE, str, MAX_PATH);
    if (_T('\0') != str[0])
        m_pIni->WriteString("Setting", "Template", str);
    m_pIni->WriteInt("Setting", "MaxHistory",
        GetDlgItemInt(IDC_EDT_EXPCNT, NULL, FALSE));
    EndDialog(IDOK);
}

void CSettingDlg::OnOpen(void)
{
    LFileDialog dlg(TRUE, _T("*.htm;*.html\0*.htm;*.html\0\0"));
    if (dlg.DoModal(m_hWnd))
        SetDlgItemText(IDC_EDT_TEMPLATE, dlg.m_szFileName);
}

///////////////////////////////////////////////////////////////////////////////

void CSettingDlg::OnClose(BOOL& bHandled)
{
    EndDialog(IDCANCEL);
}

BOOL CSettingDlg::OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled)
{
    LString str;
    m_pIni->GetString("Setting", "Template", _T(""), &str);
    SetDlgItemText(IDC_EDT_TEMPLATE, str);

    int cnt = m_pIni->GetInt("Setting", "MaxHistory", 20);
    if (cnt < 10)
        cnt = 10;
    if (30 < cnt)
        cnt = 30;
    SetDlgItemInt(IDC_EDT_EXPCNT, cnt, FALSE);

    m_spin = GetDlgItem(IDC_SPIN);
    m_spin.SetBuddy(GetDlgItem(IDC_EDT_EXPCNT));
    m_spin.SetRange32(10, 30);
    m_spin.SetPos32(cnt);

    return FALSE;
}
