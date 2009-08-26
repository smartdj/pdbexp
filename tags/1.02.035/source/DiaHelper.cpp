///////////////////////////////////////////////////////////////////////////////
// 文件名：  DiaHelper.cpp
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    DIA SDK辅助类实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include "DiaHelper.h"
#include <diacreate.h>
#include <PDLCom.h>

// 注意这里，DIA 各版本的 CLSID 是不同的
CLSID clsidDIA[] = {
    // DIA 8
    { 0xbce36434, 0x2c24, 0x499e,
    { 0xbf, 0x49, 0x8b, 0xd9, 0x9b, 0x0e, 0xeb, 0x68 } },
    // DIA 9
    { 0x4c41678e, 0x887b, 0x4365,
    { 0xa0, 0x9e, 0x92, 0x5d, 0x28, 0xdb, 0x33, 0xc2 } }
};

typedef struct _tagDumpParam {
    DumpEvent pfnCallBack;
    LPVOID    pParam;
} DUMPPARAM, *PDUMPPARAM;

typedef struct _tagModifyParam {
    ModifyEvent pfnCallBack;
    LPVOID      pParam;
} MODIFYPARAM, *PMODIFYPARAM;

CDiaHelper::CDiaHelper(void)
{
    m_ds       = NULL;
    m_pSession = NULL;
    m_pSymbol  = NULL;
}

CDiaHelper::~CDiaHelper(void)
{
    if (NULL != m_pSymbol)
        m_pSymbol->Release();
    if (NULL != m_pSession)
        m_pSession->Release();
    if (NULL != m_ds)
        m_ds->Release();
}

BOOL CDiaHelper::InitDia(void)
{
    HRESULT hr;
    for (int i = 0; i < sizeof(clsidDIA) / sizeof(GUID); ++i)
    {
        m_ds = NULL;
        hr = ::CoCreateInstance(clsidDIA[i], NULL,
            CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource),
            reinterpret_cast<LPVOID*>(&m_ds));
        if (SUCCEEDED(hr))
            return TRUE;
    }
    return FALSE;
}

BOOL CDiaHelper::OpenPDB(LPCWSTR lpPdbFile)
{
    if (NULL != m_pSymbol)
    {
        m_pSymbol->Release();
        m_pSymbol = NULL;
    }
    if (NULL != m_pSession)
    {
        m_pSession->Release();
        m_pSession = NULL;
    }
    if (NULL != m_ds)
    {
        // 装载PDB一次之后就不能再装载了，所以这里要重来
        m_ds->Release();
        m_ds = NULL;
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

BOOL CDiaHelper::cbDumpEnum(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    PDUMPPARAM pDumpParam = reinterpret_cast<PDUMPPARAM>(pParam);

    BSTR bsName = NULL;
    pCurSymbol->get_name(&bsName);

    VARIANT vt;
    pCurSymbol->get_value(&vt);
    DWORD dwValue = 0;
    switch (vt.vt)
    {
    case VT_UI1:    dwValue = vt.bVal;      break;
    case VT_UI2:    dwValue = vt.uiVal;     break;
    case VT_UI4:    dwValue = vt.ulVal;     break;
    case VT_UINT:   dwValue = vt.uintVal;   break;
    case VT_INT:    dwValue = vt.intVal;    break;
    case VT_I1:     dwValue = vt.cVal;      break;
    case VT_I2:     dwValue = vt.iVal;      break;
    case VT_I4:     dwValue = vt.lVal;      break;
    default:
        ; // TODO: Error!
    }

    LStringW str;
    str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;%s = 0x%x,<br>", bsName, dwValue);
    ::SysFreeString(bsName);

    pDumpParam->pfnCallBack(str, pDumpParam->pParam);

    return FALSE;
}

BOOL CDiaHelper::cbDumpMember(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    PDUMPPARAM pDumpParam = reinterpret_cast<PDUMPPARAM>(pParam);

    BSTR bsType = NULL, bsName = NULL, bsArray = NULL;
    LONG lOffset = 0;
    DWORD dwSize = 0, dwCnt = 0;
    GetSymbolTypeInfo(pCurSymbol, &bsType, &bsArray, &lOffset, &dwSize);
    pCurSymbol->get_name(&bsName);

    LStringW str;
    if (0 != lstrcmpW(bsName, L""))
    {
        str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;%s %s%s; "
            L"<font class=comment>// +0x%x(0x%x)</font><br>",
            bsType, bsName, bsArray, lOffset, dwSize);
    }
    else
    {
        str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;%s %s; "
            L"<font class=comment>// +0x%x(0x%x)</font><br>",
            bsType, bsName, lOffset, dwSize);
    }

    ::SysFreeString(bsArray);
    ::SysFreeString(bsName);
    ::SysFreeString(bsType);
    pDumpParam->pfnCallBack(str, pDumpParam->pParam);

    return FALSE;
}

