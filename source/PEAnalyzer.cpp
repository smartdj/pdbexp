///////////////////////////////////////////////////////////////////////////////
// 文件名：  PEAnalyzer.cpp
// 创建时间：2008-10-30
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    PE 分析器实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include "PEAnalyzer.h"

#define CONVERT(Type, Base, Offset) (Type)((DWORD_PTR)(Base) + (Offset))

typedef struct _tagCV_RSDS {
    DWORD dwHeader;
    GUID Signature;
    DWORD dwAge;
    CHAR pdb[1];
} CV_RSDS, *PCV_RSDS;

CPEAnalyzer::CPEAnalyzer(void)
{
    m_hFile = INVALID_HANDLE_VALUE;
    m_hMapping = INVALID_HANDLE_VALUE;
    m_pvBase = NULL;
}

CPEAnalyzer::~CPEAnalyzer(void)
{
    Close();
}

BOOL CPEAnalyzer::Open(__in PCTSTR lpFileName)
{
    if (INVALID_HANDLE_VALUE != m_hFile)
        return FALSE;

    m_hFile = ::CreateFile(lpFileName, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, 0, NULL);
    if (INVALID_HANDLE_VALUE == m_hFile)
        return FALSE;

    m_hMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (INVALID_HANDLE_VALUE == m_hMapping)
    {
        ::CloseHandle(m_hFile);
        return FALSE;
    }

    m_pvBase = ::MapViewOfFile(m_hMapping, FILE_MAP_READ, 0, 0, 0);
    if (NULL == m_pvBase)
    {
        ::CloseHandle(m_hMapping);
        ::CloseHandle(m_hFile);
        return FALSE;
    }
    return TRUE;
}

void CPEAnalyzer::Close(void)
{
    if (NULL != m_pvBase)
    {
        ::UnmapViewOfFile(m_pvBase);
        ::CloseHandle(m_hMapping);
        ::CloseHandle(m_hFile);

        m_pvBase = NULL;
        m_hMapping = INVALID_HANDLE_VALUE;
        m_hFile = INVALID_HANDLE_VALUE;
    }
}

BOOL CPEAnalyzer::IsPE(void)
{
    if (NULL == m_pvBase)
        return FALSE;

    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)m_pvBase;
    if ('ZM' != pDosHdr->e_magic)
        return FALSE;

    PIMAGE_NT_HEADERS pNtHdr = CONVERT(PIMAGE_NT_HEADERS, m_pvBase,
        pDosHdr->e_lfanew);
    if (0x00004550 == pNtHdr->Signature) // 'PE\0\0'
        return TRUE;
    else
        return FALSE;
}

SIGNTYPE CPEAnalyzer::GetSignature(__out PTSTR lpString, __out PTSTR lpPdbFile)
{
    if (!IsPE())
        return ErrorSign;

    // PE Header
    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)m_pvBase;
    PIMAGE_NT_HEADERS pNtHdr = CONVERT(PIMAGE_NT_HEADERS, m_pvBase,
        pDosHdr->e_lfanew);

    // Debug Directory
    PIMAGE_DATA_DIRECTORY pDataDir = &pNtHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
    if (0 == pDataDir->VirtualAddress || 0 == pDataDir->Size)
        return NoSign;

    PIMAGE_DEBUG_DIRECTORY pDebugDir = (PIMAGE_DEBUG_DIRECTORY)RvaToRaw(pDataDir->VirtualAddress);
    SIGNTYPE ret = NoSign;
    switch (pDebugDir->Type)
    {
    case IMAGE_DEBUG_TYPE_CODEVIEW:
        {
            PCV_RSDS pCV = (PCV_RSDS)RvaToRaw(pDebugDir->AddressOfRawData);
            if ('SDSR' != pCV->dwHeader)
                break;

            wsprintf(lpString,
                _T("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X%X"),
                pCV->Signature.Data1,
                pCV->Signature.Data2,
                pCV->Signature.Data3,
                pCV->Signature.Data4[0], pCV->Signature.Data4[1],
                pCV->Signature.Data4[2], pCV->Signature.Data4[3],
                pCV->Signature.Data4[4], pCV->Signature.Data4[5],
                pCV->Signature.Data4[6], pCV->Signature.Data4[7],
                pCV->dwAge);
            ret = CVSign;
#ifdef UNICODE
            MultiByteToWideChar(CP_ACP, 0, pCV->pdb, -1, lpPdbFile, MAX_PATH);
#else
            lstrcpyA(lpPdbFile, pCV->pdb);
#endif // UNICODE
        }
        break;
    }
    return ret;
}

PVOID CPEAnalyzer::RvaToRaw(__in DWORD dwRva)
{
    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)m_pvBase;
    PIMAGE_NT_HEADERS pNtHdr = CONVERT(PIMAGE_NT_HEADERS, m_pvBase,
        pDosHdr->e_lfanew);

    PIMAGE_SECTION_HEADER pSecHdr = IMAGE_FIRST_SECTION(pNtHdr);
    for (USHORT i = 0; i < pNtHdr->FileHeader.NumberOfSections; ++i)
    {
        if (dwRva >= pSecHdr->VirtualAddress
            && dwRva < pSecHdr->VirtualAddress + pSecHdr->Misc.VirtualSize)
        {
            return (PVOID)(dwRva - pSecHdr->VirtualAddress
                + pSecHdr->PointerToRawData
                + (ULONG_PTR)m_pvBase);
        }
        ++pSecHdr;
    }
    return NULL;
}
