///////////////////////////////////////////////////////////////////////////////
// 文件名：  DetailView.cpp
// 创建时间：2007-11-2
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    符号详细信息类实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include <PDLAppModule.h>
#include "DetailView.h"
#include <Shlwapi.h>

#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
// CEventHandler

void CEventHandler::OnNavigateComplete(void)
{
}

IDiaSymbol* CEventHandler::OnSymbolChange(LPCWSTR pszName)
{
    return NULL;
}

void CEventHandler::OnNewFileDrop(LPCWSTR lpFileName)
{
}

///////////////////////////////////////////////////////////////////////////////
// CDetailView

#define DETAILVIEW_CLASS        _T("DetailView")

CDetailView::CDetailView(void) : m_pEventHandler(NULL), m_pCurSymbol(NULL)
{
    LAppModule *theApp = LAppModule::GetApp();

    WNDCLASS wc = { 0 };

    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.hCursor       = ::LoadCursor(NULL, IDC_ARROW);
    wc.hInstance     = theApp->GetInstance();
    wc.lpfnWndProc   = StartWndProc;
    wc.lpszClassName = DETAILVIEW_CLASS;

    ::RegisterClass(&wc);

    m_bEnable = TRUE;
}

BOOL CDetailView::Create(__in LPCRECT lpRect, __in HWND hWndParent,
                         __in UINT nID, __in LPCWSTR pszTemplate)
{
    return LWindow::Create(DETAILVIEW_CLASS, _T(""), WS_CHILD | WS_VISIBLE,
        lpRect, hWndParent, nID, (LPVOID)pszTemplate);
}

void CDetailView::AddText(__in LPCWSTR pszText)
{
    BSTR bsText = ::SysAllocString(pszText);
    m_pBody->insertAdjacentHTML(L"beforeEnd", bsText);
    ::SysFreeString(bsText);
}

void CDetailView::Clear(void)
{
    m_pBody->put_innerText(L"");
    SetCurrentSymbol(NULL);
}

void CDetailView::Copy(void)
{
    LComPtr<IHTMLSelectionObject> pSel = NULL;
    m_pDoc->get_selection(&pSel);

    LComPtr<IHTMLTxtRange> pTxt = NULL;
    pSel->createRange(reinterpret_cast<IDispatch**>(&pTxt));

    BSTR bsSel = NULL;
    pTxt->get_text(&bsSel);
    if (NULL == bsSel)
        return;

    HGLOBAL hMem = GlobalAlloc(GHND | GMEM_SHARE,
        (lstrlenW(bsSel) + 1) * sizeof(WCHAR));
    LPWSTR  pStr = static_cast<LPWSTR>(GlobalLock(hMem));
    lstrcpyW(pStr, bsSel);
    GlobalUnlock(hMem);
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);

    ::SysFreeString(bsSel);
}

void CDetailView::CopyAll(void)
{
    BSTR bsSel = NULL;
    m_pBody->get_innerText(&bsSel);
    if (NULL == bsSel)
        return;

    HGLOBAL hMem = GlobalAlloc(GHND | GMEM_SHARE,
        (lstrlenW(bsSel) + 1) * sizeof(WCHAR));
    LPWSTR  pStr = static_cast<LPWSTR>(GlobalLock(hMem));
    lstrcpyW(pStr, bsSel);
    GlobalUnlock(hMem);
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);

    ::SysFreeString(bsSel);
}

IDiaSymbol* CDetailView::DetachCurrentSymbol(void)
{
    IDiaSymbol* pRet = m_pCurSymbol;
    m_pCurSymbol     = NULL;
    return pRet;
}

void CDetailView::EnableHyperLink(__in BOOL bEnable)
{
    m_bEnable = bEnable;
}

IDiaSymbol* CDetailView::GetCurrentSymbol(void)
{
    return m_pCurSymbol;
}

