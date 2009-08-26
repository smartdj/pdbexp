///////////////////////////////////////////////////////////////////////////////
// 文件名：  MainFrame.h
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    主框架窗口类
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_MAINFRAME
#define PDBEXP_MAINFRAME

#include "CmbEx.h"
#include <PDLCtrlExt.h>
#include <PDLComCtl.h>
#include <PDLParser.h>
#include <PDLContainer.h>
#include "DetailView.h"
#include "DiaHelper.h"

typedef struct _tagExpInfo {
    IDiaSymbol* pSymbol;
    DWORD_PTR   dwTypeInfo;
} EXPINFO, *PEXPINFO;

class CMainFrame : public LWindow, private CEventHandler
{
public:
    CMainFrame(void);
    ~CMainFrame(void);
private:
    void AddExpItem(__in IDiaSymbol* pSymbol, __in DWORD_PTR dwTypeInfo);
    static BOOL cbAddEnum(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbAddUDT(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static void cbDumpString(LPCWSTR pszString, LPVOID pParam);
    void CheckCommandState(void);
    void ClearExpItem(void);
    void DelExpItem(__in LIterator it);
    void Refresh(void);
private:
    int OnCreate(LPCREATESTRUCT lpCreateStruct, BOOL& bHandled);
    void OnDestroy(BOOL& bHandled);
    void OnDropFiles(HDROP hDropInfo, BOOL& bHandled);
    void OnSize(UINT nType, int cx, int cy, BOOL& bHandled);
private:
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
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
private:
    IDiaSymbol* OnSymbolChange(LPCWSTR pszName);
private:
    HFONT                m_hFont;
    LToolBar             m_tb;
    LImageList           m_iml;
    CCmbEx               m_cbSymbols;
    CDetailView          m_vDetail;
    LSplitter            m_split;
    LStatusBar           m_status;
    CDiaHelper           m_dia;
    LIniParser           m_ini;
    LDoubleList<EXPINFO> m_lstHistory;
    LIterator            m_itCurrent;
    int                  m_nMaxHistory;
};

#endif // PDBEXP_MAINFRAME
