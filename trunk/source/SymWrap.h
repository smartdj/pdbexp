///////////////////////////////////////////////////////////////////////////////
// FileName:    SymWrap.h
// Created:     2009/07/17
// Author:      titilima
// CopyRight:   Titi Studio (?) 2001-2009
//-----------------------------------------------------------------------------
// Information: IDiaSymbol ��������
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "diacommon.h"
#include <pdl_string.h>

typedef BOOL (*EnumProc)(IDiaSymbol* curSym, PVOID param);

class CSym
{
protected:
    CSym(__in IDiaSymbol* sym);
    ~CSym(void);
public:
    // ��ȡ���ŵ�������ʽ
    virtual void Declare(__out LStringW* str, __in PCWSTR lpName);
    // ����һ������
    static void Delete(__in CSym* sym);
    // ö��ָ�����ķ���
    static IDiaSymbol* Enum(__in IDiaSymbol* symParent,
        __in enum SymTagEnum enTag, __in EnumProc cbEnum,
        __in_opt PVOID param);
    // ��ʽ������
    virtual BOOL Format(__out LStringW* str);
    // ��ȡͷ��
    virtual BOOL GetHeader(__out LStringW* str);
    // ��ȡ���ŵ�������
    virtual BOOL GetType(__out LStringW* str);
    // ����һ������
    static CSym* NewSym(__in IDiaSymbol* sym);
    // ��ȡ���ŵ� typedef ��Ϣ
    virtual void TypeDefine(__out LStringW* str, __in PCWSTR lpType);
protected:
    IDiaSymbol* m_sym;
};

class CSymFunction : public CSym
{
    friend class CSym;
protected:
    CSymFunction(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
};

class CSymData : public CSym
{
    friend class CSym;
protected:
    CSymData(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
};

class CSymUDT : public CSym
{
    friend class CSym;
    struct SymAccess
    {
        LStringW strPublic;
        LStringW strProtected;
        LStringW strPrivate;
    };
protected:
    CSymUDT(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
    BOOL GetHeader(__out LStringW* str);
    BOOL GetType(__out LStringW* str);
private:
    static BOOL EnumBase(IDiaSymbol* sym, PVOID param);
    static BOOL EnumMember(IDiaSymbol* sym, PVOID param);
};

class CSymEnum : public CSym
{
    friend class CSym;
protected:
    CSymEnum(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
    BOOL GetHeader(__out LStringW* str);
    BOOL GetType(__out LStringW* str);
private:
    static BOOL OnEnum(IDiaSymbol* sym, PVOID param);
};

class CSymFunctionType : public CSym
{
    friend class CSym;
protected:
    CSymFunctionType(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    void Declare(__out LStringW* str, __in PCWSTR lpName);
    static PCWSTR GetCallType(IDiaSymbol* sym);
private:
    static BOOL EnumArg(IDiaSymbol* sym, PVOID param);
};

class CSymPointerType : public CSym
{
    friend class CSym;
protected:
    CSymPointerType(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    void Declare(__out LStringW* str, __in PCWSTR lpName);
    BOOL GetType(__out LStringW* str);
    void TypeDefine(__out LStringW* str, __in PCWSTR lpType);
};

class CSymArrayType : public CSym
{
    friend class CSym;
protected:
    CSymArrayType(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    void Declare(__out LStringW* str, __in PCWSTR lpName);
};

class CSymBaseType : public CSym
{
    friend class CSym;
protected:
    CSymBaseType(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL GetType(__out LStringW* str);
};


class CSymTypedef : public CSym
{
    friend class CSym;
protected:
    CSymTypedef(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
};

class CSymFunctionArgType : public CSym
{
    friend class CSym;
protected:
    CSymFunctionArgType(__in IDiaSymbol* sym) : CSym(sym) { /* Nothing */ }
public:
    BOOL Format(__out LStringW* str);
};
