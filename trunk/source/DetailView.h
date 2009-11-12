///////////////////////////////////////////////////////////////////////////////
// 文件名：  DetailView.h
// 创建时间：2007-11-2
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    符号详细信息类
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <pdl_window.h>
#include <pdl_com.h>
#include <ExDisp.h>
#include <MsHTML.h>
#include <MsHtmHst.h>
#include <dia2.h>
#include <cvconst.h>

class CEventHandler
{
public:
    virtual void OnSymbolChange(DWORD id);
    virtual void OnNavigateComplete(void);
    virtual void OnNewFileDrop(LPCWSTR lpFileName);
};

class CDetailView : public LAxCtrl, public IDocHostUIHandler
{
public:
    CDetailView(void);
public:
    void AddText(__in PCWSTR pszText);
    void Clear(void);
    void Copy(void);
    void CopyAll(void);
    IDiaSymbol* DetachCurrentSymbol(void);
    void EnableHyperLink(__in BOOL bEnable);
    IDiaSymbol* GetCurrentSymbol(void);
    BOOL GetText(__out LStringA* pStr);
    void SetCurrentSymbol(__in IDiaSymbol* pCurSymbol);
    BOOL SetEventHandler(__in CEventHandler* pEventHandler);
private:
    PDL_DECLARE_MSGMAP();
    DECLARE_CREATE_HANDLER(OnCreate);
    DECLARE_DESTROY_HANDLER(OnDestroy);
private:
    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(__in REFIID iid,
        __out LPVOID* ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
private:
    // IDispatch
    HRESULT STDMETHODCALLTYPE Invoke(__in DISPID dispIdMember, __in REFIID riid,
        __in LCID lcid, __in WORD wFlags, __inout DISPPARAMS *pDispParams,
        __out VARIANT *pVarResult, __out EXCEPINFO *pExcepInfo,
        __out UINT *puArgErr);
    void BeforeNavigate2(__in IDispatch* pDisp, __in VARIANT* URL,
        __in VARIANT* Flags, __in VARIANT* TargetFrameName,
        __in VARIANT* PostData, __in VARIANT* Headers,
        __inout VARIANT_BOOL* Cancel);
    void NavigateComplete2(__in IDispatch* pDisp, __in VARIANT* URL);
private:
    // IDocHostUIHandler
    HRESULT STDMETHODCALLTYPE ShowContextMenu(__in DWORD dwID, __in POINT *ppt,
        __in IUnknown *pcmdtReserved, __in IDispatch *pdispReserved);
    HRESULT STDMETHODCALLTYPE GetHostInfo(__inout DOCHOSTUIINFO *pInfo);
    HRESULT STDMETHODCALLTYPE ShowUI(__in DWORD dwID,
        __in IOleInPlaceActiveObject *pActiveObject,
        __in IOleCommandTarget *pCommandTarget, __in IOleInPlaceFrame *pFrame,
        __in IOleInPlaceUIWindow *pDoc);
    HRESULT STDMETHODCALLTYPE HideUI(void);
    HRESULT STDMETHODCALLTYPE UpdateUI(void);
    HRESULT STDMETHODCALLTYPE EnableModeless(__in BOOL fEnable);
    HRESULT STDMETHODCALLTYPE OnDocWindowActivate(__in BOOL fActivate);
    HRESULT STDMETHODCALLTYPE OnFrameWindowActivate(__in BOOL fActivate);
    HRESULT STDMETHODCALLTYPE ResizeBorder(__in LPCRECT prcBorder,
        __in IOleInPlaceUIWindow *pUIWindow, __in BOOL fRameWindow);
    HRESULT STDMETHODCALLTYPE TranslateAccelerator(__in LPMSG lpMsg,
        __in const GUID *pguidCmdGroup, __in DWORD nCmdID);
    HRESULT STDMETHODCALLTYPE GetOptionKeyPath(__out LPOLESTR *pchKey,
        __in DWORD dw);
    HRESULT STDMETHODCALLTYPE GetDropTarget(__in IDropTarget *pDropTarget,
        __out IDropTarget **ppDropTarget);
    HRESULT STDMETHODCALLTYPE GetExternal(__out IDispatch **ppDispatch);
    HRESULT STDMETHODCALLTYPE TranslateUrl(__in DWORD dwTranslate,
        __in OLECHAR *pchURLIn, __out OLECHAR **ppchURLOut);
    HRESULT STDMETHODCALLTYPE FilterDataObject(__in IDataObject *pDO,
        __out IDataObject **ppDORet);
private:
    LComPtr<IWebBrowser2> m_pWB2;
    LComQIPtr<IHTMLDocument2> m_pDoc;
    LComPtr<IHTMLElement> m_pBody;
    IDiaSymbol *m_pCurSymbol;
    CEventHandler *m_pEventHandler;
    BOOL m_bEnable;
};