BOOL CDiaHelper::cbFindSymbol(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    BSTR b = NULL;
    pCurSymbol->get_name(&b);

    LPCWSTR pszName = static_cast<LPCWSTR>(pParam);
    BOOL    bRet    = (0 == lstrcmpW(pszName, b));
    ::SysFreeString(b);

    return bRet;
}

void CDiaHelper::DumpEnumHeader(__in IDiaSymbol* pSymbol, __in DumpEvent pfnCallBack,
                          __in_opt LPVOID pParam)
{
    LStringW str;
    BSTR bsName = NULL;
    pSymbol->get_name(&bsName);

    str.Format(L"<font class=key>enum</font> %s<br>\r\n{<br>",
        bsName);
    ::SysFreeString(bsName);
    pfnCallBack(str, pParam);
}

void CDiaHelper::DumpSymbol(__in IDiaSymbol* pSymbol,
                            __in DWORD_PTR dwTypeInfo,
                            __in DumpEvent pfnCallBack,
                            __in_opt LPVOID pParam)
{
    enum SymTagEnum enTag   = static_cast<enum SymTagEnum>(HIWORD(dwTypeInfo));
    EnumEvent       pfnEnum = NULL;
    switch (enTag)
    {
    case SymTagUDT:
        {
            DumpUDTHeader(pSymbol, static_cast<UdtKind>(LOWORD(dwTypeInfo)),
                pfnCallBack, pParam);
            pfnEnum = cbDumpMember;
        }
        break;
    case SymTagEnum:
        {
            DumpEnumHeader(pSymbol, pfnCallBack, pParam);
            pfnEnum = cbDumpEnum;
        }
        break;
    default:
        {
            // TODO: not implemented
        }
    }

    // Members
    DUMPPARAM       param;
    param.pfnCallBack = pfnCallBack;
    param.pParam      = pParam;
    EnumTag(pSymbol, SymTagData, pfnEnum, &param, NULL);
    // Tail
    pfnCallBack(L"};", pParam);
}

void CDiaHelper::DumpUDTHeader(__in IDiaSymbol* pSymbol, __in UdtKind enKind,
                         __in DumpEvent pfnCallBack, __in_opt LPVOID pParam)
{
    LPCWSTR pszTmp = NULL;
    LStringW str;
    BSTR bsName = NULL;
    pSymbol->get_name(&bsName);
    ULONGLONG ulSize = 0;
    pSymbol->get_length(&ulSize);
    switch (enKind)
    {
    case UdtStruct:
        pszTmp = L"struct";
        break;
    case UdtClass:
        pszTmp = L"class";
        break;
    case UdtUnion:
        pszTmp = L"union";
        break;
    }
    str.Format(L"<font class=key>%s</font> %s "
        L"<font class=comment>// 0x%x</font><br>\r\n{<br>",
        pszTmp, bsName, static_cast<DWORD>(ulSize));
    ::SysFreeString(bsName);
    pfnCallBack(str, pParam);
}

BOOL CDiaHelper::EnumMemberModify(__in_opt IDiaSymbol* pSymbol,
                                  __in ModifyEvent pfnCallBack,
                                  __in DumpEvent pfnDump,
                                  __in LPVOID pParam)
{
    if (NULL == pfnCallBack || NULL == pSymbol)
        return FALSE;

    enum SymTagEnum          enTag = SymTagNull;
    pSymbol->get_symTag(reinterpret_cast<LPDWORD>(&enTag));

    LComPtr<IDiaEnumSymbols> pEnum = NULL;

    HRESULT hr = pSymbol->findChildren(SymTagData, NULL, nsNone, &pEnum);
    if (FAILED(hr))
        return FALSE;

    // 进行枚举
    int   idx   = 0;
    ULONG count = 0;
    LComPtr<IDiaSymbol> pCurItem = NULL;
    if (FAILED(pEnum->Next(1, &pCurItem, &count)))
        return FALSE;

    int      nUseless = 0;
    LStringW strUseless;
    LONG     lOffset  = -1;
    while (0 != count)
    {
        if (pfnCallBack(idx, pParam))
        {
            if (0 != idx && SymTagUDT == enTag && -1 != lOffset)
            {
                // 计算尺寸，这里不要用get_length，不可靠
                LONG lTmp = 0;
                pCurItem->get_offset(&lTmp);
                lTmp -=lOffset;
                // 把前一个useless的成员输出
                strUseless.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;"
                    L"UCHAR uUseless%d[0x%x]; "
                    L"<font class=comment>// +0x%x(0x%x)</font><br>",
                    nUseless, lTmp, lOffset, lTmp);
                pfnDump(strUseless, pParam);

                ++nUseless;
                lOffset  = -1;
            }
            // 输出当前符号
            DUMPPARAM param;
            param.pfnCallBack = pfnDump;
            param.pParam      = pParam;
            if (SymTagEnum == enTag)
                cbDumpEnum(pCurItem, &param);
            else if (SymTagUDT == enTag)
                cbDumpMember(pCurItem, &param);
            else
            {
                // TODO: not implemented
            }
        }
        else if (-1 == lOffset)
        {
            pCurItem->get_offset(&lOffset);
        }

        // next
        pCurItem.Release();
        pCurItem = NULL;
        if (FAILED(pEnum->Next(1, &pCurItem, &count)))
            return FALSE;
        ++idx;
    }
    if (SymTagUDT == enTag && -1 != lOffset)
    {
        // 输出最后一个useless成员
        ULONGLONG ulTmp = 0;
        pSymbol->get_length(&ulTmp);
        LONG lTmp = static_cast<LONG>(ulTmp) - lOffset;
        strUseless.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;"
            L"UCHAR uUseless%d[0x%x]; "
            L"<font class=comment>// +0x%x(0x%x)</font><br>",
            nUseless, lTmp, lOffset, lTmp);
        pfnDump(strUseless, pParam);
    }
    return TRUE;
}

