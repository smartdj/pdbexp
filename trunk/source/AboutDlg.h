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
    PDL_DECLARE_MSGMAP();
    DECLARE_COMMAND_HANDLER(OnCommand);
    DECLARE_INITDIALOG_HANDLER(OnInitDialog);
    DECLARE_LBUTTONDOWN_HANDLER(OnLButtonDown);
    void OnHomePage(void);
    void OnMail(void);
    void OnOk(void);
private:
    LHyperLink m_stHome;
    LHyperLink m_stMail;
};