BOOL CDetailView::GetText(__out LStringA* pStr)
{
    BSTR bsSel = NULL;
    m_pBody->get_innerText(&bsSel);
    if (NULL == bsSel)
        return FALSE;

    *pStr = bsSel;
    ::SysFreeString(bsSel);
    return TRUE;
}

void CDetailView::SetCurrentSymbol(__in IDiaSymbol* pCurSymbol)
{
    if (NULL != m_pCurSymbol)
        m_pCurSymbol->Release();
    m_pCurSymbol = pCurSymbol;
    if (NULL != m_pCurSymbol)
        m_pCurSymbol->AddRef();
}

BOOL CDetailView::SetEventHandler(__in CEventHandler* pEventHandler)
{
    if (NULL != m_pEventHandler)
        return FALSE;
    m_pEventHandler = pEventHandler;
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

int CDetailView::OnCreate(LPCREATESTRUCT lpCreateStruct, BOOL& bHandled)
{
    CreateAxCtrl(CLSID_WebBrowser);
    Connect(DIID_DWebBrowserEvents2);

    QueryCtrl(IID_IWebBrowser2, reinterpret_cast<LPVOID*>(&m_pWB2));

    RECT rc;
    GetClientRect(&rc);
    m_pWB2->put_Left(rc.left);
    m_pWB2->put_Top(rc.top);
    m_pWB2->put_Width(rc.right);
    m_pWB2->put_Height(rc.bottom);

    WCHAR strExe[MAX_PATH];
    ::GetModuleFileNameW(NULL, strExe, MAX_PATH);

    LStringW strPage;
    strPage.Attach(static_cast<LPWSTR>(lpCreateStruct->lpCreateParams));
    if (0 == strPage.GetLength())
        strPage.Format(L"res://%s/%d", strExe, IDR_HTML);

    VARIANT url;
    ::VariantInit(&url);
    url.vt      = VT_BSTR;
    url.bstrVal = ::SysAllocString(strPage);

    m_pWB2->Navigate2(&url, NULL, NULL, NULL, NULL);

    ::VariantClear(&url);

    LComPtr<IDispatch> pDisp = NULL;
    m_pWB2->get_Document(&pDisp);
    m_pDoc = queryinterface_cast<IHTMLDocument2>(pDisp);

    return 0;
}

void CDetailView::OnDestroy(BOOL& bHandled)
{
    if (NULL != m_pCurSymbol)
        m_pCurSymbol->Release();
    m_pCurSymbol = NULL;

    Disconnect(__uuidof(DWebBrowserEvents2));
    DestroyAxCtrl();
}

void CDetailView::OnSize(UINT nType, int cx, int cy, BOOL& bHandled)
{
    m_pWB2->put_Left(0);
    m_pWB2->put_Top(0);
    m_pWB2->put_Width(cx);
    m_pWB2->put_Height(cy);
}

///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDetailView::QueryInterface(REFIID iid, LPVOID* ppvObject)
{
    if (iid == __uuidof(IDocHostUIHandler))
    {
        *ppvObject = static_cast<IDocHostUIHandler*>(this);
        return S_OK;
    }
    return LAxCtrl::QueryInterface(iid, ppvObject);
}

STDMETHODIMP_(ULONG) CDetailView::AddRef(void)
{
    return LAxCtrl::AddRef();
}

STDMETHODIMP_(ULONG) CDetailView::Release(void)
{
    return LAxCtrl::Release();
}

///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDetailView::Invoke(DISPID dispIdMember, const IID &riid,
                                 LCID lcid, WORD wFlags,
                                 DISPPARAMS *pDispParams, VARIANT *pVarResult,
                                 EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HRESULT hr = S_OK;
    switch (dispIdMember)
    {
    case 250: // BeforeNavigate2
        {
            BeforeNavigate2(pDispParams->rgvarg[6].pdispVal,
                pDispParams->rgvarg[5].pvarVal, pDispParams->rgvarg[4].pvarVal,
                pDispParams->rgvarg[3].pvarVal, pDispParams->rgvarg[2].pvarVal,
                pDispParams->rgvarg[1].pvarVal, pDispParams->rgvarg[0].pboolVal);
        }
        break;
    case 252: // NavigateComplete2
        {
            NavigateComplete2(pDispParams->rgvarg[1].pdispVal,
                pDispParams->rgvarg[0].pvarVal);
        }
        break;
    default:
        hr = E_NOTIMPL;
    }
    return hr;
}

