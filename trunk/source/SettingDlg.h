///////////////////////////////////////////////////////////////////////////////
// �ļ�����  SettingDlg.h
// ����ʱ�䣺2007-11-12
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ���öԻ���
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_parser.h>
#include <pdl_commctrl.h>

class CSettingDlg : public LDialog
{
public:
    CSettingDlg(__in LIniParser* pIni);
private:
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
    void OnOk(void);
    void OnOpen(void);
private:
    void OnClose(BOOL& bHandled);
    BOOL OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled);
private:
    LIniParser* m_pIni;
    LUpDown     m_spin;
};