BOOL CDiaHelper::EnumTag(__in_opt IDiaSymbol* pSymbol,
                         __in enum SymTagEnum enTag,
                         __in EnumEvent pfnCallBack,
                         __in_opt LPVOID pParam,
                         __out_opt IDiaSymbol** ppRet)
{
    if (NULL == pfnCallBack)
        return FALSE;
    if (NULL == pSymbol)
        pSymbol = m_pSymbol;

    LComPtr<IDiaEnumSymbols> pEnum = NULL;

    HRESULT hr = pSymbol->findChildren(enTag, NULL, nsNone, &pEnum);
    if (FAILED(hr))
        return FALSE;

    // 进行枚举
    ULONG count = 0;
    LComPtr<IDiaSymbol> pCurItem = NULL;
    if (FAILED(pEnum->Next(1, &pCurItem, &count)))
        return FALSE;

    while (0 != count)
    {
        if (pfnCallBack(pCurItem, pParam))
            break;

        // next
        pCurItem.Release();
        pCurItem = NULL;
        if (FAILED(pEnum->Next(1, &pCurItem, &count)))
            return FALSE;
    }
    if (NULL != pCurItem)
        *ppRet = pCurItem.Detach();
    return TRUE;
}

IDiaSymbol* CDiaHelper::FindSymbol(__in_opt IDiaSymbol* pParentSym,
                                   __in enum SymTagEnum enTag,
                                   __in LPCWSTR lpszName)
{
    IDiaSymbol* pRet = NULL;
    if (NULL == pParentSym)
        pParentSym = m_pSymbol;

    EnumTag(pParentSym, enTag, cbFindSymbol, (LPVOID)lpszName, &pRet );

    return pRet;
}

IDiaSymbol* CDiaHelper::GetSymbolBaseType(__in IDiaSymbol* pSymbol)
{
    IDiaSymbol* pType = NULL;
    pSymbol->get_type(&pType);

    BasicType enType = btNoType;
    pType->get_baseType(reinterpret_cast<LPDWORD>(&enType));
    if (btNoType != enType)
        return pType;

    enum SymTagEnum enTag;
    pType->get_symTag(reinterpret_cast<LPDWORD>(&enTag));
    switch (enTag)
    {
    case SymTagPointerType:
    case SymTagArrayType:
        {
            IDiaSymbol* pTmp = pType;
            pType = GetSymbolBaseType(pTmp);
            pTmp->Release();
        }
        break;
    }
    return pType;
}

BOOL CDiaHelper::GetSymbolTypeInfo(__in IDiaSymbol* pSymbol, __out BSTR* bsName,
                                   __out BSTR* bsArray, __out LPLONG pOffset,
                                   __out LPDWORD pSize)
{
    if (NULL == pSymbol || NULL == bsName)
        return FALSE;

    LComPtr<IDiaSymbol> pType = NULL;
    HRESULT             hr    = pSymbol->get_type(&pType);
    if (FAILED(hr))
        return FALSE;

    // Size and offset
    ULONGLONG ulSize = 0;
    hr               = pType->get_length(&ulSize);
    *pSize           = static_cast<DWORD>(ulSize);
    hr               = pSymbol->get_offset(pOffset);

    // Array
    *bsArray = ::SysAllocString(L"");

    // Type
    BSTR bsElemName = NULL;
    pSymbol->get_name(&bsElemName);
    GetSymbolTypeName(pType, bsElemName, bsName, bsArray);
    ::SysFreeString(bsElemName);
    return SUCCEEDED(hr);
}

