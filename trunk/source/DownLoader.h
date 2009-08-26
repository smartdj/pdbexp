///////////////////////////////////////////////////////////////////////////////
// �ļ�����  DownLoader.h
// ����ʱ�䣺2008-10-30
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// ˵����    �ļ�������
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <WinInet.h>

typedef enum DownError {
    DownOK = 0,        // �ɹ��������
    DownDisconnected,  // �Ͽ�����
    DownFileNotExists, // �Ҳ����ļ�
    DownCannotOpenFile // �޷����ļ�
} DOWNERROR;

typedef enum ProxyType {
    UnknownProxy,
    NoProxy,
    UseProxy
} PROXYTYPE;

class CDownLoader
{
public:
    CDownLoader(__in PCTSTR pszAgent);
    ~CDownLoader(void);
public:
    DOWNERROR DownLoadFile(
        __in PCTSTR lpszServerName,
        __in INTERNET_PORT nServerPort,
        __in PCTSTR lpSrcFile,
        __in PCTSTR lpDstFile);
private:
    PCTSTR QueryString(__in HINTERNET hInternet, __in DWORD dwOption);
    BOOL SaveProxy(__in HINTERNET hRequest);
    void SetProxy(__in HINTERNET hRequest);
private:
    HINTERNET m_hSession;
    PROXYTYPE m_proxyType;
    PCTSTR m_proxyUserName;
    PCTSTR m_proxyPassWord;
};
