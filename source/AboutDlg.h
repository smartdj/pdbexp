///////////////////////////////////////////////////////////////////////////////
// 文件名：  AboutDlg.h
// 创建时间：2007-11-12
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    关于对话框
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
