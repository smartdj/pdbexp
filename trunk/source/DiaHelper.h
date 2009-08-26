///////////////////////////////////////////////////////////////////////////////
// �ļ�����  DiaHelper.h
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    DIA SDK������
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "diacommon.h"

typedef BOOL (*EnumEvent)(IDiaSymbol* pCurSymbol, PVOID pParam);
typedef BOOL (*ModifyEvent)(int index, PVOID pParam);

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
    // ö��ĳһ���͵ķ���
    BOOL EnumTag(__in_opt IDiaSymbol* pSymbol, __in enum SymTagEnum enTag,
        __in EnumEvent pfnCallBack, __in_opt LPVOID pParam,
        __out_opt IDiaSymbol** ppRet);
    // �����Ʋ���ĳһ���͵ķ���
    IDiaSymbol* FindSymbol(__in_opt IDiaSymbol* pParentSym, __in DWORD id);
private:
    static BOOL cbFindSymbol(IDiaSymbol* pCurSymbol, LPVOID pParam);
private:
    LComPtr<IDiaDataSource> m_ds;
    LComPtr<IDiaSession> m_pSession;
    SymPtr m_pSymbol;
};
