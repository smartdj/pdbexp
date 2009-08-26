///////////////////////////////////////////////////////////////////////////////
// �ļ�����  SettingDlg.h
// ����ʱ�䣺2007-11-12
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ���öԻ���
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
