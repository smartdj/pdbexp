///////////////////////////////////////////////////////////////////////////////
// �ļ�����  ModifyDlg.h
// ����ʱ�䣺2007-11-11
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ����Ի���
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_commctrl.h>
#include "DiaHelper.h"
#include "DetailView.h"

class CModifyDlg : public LDialog, private CEventHandler
{
public:
    CModifyDlg(CDiaHelper* pDia, IDiaSymbol* pSymbol);
private:
    static BOOL cbAddMember(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbEnumModify(IDiaSymbol* pCurSymbol, LPVOID pParam);
    void DumpModified(void);
private:
    PDL_DECLARE_MSGMAP();
    DECLARE_CLOSE_HANDLER(OnClose);
    DECLARE_COMMAND_HANDLER(OnCommand);
    DECLARE_INITDIALOG_HANDLER(OnInitDialog);
    DECLARE_NOTIFY_HANDLER(OnNotify);
    void OnBtnAll(void);
    void OnBtnCopy(void);
private:
    void OnNavigateComplete(void);
private:
    LListView m_list;
    CDetailView m_view;
    IDiaSymbol* m_pSymbol;
    CDiaHelper* m_pDia;
    BOOL m_bInited;
};
