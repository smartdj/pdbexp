///////////////////////////////////////////////////////////////////////////////
// 文件名：  PEAnalyzer.h
// 创建时间：2008-10-30
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    PE 分析器
///////////////////////////////////////////////////////////////////////////////

#pragma once

typedef enum _tagSignType {
    ErrorSign = -1,
    NoSign = 0,
    CVSign
} SIGNTYPE;

class CPEAnalyzer
{
public:
    CPEAnalyzer(void);
    ~CPEAnalyzer(void);
public:
    BOOL Open(__in PCTSTR lpFileName);
    void Close(void);
    BOOL IsPE(void);
    SIGNTYPE GetSignature(__out PTSTR lpString, __out PTSTR lpPdbFile);
private:
    PVOID RvaToRaw(__in DWORD dwRva);
private:
    HANDLE m_hFile;
    HANDLE m_hMapping;
    PVOID m_pvBase;
};
