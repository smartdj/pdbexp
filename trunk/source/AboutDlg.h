///////////////////////////////////////////////////////////////////////////////
// �ļ�����  AboutDlg.h
// ����ʱ�䣺2007-11-12
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ���ڶԻ���
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_ctrlext.h>

class CAboutDlg : public LDialog
{
public:
    CAboutDlg(void);
private:
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
    void OnHomePage(void);
    void OnMail(void);
    void OnOk(void);
private:
    BOOL OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled);
    void OnLButtonDown(UINT uFlags, int x, int y, BOOL& bHandled);
private:
    LHyperLink m_stHome;
    LHyperLink m_stMail;
};
