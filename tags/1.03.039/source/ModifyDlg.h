///////////////////////////////////////////////////////////////////////////////
// �ļ�����  ModifyDlg.h
// ����ʱ�䣺2007-11-11
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ����Ի���
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_MODIFYDLG
#define PDBEXP_MODIFYDLG

#include <PDLWindow.h>
#include <PDLComCtl.h>
#include "DiaHelper.h"
#include "DetailView.h"

class CModifyDlg : public LDialog, private CEventHandler
{
public:
    CModifyDlg(CDiaHelper* pDia, IDiaSymbol* pSymbol, DWORD_PTR dwTypeInfo);
private:
    static BOOL cbAddMember(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static void cbDumpString(LPCWSTR pszString, LPVOID pParam);
    static BOOL cbEnumModify(int index, LPVOID pParam);
    void DumpModified(void);
private:
    LRESULT ProcessNotifyMessage(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
private:
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
    void OnBtnAll(void);
    void OnBtnCopy(void);
private:
    void OnClose(BOOL& bHandled);
    BOOL OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled);
private:
    void OnNavigateComplete(void);
private:
    LListView   m_list;
    CDetailView m_view;
    IDiaSymbol* m_pSymbol;
    DWORD_PTR   m_dwTypeInfo;
    CDiaHelper* m_pDia;
    BOOL        m_bInited;
};

#endif // PDBEXP_MODIFYDLG
