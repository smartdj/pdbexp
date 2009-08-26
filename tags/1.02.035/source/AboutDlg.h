///////////////////////////////////////////////////////////////////////////////
// 文件名：  AboutDlg.h
// 创建时间：2007-11-12
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    关于对话框
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_ABOUTDLG
#define PDBEXP_ABOUTDLG

#include <PDLWindow.h>
#include <PDLCtrlExt.h>

class CAboutDlg : public LDialog
{
public:
    CAboutDlg(void);
private:
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
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

#endif // PDBEXP_ABOUTDLG
