///////////////////////////////////////////////////////////////////////////////
// �ļ�����  DiaHelper.h
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    DIA SDK������
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
    // ��ʼ��DIA��
    BOOL InitDia(void);
    // ��PDB�ļ�
    BOOL OpenPDB(__in LPCWSTR lpPdbFile);
    // ���enum����
    void DumpEnumHeader(__in IDiaSymbol* pSymbol, __in DumpEvent pfnCallBack,
        __in_opt LPVOID pParam);
    // �������
    void DumpSymbol(__in IDiaSymbol* pSymbol, __in DWORD_PTR dwTypeInfo,
        __in DumpEvent pfnCallBack, __in_opt LPVOID pParam);
    // ���UDT����
    void DumpUDTHeader(__in IDiaSymbol* pSymbol, __in UdtKind enKind,
        __in DumpEvent pfnCallBack, __in_opt LPVOID pParam);
    // ö���޸ĺ�ķ���
    BOOL EnumMemberModify(__in_opt IDiaSymbol* pSymbol,
        __in ModifyEvent pfnCallBack, __in DumpEvent pfnDump,
        __in LPVOID pParam);
    // ö��ĳһ���͵ķ���
    BOOL EnumTag(__in_opt IDiaSymbol* pSymbol, __in enum SymTagEnum enTag,
        __in EnumEvent pfnCallBack, __in_opt LPVOID pParam,
        __out_opt IDiaSymbol** ppRet);
    // �����Ʋ���ĳһ���͵ķ���
    IDiaSymbol* FindSymbol(__in_opt IDiaSymbol* pParentSym,
        __in enum SymTagEnum enTag, __in LPCWSTR lpszName);
    // ��ȡĳһ���ŵĻ�������
    static IDiaSymbol* GetSymbolBaseType(__in IDiaSymbol* pSymbol);
    // ��ȡĳһ�������͵���Ϣ
    static BOOL GetSymbolTypeInfo(__in IDiaSymbol* pSymbol, __out BSTR* bsName,
        __out BSTR* bsArray, __out LPLONG pOffset, __out LPDWORD pSize);
    // ��ȥĳһ�������͵����Ƽ���Ա��
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
