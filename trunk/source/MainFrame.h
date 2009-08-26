///////////////////////////////////////////////////////////////////////////////
// �ļ�����  MainFrame.h
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ����ܴ�����
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CmbEx.h"
#include <pdl_ctrlext.h>
#include <pdl_commctrl.h>
#include <pdl_parser.h>
#include <pdl_container.h>
#include "DetailView.h"
#include "DiaHelper.h"
#include "DownLoader.h"

typedef struct _tagExpInfo {
    IDiaSymbol* pSymbol;
} EXPINFO, *PEXPINFO;

class CMainFrame : public LWindow, private CEventHandler
{
public:
    CMainFrame(__in LPWNDCLASS wc);
    ~CMainFrame(void);
private:
    void AddExpItem(__in IDiaSymbol* pSymbol);
    static BOOL cbAddEnum(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbAddTypedef(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbAddUDT(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static void cbDumpString(LPCWSTR pszString, LPVOID pParam);
    void CheckCommandState(void);
    static void DestroyExpItem(PVOID ptr);
    void DumpSymbol(__in IDiaSymbol* pSymbol);
    void Open(__in LPCWSTR pszPdbFile);
    void Refresh(void);
private:
    int OnCreate(LPCREATESTRUCT lpCreateStruct, BOOL& bHandled);
    void OnDestroy(BOOL& bHandled);
    void OnDropFiles(HDROP hDropInfo, BOOL& bHandled);
    void OnSize(UINT nType, int cx, int cy, BOOL& bHandled);
private:
    void OnCommand(WORD wNotifyCode, WORD wID, HWND hWndCtrl, BOOL& bHandled);
    void OnAbout(void);
    void OnBack(void);
    void OnCopy(void);
    void OnCopyAll(void);
    void OnExit(void);
    void OnModify(void);
    void OnNext(void);
    void OnOk(void);
    void OnOpen(void);
    void OnSave(void);
    void OnSetting(void);
    void OnDownLoad(void);
private:
    void OnSymbolChange(DWORD id);
    void OnNewFileDrop(LPCWSTR lpFileName);
private:
    HFONT m_hFont;
    LToolBar m_tb;
    LImageList m_iml;
    CCmbEx m_cbSymbols;
    CDetailView m_vDetail;
    LSplitter m_split;
    LStatusBar m_status;
    CDiaHelper m_dia;
    LIniParser m_ini;
    LPtrList m_lstHistory;
    LIterator m_itCurrent;
    int m_nMaxHistory;
    CDownLoader m_DnLdr;
};
