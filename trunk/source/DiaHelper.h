///////////////////////////////////////////////////////////////////////////////
// 文件名：  DiaHelper.h
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    DIA SDK辅助类
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
    // 初始化DIA库
    BOOL InitDia(void);
    // 打开PDB文件
    BOOL OpenPDB(__in LPCWSTR lpPdbFile);
    // 枚举某一类型的符号
    BOOL EnumTag(__in_opt IDiaSymbol* pSymbol, __in enum SymTagEnum enTag,
        __in EnumEvent pfnCallBack, __in_opt LPVOID pParam,
        __out_opt IDiaSymbol** ppRet);
    // 按名称查找某一类型的符号
    IDiaSymbol* FindSymbol(__in_opt IDiaSymbol* pParentSym, __in DWORD id);
private:
    static BOOL cbFindSymbol(IDiaSymbol* pCurSymbol, LPVOID pParam);
private:
    LComPtr<IDiaDataSource> m_ds;
    LComPtr<IDiaSession> m_pSession;
    SymPtr m_pSymbol;
};
