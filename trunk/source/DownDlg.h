///////////////////////////////////////////////////////////////////////////////
// 文件名：  DownDlg.h
// 创建时间：2008-11-17
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    下载对话框
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_DOWNDLG
#define PDBEXP_DOWNDLG

#include <PDLWindow.h>
#include <PDLCtrl.h>
#include <PDLParser.h>
#include "DownLoader.h"
#include "PEAnalyzer.h"

class CDownDlg : public LDialog
{
public:
    CDownDlg(__in CDownLoader* pDnLdr, __in LIniParser* pIni);
    ~CDownDlg(void);
private:
    void AddInfo(__in PCTSTR lpInfo);
    void ClearInfo(void);
private:
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
    void OnEditUpdate(WORD wID);
    void OnBtnOpen1(void);
    void OnBtnOpen2(void);
    void OnBtnDownLoad(void);
    void OnOK(void);
private:
    void OnClose(BOOL& bHandled);
    BOOL OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled);
private:
    CDownLoader* m_pDnLdr;
    LIniParser* m_pIni;
    LEdit m_edtInfo;
    SIGNTYPE m_type;
    LPTSTR m_pSign;
    LPTSTR m_pFile;
};

#endif // PDBEXP_DOWNDLG
