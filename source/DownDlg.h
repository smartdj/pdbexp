///////////////////////////////////////////////////////////////////////////////
// 文件名：  DownDlg.h
// 创建时间：2008-11-17
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    下载对话框
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_ctrl.h>
#include <pdl_parser.h>
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
    PDL_DECLARE_MSGMAP();
    DECLARE_CLOSE_HANDLER(OnClose);
    DECLARE_COMMAND_HANDLER(OnCommand);
    DECLARE_INITDIALOG_HANDLER(OnInitDialog);
    void OnEditUpdate(WORD wID);
    void OnBtnOpen1(void);
    void OnBtnOpen2(void);
    void OnBtnDownLoad(void);
    void OnOK(void);
private:
    CDownLoader* m_pDnLdr;
    LIniParser* m_pIni;
    LEdit m_edtInfo;
    SIGNTYPE m_type;
    LPTSTR m_pSign;
    LPTSTR m_pFile;
};
