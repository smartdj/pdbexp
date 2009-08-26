///////////////////////////////////////////////////////////////////////////////
// 文件名：  DiaHelper.h
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    DIA SDK辅助类
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_DIAHELPER
#define PDBEXP_DIAHELPER

#include <dia2.h>
#include <cvconst.h>

typedef void (*DumpEvent)(LPCWSTR pszString, LPVOID pParam);
typedef BOOL (*EnumEvent)(IDiaSymbol* pCurSymbol, LPVOID pParam);
typedef BOOL (*ModifyEvent)(int index, LPVOID pParam);

class CDiaHelper
{
public:
    CDiaHelper(void);
    ~CDiaHelper(void);
public:
    // 初始化DIA库
    BOOL InitDia(void);
    // 打开PDB文件
    BOOL OpenPDB(__in LPCWSTR lpPdbFile);
    // 输出enum符号
    void DumpEnumHeader(__in IDiaSymbol* pSymbol, __in DumpEvent pfnCallBack,
        __in_opt LPVOID pParam);
    // 输出符号
    void DumpSymbol(__in IDiaSymbol* pSymbol, __in DWORD_PTR dwTypeInfo,
        __in DumpEvent pfnCallBack, __in_opt LPVOID pParam);
    // 输出UDT符号
    void DumpUDTHeader(__in IDiaSymbol* pSymbol, __in UdtKind enKind,
        __in DumpEvent pfnCallBack, __in_opt LPVOID pParam);
    // 枚举修改后的符号
    BOOL EnumMemberModify(__in_opt IDiaSymbol* pSymbol,
        __in ModifyEvent pfnCallBack, __in DumpEvent pfnDump,
        __in LPVOID pParam);
    // 枚举某一类型的符号
    BOOL EnumTag(__in_opt IDiaSymbol* pSymbol, __in enum SymTagEnum enTag,
        __in EnumEvent pfnCallBack, __in_opt LPVOID pParam,
        __out_opt IDiaSymbol** ppRet);
    // 按名称查找某一类型的符号
    IDiaSymbol* FindSymbol(__in_opt IDiaSymbol* pParentSym,
        __in enum SymTagEnum enTag, __in LPCWSTR lpszName);
    // 获取某一符号的基础类型
    static IDiaSymbol* GetSymbolBaseType(__in IDiaSymbol* pSymbol);
    // 获取某一符号类型的信息
    static BOOL GetSymbolTypeInfo(__in IDiaSymbol* pSymbol, __out BSTR* bsName,
        __out BSTR* bsArray, __out LPLONG pOffset, __out LPDWORD pSize);
    // 后去某一符号类型的名称及成员数
    static BOOL GetSymbolTypeName(__in IDiaSymbol* pType, __in BSTR bsName,
        __out BSTR* bsType, __out BSTR* bsArray);
private:
    static BOOL cbDumpEnum(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbDumpMember(IDiaSymbol* pCurSymbol, LPVOID pParam);
    static BOOL cbFindSymbol(IDiaSymbol* pCurSymbol, LPVOID pParam);
private:
    IDiaDataSource* m_ds;
    IDiaSession*    m_pSession;
    IDiaSymbol*     m_pSymbol;
};

#endif // PDBEXP_DIAHELPER
