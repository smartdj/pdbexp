///////////////////////////////////////////////////////////////////////////////
// 文件名：  DownLoader.cpp
// 创建时间：2008-10-30
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    文件下载类实现
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "DownLoader.h"

#define BUF_SIZE    4096

CDownLoader::CDownLoader(__in PCTSTR pszAgent)
{
    m_hSession = ::InternetOpen(pszAgent, INTERNET_OPEN_TYPE_PRECONFIG,
        NULL, NULL, 0);

    m_proxyType = UnknownProxy;
    m_proxyUserName = NULL;
    m_proxyPassWord = NULL;
}

CDownLoader::~CDownLoader(void)
{
    if (NULL != m_hSession)
        ::InternetCloseHandle(m_hSession);

    if (NULL != m_proxyUserName)
        delete [] m_proxyUserName;
    if (NULL != m_proxyPassWord)
        delete [] m_proxyPassWord;
}

DOWNERROR CDownLoader::DownLoadFile(
     __in PCTSTR lpszServerName,
     __in INTERNET_PORT nServerPort,
     __in PCTSTR lpSrcFile,
     __in PCTSTR lpDstFile)
{
    if (NULL == m_hSession)
        return DownDisconnected;

    HINTERNET hConnect = ::InternetConnect(m_hSession, lpszServerName,
        nServerPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (NULL == hConnect)
        return DownDisconnected;

    PCTSTR accept[] = { _T("accept: */*"), NULL };
    HINTERNET hRequest = ::HttpOpenRequest(hConnect, _T("GET"), lpSrcFile, NULL, NULL,
        accept, 0, 0);
    if (NULL == hRequest)
    {
        ::InternetCloseHandle(hConnect);
        return DownDisconnected;
    }

    if (UseProxy == m_proxyType)
        SetProxy(hRequest);

    if (!::HttpSendRequest(hRequest, NULL, 0, NULL, 0))
    {
        ::InternetCloseHandle(hRequest);
        ::InternetCloseHandle(hConnect);
        return DownDisconnected;
    }

    DWORD dwCode = 0;
    DWORD dwSize = 0;
    if (UnknownProxy == m_proxyType)
    {
        dwSize = sizeof(DWORD);
        HttpQueryInfo (hRequest,
            HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
            &dwCode, &dwSize, NULL);
        if (HTTP_STATUS_PROXY_AUTH_REQ == dwCode)
            m_proxyType = UseProxy;
        else
            m_proxyType = NoProxy;

        SaveProxy(hRequest);
        SetProxy(hRequest);
        ::HttpSendRequest(hRequest, NULL, 0, NULL, 0);
    }

    HttpQueryInfo (hRequest,
        HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
        &dwCode, &dwSize, NULL);
    if (HTTP_STATUS_NOT_FOUND == dwCode)
    {
        ::InternetCloseHandle(hRequest);
        ::InternetCloseHandle(hConnect);
        return DownFileNotExists;
    }

    HANDLE hFile = CreateFile(lpDstFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
        FILE_ATTRIBUTE_ARCHIVE, NULL);
    if (INVALID_HANDLE_VALUE == hFile)
    {
        ::InternetCloseHandle(hRequest);
        ::InternetCloseHandle(hConnect);
        return DownCannotOpenFile;
    }

    PBYTE buf = new BYTE[BUF_SIZE];
    DWORD dw, dwWritten;
    while (::InternetReadFile(hRequest, buf, BUF_SIZE, &dw) && dw > 0)
    {
        ::WriteFile(hFile, buf, dw, &dwWritten, NULL);
    }

    ::CloseHandle(hFile);
    ::InternetCloseHandle(hRequest);
    ::InternetCloseHandle(hConnect);
    delete [] buf;
    return DownOK;
}

PCTSTR CDownLoader::QueryString(__in HINTERNET hInternet, __in DWORD dwOption)
{
    DWORD dwSize = 0;
    ::InternetQueryOption(hInternet, dwOption, NULL, &dwSize);

    PCTSTR ret = new TCHAR[dwSize / sizeof(TCHAR)];
    ::InternetQueryOption(hInternet, dwOption, (PVOID)ret, &dwSize);
    return ret;
}

BOOL CDownLoader::SaveProxy(__in HINTERNET hRequest)
{
    ::InternetErrorDlg(::GetDesktopWindow(),
        hRequest, ERROR_INTERNET_INCORRECT_PASSWORD,
        FLAGS_ERROR_UI_FILTER_FOR_ERRORS
        | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA
        | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, NULL);

    // 保存代理信息
    m_proxyUserName = QueryString(hRequest, INTERNET_OPTION_PROXY_USERNAME);
    m_proxyPassWord = QueryString(hRequest, INTERNET_OPTION_PROXY_PASSWORD);
    m_proxyType = UseProxy;
    return TRUE;
}

void CDownLoader::SetProxy(__in HINTERNET hRequest)
{
    ::InternetSetOption(hRequest, INTERNET_OPTION_PROXY_USERNAME,
        (PVOID)m_proxyUserName, lstrlen(m_proxyUserName) * sizeof(TCHAR));
    ::InternetSetOption(hRequest, INTERNET_OPTION_PROXY_PASSWORD,
        (PVOID)m_proxyPassWord, lstrlen(m_proxyUserName) * sizeof(TCHAR));
}
