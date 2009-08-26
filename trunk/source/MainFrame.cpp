///////////////////////////////////////////////////////////////////////////////
// 文件名：  MainFrame.cpp
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    主框架窗口实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include <PDLAppModule.h>
#include "MainFrame.h"
#include <PDLCom.h>
#include <PDLComDlg.h>
#include <PDLMenu.h>
#include <PDLFile.h>
#include "ModifyDlg.h"
#include "SettingDlg.h"
#include "AboutDlg.h"

#include "resource.h"

#define TB_BTN_COUNT    8

const TBBUTTON g_btns[TB_BTN_COUNT] = {
    { 0, ID_OPEN,    TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 0 },
    { 1, ID_SAVE,    TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 1 },
    { 2, ID_BACK,    TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 2 },
    { 3, ID_NEXT,    TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 3 },
    { 4, ID_COPYALL, TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 4 },
    { 5, ID_MODIFY,  TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 5 },
    { 6, ID_SETTING, TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 6 },
    { 7, ID_ABOUT,   TBSTATE_ENABLED, BTNS_AUTOSIZE, { 0, 0 }, 0, 7 }
};

CMainFrame::CMainFrame(void)
{
    LAppModule *theApp = LAppModule::GetApp();

    WNDCLASS wc      = { 0 };
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = theApp->LoadIcon(MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hInstance     = theApp->GetInstance();
    wc.lpfnWndProc   = LWindow::StartWndProc;
    wc.lpszClassName = _T("PDBExp");
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);

    ::RegisterClass(&wc);

    LOGFONT lf  = { 0 };
    lstrcpy(lf.lfFaceName, _T("Courier New"));
    lf.lfHeight = -12;
    lf.lfWeight = FW_NORMAL;
    m_hFont     = ::CreateFontIndirect(&lf);

    m_ini.Open(_T("PDBExp.ini"));
    m_nMaxHistory = m_ini.GetInt("Setting", "MaxHistory", 20);
    if (m_nMaxHistory < 10)
        m_nMaxHistory = 10;
    if (30 < m_nMaxHistory)
        m_nMaxHistory = 30;
    m_itCurrent   = NULL;
}

CMainFrame::~CMainFrame(void)
{
    if (NULL != m_hFont)
        ::DeleteObject(m_hFont);
}

void CMainFrame::AddExpItem(__in IDiaSymbol* pSymbol,
                            __in DWORD_PTR dwTypeInfo)
{
    EXPINFO info;
    info.pSymbol    = pSymbol;
    info.dwTypeInfo = dwTypeInfo;
    if (m_itCurrent == m_lstHistory.GetTailIterator())
    {
        // 如果是浏览末尾，则加入末尾项
        m_lstHistory.AddTail(info);
        if (m_nMaxHistory < m_lstHistory.GetSize())
        {
            // 如果超过了最大限制，则删除最早项
            LIterator itDel = m_lstHistory.GetHeadIterator();
            DelExpItem(itDel);
        }
    }
    else
    {
        // 对于其余的情况，则删除当前浏览项后所有项
        if (NULL != m_itCurrent)
        {
            LIterator itDel = m_lstHistory.GetTailIterator();
            while (itDel != m_itCurrent)
            {
                DelExpItem(itDel);
                itDel = m_lstHistory.GetTailIterator();
            }
        }
        m_lstHistory.AddTail(info);
    }
    m_itCurrent = m_lstHistory.GetTailIterator();
    CheckCommandState();

    LMenu menu = GetMenu();
    m_tb.EnableButton(ID_SAVE, TRUE);
    m_tb.EnableButton(ID_MODIFY, TRUE);
    menu.EnableMenuItem(ID_SAVE, MF_BYCOMMAND | MF_ENABLED);
    menu.EnableMenuItem(ID_MODIFY, MF_BYCOMMAND | MF_ENABLED);
}

