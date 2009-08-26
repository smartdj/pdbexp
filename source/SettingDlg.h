///////////////////////////////////////////////////////////////////////////////
// 文件名：  SettingDlg.h
// 创建时间：2007-11-12
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    设置对话框
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
