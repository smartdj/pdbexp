///////////////////////////////////////////////////////////////////////////////
// �ļ�����  DownDlg.h
// ����ʱ�䣺2008-11-17
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// ˵����    ���ضԻ���
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
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
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