BOOL CMainFrame::cbAddEnum(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    CMainFrame *pThis = (CMainFrame*)pParam;

    BSTR b = NULL;
    pCurSymbol->get_name(&b);

    LString strName = b;
    if (0 != lstrcmp(strName, _T("__unnamed"))
        && CB_ERR == pThis->m_cbSymbols.FindString(strName))
    {
        int idx = pThis->m_cbSymbols.AddString(strName);
        pThis->m_cbSymbols.SetItemData(idx,
            MAKELPARAM(0, SymTagEnum));
    }
    ::SysFreeString(b);

    return FALSE;
}

BOOL CMainFrame::cbAddUDT(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    CMainFrame *pThis = (CMainFrame*)pParam;

    UdtKind enKind;
    pCurSymbol->get_udtKind(reinterpret_cast<LPDWORD>(&enKind));

    BSTR b = NULL;
    pCurSymbol->get_name(&b);

    LString strName = b;
    if (UdtClass != enKind // TODO: Class!!
        && 0 != lstrcmp(strName, _T("__unnamed"))
        && CB_ERR == pThis->m_cbSymbols.FindString(strName))
    {
        int idx = pThis->m_cbSymbols.AddString(strName);
        pThis->m_cbSymbols.SetItemData(idx,
            MAKELPARAM(enKind, SymTagUDT));
    }
    ::SysFreeString(b);

    return FALSE;
}

void CMainFrame::cbDumpString(LPCWSTR pszString, LPVOID pParam)
{
    CMainFrame *pThis = (CMainFrame*)pParam;
    pThis->m_vDetail.AddText(pszString);
}

