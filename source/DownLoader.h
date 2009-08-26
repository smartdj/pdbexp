///////////////////////////////////////////////////////////////////////////////
// 文件名：  DownLoader.h
// 创建时间：2008-10-30
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    文件下载类
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <WinInet.h>

typedef enum DownError {
    DownOK = 0,        // 成功完成下载
    DownDisconnected,  // 断开连接
    DownFileNotExists, // 找不到文件
    DownCannotOpenFile // 无法打开文件
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
