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
    PDL_DECLARE_MSGMAP();
    DECLARE_COMMAND_HANDLER(OnCommand);
    DECLARE_CLOSE_HANDLER(OnClose);
    DECLARE_INITDIALOG_HANDLER(OnInitDialog);
    void OnOk(void);
    void OnOpen(void);
private:
    LIniParser* m_pIni;
    LUpDown     m_spin;
};