void CMainFrame::CheckCommandState(void)
{
    LMenu menu = GetMenu();
    if (NULL == m_itCurrent || m_lstHistory.GetHeadIterator() == m_itCurrent)
    {
        m_tb.EnableButton(ID_BACK, FALSE);
        menu.EnableMenuItem(ID_BACK, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
    else
    {
        m_tb.EnableButton(ID_BACK, TRUE);
        menu.EnableMenuItem(ID_BACK, MF_BYCOMMAND | MF_ENABLED);
    }

    if (NULL == m_itCurrent || m_lstHistory.GetTailIterator() == m_itCurrent)
    {
        m_tb.EnableButton(ID_NEXT, FALSE);
        menu.EnableMenuItem(ID_NEXT, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
    else
    {
        m_tb.EnableButton(ID_NEXT, TRUE);
        menu.EnableMenuItem(ID_NEXT, MF_BYCOMMAND | MF_ENABLED);
    }

    if (NULL == m_vDetail.GetCurrentSymbol())
    {
        m_tb.EnableButton(ID_SAVE, FALSE);
        m_tb.EnableButton(ID_MODIFY, FALSE);
        menu.EnableMenuItem(ID_SAVE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
        menu.EnableMenuItem(ID_MODIFY, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
    }
    else
    {
        m_tb.EnableButton(ID_SAVE, TRUE);
        m_tb.EnableButton(ID_MODIFY, TRUE);
        menu.EnableMenuItem(ID_SAVE, MF_BYCOMMAND | MF_ENABLED);
        menu.EnableMenuItem(ID_MODIFY, MF_BYCOMMAND | MF_ENABLED);
    }
}

void CMainFrame::ClearExpItem(void)
{
    LIterator it = m_lstHistory.GetHeadIterator();
    while (NULL != it)
    {
        const EXPINFO& info = m_lstHistory.GetDataRef(it);
        info.pSymbol->Release();
        m_lstHistory.GetNextIterator(&it);
    }
    m_lstHistory.Clear();
    m_itCurrent = NULL;
}

void CMainFrame::DelExpItem(__in LIterator it)
{
    const EXPINFO& info = m_lstHistory.GetDataRef(it);
    info.pSymbol->Release();
    m_lstHistory.Remove(it);
}

void CMainFrame::Refresh(void)
{
    m_cbSymbols.ResetContent();
    m_dia.EnumTag(NULL, SymTagUDT, cbAddUDT, this, NULL);
    m_dia.EnumTag(NULL, SymTagEnum, cbAddEnum, this, NULL);
}

///////////////////////////////////////////////////////////////////////////////

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct, BOOL& bHandled)
{
    LAppModule *theApp = LAppModule::GetApp();

    RECT rcClient, rcToolBar, rcStatus;
    GetClientRect(&rcClient);

    // 创建并初始化工具栏
    HBITMAP hBmp = theApp->LoadBitmap(MAKEINTRESOURCE(IDB_BMP_TOOLBAR));
    m_iml.Create(16, 16, ILC_MASK | ILC_COLOR24, 0, 2);
    m_iml.AddMasked(hBmp, 0xff00ff);

    m_tb.Create(CCS_TOP | TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        m_hWnd, IDC_TOOLBAR);
    m_tb.ButtonStructSize(sizeof(TBBUTTON));
    m_tb.SetImageList(m_iml);
    m_tb.AddString(_T("打开"));
    m_tb.AddString(_T("保存"));
    m_tb.AddString(_T("后退"));
    m_tb.AddString(_T("前进"));
    m_tb.AddString(_T("复制"));
    m_tb.AddString(_T("整理"));
    m_tb.AddString(_T("设置"));
    m_tb.AddString(_T("关于"));
    m_tb.AddButtons(TB_BTN_COUNT, g_btns);
    m_tb.AutoSize();
    m_tb.GetSizeRect(&rcToolBar);

    // 创建并初始化状态栏
    int nParts[] = { 200, -1 };
    m_status.Create(m_hWnd, _T(""),
        SBARS_SIZEGRIP | WS_CHILD | WS_BORDER | WS_VISIBLE,
        IDC_STATUS);
    m_status.GetClientRect(&rcStatus);
    m_status.SetParts(2, nParts);

    // 创建分隔条
    m_split.Create(m_hWnd, IDC_SPLITTER, 0, rcToolBar.bottom, rcClient.right,
        rcClient.bottom - rcStatus.bottom - rcToolBar.bottom);

    // 创建符号列表
    m_cbSymbols.Create(_T(""),
        WS_VISIBLE | CBS_SIMPLE | CBS_SORT | WS_VSCROLL | CBS_NOINTEGRALHEIGHT,
        NULL, m_split.GetSafeHWND(), IDC_CB_SYMBOLS, NULL);
    m_cbSymbols.SetFont(m_hFont);

    // 创建符号视图
    LStringW strTemplate;
    m_ini.GetString("Setting", "Template", L"", &strTemplate);
    m_vDetail.Create(NULL, m_split.GetSafeHWND(), IDC_VIEW_DETAIL,
        strTemplate.Detach());
    m_vDetail.SetEventHandler(this);

    // 设置分隔条面板
    m_split.SetPane(LSPLIT_LEFTPANE, m_cbSymbols, 200);
    m_split.SetPane(LSPLIT_RIGHTPANE, m_vDetail.GetSafeHWND());

    if (!m_dia.InitDia())
    {
        MessageBox(_T("DIA库初始化失败。"), _T("错误"), MB_ICONERROR);
        return -1;
    }

    // 文件拖放支持
    DragAcceptFiles(m_hWnd, TRUE);

    m_status.SetText(0, _T("未打开文件"));
    m_vDetail.EnableWindow(FALSE);
    CheckCommandState();
    return 0;
}

void CMainFrame::OnDestroy(BOOL& bHandled)
{
    ::PostQuitMessage(0);
}

void CMainFrame::OnDropFiles(HDROP hDropInfo, BOOL& bHandled)
{
    WCHAR szFile[MAX_PATH];
    if (0 < DragQueryFileW(hDropInfo, 0, szFile, MAX_PATH))
    {
        ClearExpItem();
        m_vDetail.EnableWindow();
        m_vDetail.Clear();
        if (m_dia.OpenPDB(szFile))
        {
            m_status.SetText(0, _T("正在扫描..."));
            m_status.SetText(1, szFile);
            Refresh();
            CheckCommandState();
            LString str;
            str.Format(_T("共有 %d 个符号"), m_cbSymbols.GetCount());
            m_status.SetText(0, str);
        }
        else
        {
            MessageBox(_T("打开PDB文件失败！"), _T("错误"), MB_ICONSTOP);
        }
    }
    DragFinish(hDropInfo);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy, BOOL& bHandled)
{
    if (SIZE_MINIMIZED != nType)
    {
        RECT rcToolBar, rcStatus;
        m_tb.GetSizeRect(&rcToolBar);
        m_tb.SetWindowPos(NULL, 0, 0, cx, rcToolBar.bottom,
            SWP_NOMOVE | SWP_NOZORDER);

        m_status.GetClientRect(&rcStatus);
        m_status.SetWindowPos(NULL, 0, cy - rcStatus.bottom,
            cx, rcStatus.bottom, SWP_NOZORDER);

        m_split.SetWindowPos(NULL, 0, rcToolBar.bottom,
            cx, cy - rcStatus.bottom - rcToolBar.bottom,
            SWP_NOZORDER | SWP_NOMOVE | SWP_NOREDRAW);
        m_split.Invalidate();
    }
}

///////////////////////////////////////////////////////////////////////////////

void CMainFrame::ProcessCommandMessage(WORD wNotifyCode, WORD wID,
                                       HWND hWndCtrl, BOOL& bHandled)
{
    switch (wID)
    {
    case IDOK:          OnOk();         break;
    case ID_ABOUT:      OnAbout();      break;
    case ID_BACK:       OnBack();       break;
    case ID_COPY:       OnCopy();       break;
    case ID_COPYALL:    OnCopyAll();    break;
    case ID_EXIT:       OnExit();       break;
    case ID_MODIFY:     OnModify();     break;
    case ID_NEXT:       OnNext();       break;
    case ID_OPEN:       OnOpen();       break;
    case ID_SAVE:       OnSave();       break;
    case ID_SETTING:    OnSetting();    break;
    default:
        bHandled = FALSE;
    }
}

void CMainFrame::OnAbout(void)
{
    CAboutDlg dlg;
    dlg.DoModal(m_hWnd);
}

void CMainFrame::OnBack(void)
{
    if (m_itCurrent != m_lstHistory.GetHeadIterator())
    {
        m_vDetail.Clear();
        m_lstHistory.GetPrevIterator(&m_itCurrent);
        const EXPINFO& info = m_lstHistory.GetDataRef(m_itCurrent);
        m_dia.DumpSymbol(info.pSymbol, info.dwTypeInfo, cbDumpString, this);
        m_vDetail.SetCurrentSymbol(info.pSymbol);
    }
    CheckCommandState();
}

void CMainFrame::OnCopy(void)
{
    // TODO: 如何为IWebBrowser2实现默认的复制？
    HWND hWnd = ::GetFocus();
    TCHAR strClass[MAX_PATH];
    ::GetClassName(hWnd, strClass, MAX_PATH);

    if (0 == lstrcmp(strClass, _T("Internet Explorer_Server")))
        m_vDetail.Copy();
    else
        ::SendMessage(hWnd, WM_COPY, 0, 0);
}

void CMainFrame::OnCopyAll(void)
{
    m_vDetail.CopyAll();
}

void CMainFrame::OnExit(void)
{
    DestroyWindow();
}

void CMainFrame::OnModify(void)
{
    LStringW strTemplate;
    m_ini.GetString("Setting", "Template", L"", &strTemplate);

    const EXPINFO& info = m_lstHistory.GetDataRef(m_itCurrent);
    CModifyDlg dlg(&m_dia, info.pSymbol, info.dwTypeInfo);
    dlg.DoModal(m_hWnd, reinterpret_cast<LPARAM>(strTemplate.Detach()));
}

void CMainFrame::OnNext(void)
{
    if (m_itCurrent != m_lstHistory.GetTailIterator())
    {
        m_vDetail.Clear();
        m_lstHistory.GetNextIterator(&m_itCurrent);
        const EXPINFO& info = m_lstHistory.GetDataRef(m_itCurrent);
        m_dia.DumpSymbol(info.pSymbol, info.dwTypeInfo, cbDumpString, this);
        m_vDetail.SetCurrentSymbol(info.pSymbol);
    }
    CheckCommandState();
}

void CMainFrame::OnOk(void)
{
    m_vDetail.Clear();
    int idx = m_cbSymbols.GetCurSel();
    if (CB_ERR != idx)
    {
        LStringW str;
        m_cbSymbols.GetLBText(idx, &str);
        DWORD_PTR dwTypeInfo = m_cbSymbols.GetItemData(idx);

        IDiaSymbol *pCurSymbol = m_dia.FindSymbol(NULL,
            static_cast<enum SymTagEnum>(HIWORD(dwTypeInfo)), str);
        m_dia.DumpSymbol(pCurSymbol, dwTypeInfo, cbDumpString, this);
        m_vDetail.SetCurrentSymbol(pCurSymbol);
        AddExpItem(pCurSymbol, dwTypeInfo);
    }
}

void CMainFrame::OnOpen(void)
{
    LFileDialogW dlg(TRUE, L"*.pdb\0*.pdb\0\0", L"*.pdb");
    if (dlg.DoModal(m_hWnd))
    {
        ClearExpItem();
        m_vDetail.EnableWindow();
        m_vDetail.Clear();
        if (m_dia.OpenPDB(dlg.m_szFileName))
        {
            m_status.SetText(0, _T("正在扫描..."));
            m_status.SetText(1, dlg.m_szFileName);
            Refresh();
            CheckCommandState();
            LString str;
            str.Format(_T("共有 %d 个符号"), m_cbSymbols.GetCount());
            m_status.SetText(0, str);
        }
        else
        {
            MessageBox(_T("打开PDB文件失败！"), _T("错误"), MB_ICONSTOP);
        }
    }
}

void CMainFrame::OnSave(void)
{
    LStringA str;
    if (!m_vDetail.GetText(&str))
        return;

    LFileDialogW dlg(FALSE, L"*.txt\0*.txt\0\0", NULL);
    IDiaSymbol* pSymbol = m_vDetail.GetCurrentSymbol();
    BSTR        bsName  = NULL;
    pSymbol->get_name(&bsName);
    wsprintfW(dlg.m_szFileName, L"%s.txt", bsName);
    ::SysFreeString(bsName);

    if (dlg.DoModal(m_hWnd))
    {
        LFile file;
        file.Create(dlg.m_szFileName, GENERIC_WRITE, 0, CREATE_ALWAYS);
        file.Write((LPCSTR)str, str.GetLength());
    }
}

void CMainFrame::OnSetting(void)
{
    CSettingDlg dlg(&m_ini);
    if (IDOK == dlg.DoModal(m_hWnd))
        m_ini.Save(_T("PDBExp.ini"));
}

///////////////////////////////////////////////////////////////////////////////

IDiaSymbol* CMainFrame::OnSymbolChange(LPCWSTR pszName)
{
    LComPtr<IDiaSymbol> pCurSymbol;
    pCurSymbol.Attach(m_vDetail.DetachCurrentSymbol());
    m_vDetail.Clear();

    LComPtr<IDiaSymbol> pData = m_dia.FindSymbol(pCurSymbol, SymTagData,
        pszName);
    IDiaSymbol* pRet = CDiaHelper::GetSymbolBaseType(pData);
    enum SymTagEnum enTag = SymTagNull;
    pRet->get_symTag(reinterpret_cast<LPDWORD>(&enTag));
    DWORD_PTR dwTypeInfo = MAKELPARAM(0, enTag);
    if (SymTagUDT == enTag)
    {
        UdtKind enKind;
        pRet->get_udtKind(reinterpret_cast<LPDWORD>(&enKind));
        dwTypeInfo = MAKELPARAM(enKind, enTag);
    }
    m_dia.DumpSymbol(pRet, dwTypeInfo, cbDumpString, this);
    AddExpItem(pRet, dwTypeInfo);
    return pRet;
}
