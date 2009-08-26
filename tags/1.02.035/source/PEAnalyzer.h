///////////////////////////////////////////////////////////////////////////////
// �ļ�����  PEAnalyzer.h
// ����ʱ�䣺2008-10-30
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// ˵����    PE ������
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_PEANALYZER
#define PDBEXP_PEANALYZER

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

#endif // PDBEXP_PEANALYZER