void CDetailView::BeforeNavigate2(__in IDispatch* pDisp, __in VARIANT* URL,
                                  __in VARIANT* Flags,
                                  __in VARIANT* TargetFrameName,
                                  __in VARIANT* PostData,
                                  __in VARIANT* Headers,
                                  __inout VARIANT_BOOL* Cancel)
{
    if (!m_pBody)
        return;
    if (NULL == m_pEventHandler || !m_bEnable)
    {
        *Cancel = VARIANT_TRUE;
        return;
    }

    // 复制文件名
    int len = lstrlenW(URL->bstrVal) + 1;
    LPWSTR pszSymbol = new WCHAR[len];
    ZeroMemory(pszSymbol, len * sizeof(WCHAR));
    lstrcpyW(pszSymbol, URL->bstrVal);

    if (PathFileExistsW(pszSymbol))
    {
        m_pEventHandler->OnNewFileDrop(pszSymbol);
    }
    else
    {
        LPWSTR      pszSplit = wcsrchr(pszSymbol, L'?');
        BOOL        bHandled = TRUE;
        IDiaSymbol* pRet     = m_pEventHandler->OnSymbolChange(pszSplit + 1);
        SetCurrentSymbol(pRet);
    }
    delete[] pszSymbol;
    *Cancel = VARIANT_TRUE;
}

void CDetailView::NavigateComplete2(__in IDispatch* pDisp, __in VARIANT* URL)
{
    // Body一定要在浏览完成后获取
    m_pDoc->get_body(&m_pBody);
    if (NULL != m_pEventHandler)
        m_pEventHandler->OnNavigateComplete();
}

///////////////////////////////////////////////////////////////////////////////
// IDocHostUIHandler

STDMETHODIMP CDetailView::ShowContextMenu(__in DWORD dwID, __in POINT *ppt,
                                          __in IUnknown *pcmdtReserved,
                                          __in IDispatch *pdispReserved)
{
#ifdef _DEBUG
    return E_NOTIMPL;
#else
    return S_OK;
#endif
}

STDMETHODIMP CDetailView::GetHostInfo(__inout DOCHOSTUIINFO *pInfo)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::ShowUI(__in DWORD dwID,
                                 __in IOleInPlaceActiveObject *pActiveObject,
                                 __in IOleCommandTarget *pCommandTarget,
                                 __in IOleInPlaceFrame *pFrame,
                                 __in IOleInPlaceUIWindow *pDoc)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::HideUI(void)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::UpdateUI(void)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::EnableModeless(__in BOOL fEnable)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::OnDocWindowActivate(__in BOOL fActivate)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::OnFrameWindowActivate(__in BOOL fActivate)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::ResizeBorder(__in LPCRECT prcBorder,
                                       __in IOleInPlaceUIWindow *pUIWindow,
                                       __in BOOL fRameWindow)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::TranslateAccelerator(__in LPMSG lpMsg,
                                               __in const GUID *pguidCmdGroup,
                                               __in DWORD nCmdID)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetOptionKeyPath(__out LPOLESTR *pchKey,
                                           __in DWORD dw)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetDropTarget(__in IDropTarget *pDropTarget,
                                        __out IDropTarget **ppDropTarget)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetExternal(__out IDispatch **ppDispatch)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::TranslateUrl(__in DWORD dwTranslate,
                                       __in OLECHAR *pchURLIn,
                                       __out OLECHAR **ppchURLOut)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::FilterDataObject(__in IDataObject *pDO,
                                           __out IDataObject **ppDORet)
{
    return E_NOTIMPL;
}
