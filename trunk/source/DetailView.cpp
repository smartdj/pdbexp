///////////////////////////////////////////////////////////////////////////////
// 文件名：  DetailView.cpp
// 创建时间：2007-11-2
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    符号详细信息类实现
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include <pdl_module.h>
#include "DetailView.h"
#include <pdl_file.h>
#include <ExDispid.h>

#include "resource.h"

///////////////////////////////////////////////////////////////////////////////
// CEventHandler

void CEventHandler::OnNavigateComplete(void)
{
}

void CEventHandler::OnSymbolChange(DWORD id)
{
}

void CEventHandler::OnNewFileDrop(LPCWSTR lpFileName)
{
}

///////////////////////////////////////////////////////////////////////////////
// CDetailView

CDetailView::CDetailView(void) : LAxCtrl(), m_pEventHandler(NULL)
                               , m_pCurSymbol(NULL)
{
    m_bEnable = TRUE;
}

void CDetailView::AddText(__in PCWSTR pszText)
{
    LBStr bsText = pszText;
    m_pBody->insertAdjacentHTML(L"beforeEnd", bsText);
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

    LBStr bsSel;
    pTxt->get_text(&bsSel);
    if (bsSel.IsEmpty())
        return;

    HGLOBAL hMem = GlobalAlloc(GHND | GMEM_SHARE,
        (bsSel.GetLength() + 1) * sizeof(WCHAR));
    PWSTR pStr = (PWSTR)GlobalLock(hMem);
    lstrcpyW(pStr, bsSel);
    GlobalUnlock(hMem);
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);
}

void CDetailView::CopyAll(void)
{
    LBStr bsSel;
    m_pBody->get_innerText(&bsSel);
    if (bsSel.IsEmpty())
        return;

    HGLOBAL hMem = GlobalAlloc(GHND | GMEM_SHARE,
        (bsSel.GetLength() + 1) * sizeof(WCHAR));
    PWSTR pStr = (PWSTR)GlobalLock(hMem);
    lstrcpyW(pStr, bsSel);
    GlobalUnlock(hMem);
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);
    CloseClipboard();
    GlobalFree(hMem);
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
    LBStr bsSel;
    m_pBody->get_innerText(&bsSel);
    if (bsSel.IsEmpty())
        return FALSE;

    *pStr = bsSel;
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

    QueryCtrl(IID_IWebBrowser2, (PVOID*)&m_pWB2);

    RECT rc;
    GetClientRect(&rc);
    m_pWB2->put_Left(rc.left);
    m_pWB2->put_Top(rc.top);
    m_pWB2->put_Width(rc.right);
    m_pWB2->put_Height(rc.bottom);

    WCHAR strExe[MAX_PATH];
    ::GetModuleFileNameW(NULL, strExe, MAX_PATH);

    LStringW strPage;
    strPage.Attach((PWSTR)lpCreateStruct->lpCreateParams);
    if (0 == strPage.GetLength())
        strPage.Format(L"res://%s/%d", strExe, IDR_HTML);

    LVariant url = strPage;
    m_pWB2->Navigate2(&url, NULL, NULL, NULL, NULL);

    LComPtr<IDispatch> pDisp = NULL;
    m_pWB2->get_Document(&pDisp);
    m_pDoc = pDisp;
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

///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDetailView::QueryInterface(REFIID iid, LPVOID* ppvObject)
{
    if (iid == IID_IDocHostUIHandler)
    {
        *ppvObject = (IDocHostUIHandler*)this;
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

STDMETHODIMP CDetailView::Invoke(
    DISPID dispIdMember,
    const IID &riid,
    LCID lcid,
    WORD wFlags,
    DISPPARAMS *pDispParams,
    VARIANT *pVarResult,
    EXCEPINFO *pExcepInfo,
    UINT *puArgErr)
{
    HRESULT hr = S_OK;
    switch (dispIdMember)
    {
    case DISPID_BEFORENAVIGATE2:
        {
            BeforeNavigate2(pDispParams->rgvarg[6].pdispVal,
                pDispParams->rgvarg[5].pvarVal, pDispParams->rgvarg[4].pvarVal,
                pDispParams->rgvarg[3].pvarVal, pDispParams->rgvarg[2].pvarVal,
                pDispParams->rgvarg[1].pvarVal, pDispParams->rgvarg[0].pboolVal);
        }
        break;
    case DISPID_NAVIGATECOMPLETE2:
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

void CDetailView::BeforeNavigate2(
    __in IDispatch* pDisp,
    __in VARIANT* URL,
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
    PWSTR pszSymbol = new WCHAR[len];
    ZeroMemory(pszSymbol, len * sizeof(WCHAR));
    lstrcpyW(pszSymbol, URL->bstrVal);

    if (LFile::Exists(pszSymbol, FALSE))
    {
        m_pEventHandler->OnNewFileDrop(pszSymbol);
    }
    else
    {
        PCWSTR p = wcsstr(pszSymbol, L"sym://");
        if (NULL != p)
        {
            DWORD id = _wtoi(p + 6);
            m_pEventHandler->OnSymbolChange(id);
        }
    }
    delete [] pszSymbol;
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

STDMETHODIMP CDetailView::ShowContextMenu(
    __in DWORD dwID,
    __in POINT *ppt,
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

STDMETHODIMP CDetailView::ShowUI(
    __in DWORD dwID,
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

STDMETHODIMP CDetailView::ResizeBorder(
    __in LPCRECT prcBorder,
    __in IOleInPlaceUIWindow *pUIWindow,
    __in BOOL fRameWindow)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::TranslateAccelerator(
    __in LPMSG lpMsg,
    __in const GUID *pguidCmdGroup,
    __in DWORD nCmdID)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetOptionKeyPath(
    __out LPOLESTR *pchKey,
    __in DWORD dw)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetDropTarget(
    __in IDropTarget *pDropTarget,
    __out IDropTarget **ppDropTarget)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::GetExternal(__out IDispatch **ppDispatch)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::TranslateUrl(
    __in DWORD dwTranslate,
    __in OLECHAR *pchURLIn,
    __out OLECHAR **ppchURLOut)
{
    return E_NOTIMPL;
}

STDMETHODIMP CDetailView::FilterDataObject(
    __in IDataObject *pDO,
    __out IDataObject **ppDORet)
{
    return E_NOTIMPL;
}
