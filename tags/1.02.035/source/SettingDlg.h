///////////////////////////////////////////////////////////////////////////////
// 文件名：  SettingDlg.h
// 创建时间：2007-11-12
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    设置对话框
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_SETTINGDLG
#define PDBEXP_SETTINGDLG

#include <PDLWindow.h>
#include <PDLParser.h>
#include <PDLComCtl.h>

class CSettingDlg : public LDialog
{
public:
    CSettingDlg(__in LIniParser* pIni);
private:
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
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

#endif // PDBEXP_SETTINGDLG