BOOL CDiaHelper::GetSymbolTypeName(__in IDiaSymbol* pType, __in BSTR bsName,
                                   __out BSTR* bsType, __out BSTR* bsArray)
{
    BasicType enType = btNoType;
    HRESULT   hr     = pType->get_baseType(reinterpret_cast<LPDWORD>(&enType));
    if (btNoType != enType)
    {
        // 标准类型
        ULONGLONG ulSize  = 0;
        hr                = pType->get_length(&ulSize);
        LPCOLESTR pszType = L"BaseType";
        switch (enType)
        {
        case btVoid:
            pszType = L"<font class=key>void</font>";
            break;
        case btChar:
            pszType = L"CHAR";
            break;
        case btWChar:
            pszType = L"WCHAR";
            break;
        case btInt:
            {
                switch (ulSize)
                {
                case 1:
                    pszType = L"CHAR";
                    break;
                case 2:
                    pszType = L"SHORT";
                    break;
                case 4:
                    pszType = L"<font class=key>int</font>";
                    break;
                case 8:
                    pszType = L"<font class=key>__int64</font>";
                    break;
                }
            }
            break;
        case btUInt:
            {
                switch (ulSize)
                {
                case 1:
                    pszType = L"UCHAR";
                    break;
                case 2:
                    pszType = L"USHORT";
                    break;
                case 4:
                    pszType = L"<font class=key>unsigned int</font>";
                    break;
                case 8:
                    pszType = L"<font class=key>unsigned __int64</font>";
                    break;
                }
            }
            break;
        case btFloat:
            {
                switch (ulSize)
                {
                case 4:
                    pszType = L"<font class=key>float</font>";
                    break;
                case 8:
                    pszType = L"<font class=key>double</font>";
                    break;
                }
            }
            break;
        case btBCD:
            pszType = L"BCD";
            break;
        case btBool: 
            pszType = L"BOOLEAN";
            break;
        case btLong: 
            pszType = L"LONG";
            break;
        case btULong: 
            pszType = L"ULONG";
            break;
        case btCurrency: 
            pszType = L"CURRENCY";
            break;
        case btDate: 
            pszType = L"DATE";
            break;
        case btVariant: 
            pszType = L"VARIANT";
            break;
        case btComplex:
            pszType = L"COMPLEX";
            break;
        case btBit:
            pszType = L"BIT";
            break;
        case btBSTR:
            pszType = L"BSTR";
            break;
        case btHresult:
            pszType = L"HRESULT";
            break;
        }
        *bsType = ::SysAllocString(pszType);
    }
    else
    {
        enum SymTagEnum enTag;
        pType->get_symTag(reinterpret_cast<LPDWORD>(&enTag));

        LStringW strType;
        BSTR bsTypeTmp = NULL;
        switch (enTag)
        {
        case SymTagPointerType:
            {
                LComPtr<IDiaSymbol> pPointee = NULL;
                pType->get_type(&pPointee);
                GetSymbolTypeName(pPointee, bsName, &bsTypeTmp, bsArray);
                strType.Format(L"%s*", bsTypeTmp);
            }
            break;
        case SymTagArrayType:
            {
                LComPtr<IDiaSymbol> pMember = NULL;
                pType->get_type(&pMember);

                // 计算成员个数
                LStringW strArray;
                ULONGLONG ulSize = 0, ulUnit = 0;
                pType->get_length(&ulSize);
                pMember->get_length(&ulUnit);
                strArray.Format(L"[0x%x]%s",
                    static_cast<DWORD>(ulSize / ulUnit), *bsArray);

                ::SysFreeString(*bsArray);
                *bsArray = ::SysAllocString(strArray);
                GetSymbolTypeName(pMember, bsName, &bsTypeTmp, bsArray);
                strType = bsTypeTmp;
            }
            break;
        case SymTagFunctionType:
            {
                strType = L"<font class=key>void</font>";
            }
            break;
        case SymTagUDT:
            {
                LPCWSTR pszTmp = NULL;
                UdtKind enType = UdtStruct;
                pType->get_udtKind(reinterpret_cast<LPDWORD>(&enType));
                pType->get_name(&bsTypeTmp);
                switch (enType)
                {
                case UdtStruct:
                    pszTmp = L"struct";
                    break;
                case UdtClass:
                    pszTmp = L"class";
                    break;
                case UdtUnion:
                    pszTmp = L"union";
                    break;
                }
                strType.Format(L"<font class=key>%s</font> "
                    L"<a href=\"type://%s?%s\">%s</a>",
                    pszTmp, bsTypeTmp, bsName, bsTypeTmp);
            }
            break;
        default:
            {
                // TODO:
            }
        }
        *bsType = ::SysAllocString(strType);
        ::SysFreeString(bsTypeTmp);
    }
    return SUCCEEDED(hr);
}
