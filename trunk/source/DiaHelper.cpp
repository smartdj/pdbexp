///////////////////////////////////////////////////////////////////////////////
// �ļ�����  DiaHelper.cpp
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    DIA SDK������ʵ��
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "DiaHelper.h"
#include <diacreate.h>
#include <pdl_com.h>
#include "SymWrap.h"

// ע�����DIA ���汾�� CLSID �ǲ�ͬ��
CLSID clsidDIA[] = {
    // DIA 8
    { 0xbce36434, 0x2c24, 0x499e,
    { 0xbf, 0x49, 0x8b, 0xd9, 0x9b, 0x0e, 0xeb, 0x68 } },
    // DIA 9
    { 0x4c41678e, 0x887b, 0x4365,
    { 0xa0, 0x9e, 0x92, 0x5d, 0x28, 0xdb, 0x33, 0xc2 } }
};

typedef struct _tagModifyParam {
    ModifyEvent pfnCallBack;
    LPVOID      pParam;
} MODIFYPARAM, *PMODIFYPARAM;

CDiaHelper::CDiaHelper(void)
{
}

CDiaHelper::~CDiaHelper(void)
{
}

BOOL CDiaHelper::InitDia(void)
{
    HRESULT hr;
    for (int i = 0; i < sizeof(clsidDIA) / sizeof(GUID); ++i)
    {
        m_ds = NULL;
        hr = ::CoCreateInstance(clsidDIA[i], NULL, CLSCTX_INPROC_SERVER,
            IID_IDiaDataSource, (PVOID*)&m_ds);
        if (SUCCEEDED(hr))
            return TRUE;
    }
    return FALSE;
}

BOOL CDiaHelper::OpenPDB(LPCWSTR lpPdbFile)
{
    if (NULL != m_pSymbol)
        m_pSymbol.Release();
    if (NULL != m_pSession)
        m_pSession.Release();
    if (NULL != m_ds)
    {
        // װ��PDBһ��֮��Ͳ�����װ���ˣ���������Ҫ����
        m_ds.Release();
        InitDia();
    }

    HRESULT hr = m_ds->loadDataFromPdb(lpPdbFile);
    if (FAILED(hr))
        return FALSE;

    hr = m_ds->openSession(&m_pSession);
    if (FAILED(hr))
        return FALSE;

    hr = m_pSession->get_globalScope(&m_pSymbol);
    if (FAILED(hr))
        return FALSE;

    return TRUE;
}

BOOL CDiaHelper::cbFindSymbol(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    DWORD id;
    pCurSymbol->get_symIndexId(&id);
    return id == (DWORD)pParam;
}

BOOL CDiaHelper::EnumTag(
    __in_opt IDiaSymbol* pSymbol,
    __in enum SymTagEnum enTag,
    __in EnumEvent pfnCallBack,
    __in_opt LPVOID pParam,
    __out_opt IDiaSymbol** ppRet)
{
    if (NULL == pfnCallBack)
        return FALSE;
    if (NULL == pSymbol)
        pSymbol = m_pSymbol;

    IDiaSymbol* sym = CSym::Enum(pSymbol, enTag, pfnCallBack, pParam);
    if (PDL_ARGUMENT_PRESENT(ppRet))
        *ppRet = sym;
    return TRUE;
}

IDiaSymbol* CDiaHelper::FindSymbol(
    __in_opt IDiaSymbol* pParentSym,
    __in DWORD id)
{
    if (NULL == pParentSym)
        pParentSym = m_pSymbol;
    return CSym::Enum(pParentSym, SymTagNull, cbFindSymbol, (PVOID)id);
}
