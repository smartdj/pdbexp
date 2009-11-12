///////////////////////////////////////////////////////////////////////////////
// 文件名：  DownDlg.cpp
// 创建时间：2008-11-17
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2008
//-----------------------------------------------------------------------------
// 说明：    下载对话框实现
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "DownDlg.h"
#include <pdl_comdlg.h>
#include <ShlObj.h>

#include "resource.h"

#define SIGN_LEN    64
#define BUF_SIZE    4096

CDownDlg::CDownDlg(__in CDownLoader* pDnLdr, __in LIniParser* pIni) : LDialog(IDD_DLG_DOWNLOAD)
{
    m_pDnLdr = pDnLdr;
    m_pIni = pIni;

    m_type = ErrorSign;
    m_pSign = new TCHAR[SIGN_LEN];
    m_pFile = new TCHAR[MAX_PATH];
}

CDownDlg::~CDownDlg(void)
{
    if (NULL != m_pSign)
        delete [] m_pSign;
    if (NULL != m_pFile)
        delete [] m_pFile;
}

void CDownDlg::AddInfo(__in PCTSTR lpInfo)
{
    int len = m_edtInfo.GetWindowTextLength();
    m_edtInfo.SetSel(len, -1);
    m_edtInfo.ReplaceSel(lpInfo);
}

void CDownDlg::ClearInfo(void)
{
    m_edtInfo.SetWindowText(_T(""));
}

///////////////////////////////////////////////////////////////////////////////

PDL_BEGIN_MSGMAP(CDownDlg)
    PROCESS_CLOSE(OnClose)
    PROCESS_COMMAND(OnCommand)
    PROCESS_INITDIALOG(OnInitDialog)
PDL_END_MSGMAP(LDialog)

void CDownDlg::OnCommand(
    WORD wNotifyCode,
    WORD wID,
    HWND hWndCtrl,
    BOOL& bHandled)
{
    if (EN_UPDATE == wNotifyCode)
    {
        OnEditUpdate(wID);
        return;
    }

    switch (wID)
    {
    case IDC_BTN_BROWSE1:   OnBtnOpen1();       break;
    case IDC_BTN_DOWNLOAD:  OnBtnDownLoad();    break;
    case IDOK:              OnOK();             break;
    default:
        bHandled = FALSE;
    }
}

void CDownDlg::OnEditUpdate(WORD wID)
{
    if (IDC_EDIT_FILE == wID)
    {
        ClearInfo();

        TCHAR str[MAX_PATH];
        GetDlgItemText(IDC_EDIT_FILE, str, MAX_PATH);

        CPEAnalyzer pe;
        pe.Open(str);
        m_type = pe.GetSignature(m_pSign, m_pFile);

        BOOL bEnable = TRUE;
        switch (m_type)
        {
        case ErrorSign:
            {
                lstrcpy(str, _T("ERROR: 打开文件错误。\r\n"));
                bEnable = FALSE;
            }
            break;
        case NoSign:
            {
                lstrcpy(str, _T("ERROR: 无签名信息。\r\n"));
                bEnable = FALSE;
            }
            break;
        case CVSign:
            {
                wsprintf(str, _T("PDB 签名: %s\r\n"), m_pSign);
                EnableDlgItem(IDC_BTN_DOWNLOAD);
            }
            break;
        }

        EnableDlgItem(IDC_BTN_DOWNLOAD, bEnable);
        AddInfo(str);
    }
}

void CDownDlg::OnBtnOpen1(void)
{
    LFileDialog dlg(TRUE,
        _T("所有支持格式\0*.exe;*.sys;*.dll;*.ocx;\0")
        _T("可执行文件\0*.exe\0")
        _T("系统文件\0*.sys\0")
        _T("动态链接库\0*.dll\0")
        _T("ActiveX 控件\0*.ocx\0")
        _T("\0"));
    if (dlg.DoModal(m_hWnd))
        SetDlgItemText(IDC_EDIT_FILE, dlg.m_szFileName);
}

void CDownDlg::OnBtnDownLoad(void)
{
    AddInfo(_T("开始下载..."));
    DWORD dwLen = lstrlen(m_pSign) + lstrlen(m_pFile) * 2 + 20;

    PTSTR lpURL = new TCHAR[dwLen];
    wsprintf(lpURL, _T("download/symbols/%s/%s/%s"), m_pFile, m_pSign, m_pFile);
    lpURL[dwLen - 2] = _T('_');

    // 获取目标文件路径
    TCHAR str[MAX_PATH];
    GetDlgItemText(IDC_EDIT_FILE, str, MAX_PATH);
    *(_tcsrchr(str, _T('\\')) + 1) = _T('\0');

    LFileDialog dlg(FALSE, _T("*.pdb\0*.pdb\0\0"), _T("*.pdb"));
    dlg.m_ofn->lpstrInitialDir = str;
    lstrcpy(dlg.m_szFileName, str);
    lstrcat(dlg.m_szFileName, m_pFile);
    if (!dlg.DoModal(m_hWnd))
    {
        AddInfo(_T("下载已取消。"));
        delete [] lpURL;
        return;
    }

    lstrcat(str, _T("download.tmp"));
    DOWNERROR ret = m_pDnLdr->DownLoadFile(_T("msdl.microsoft.com"),
        INTERNET_DEFAULT_HTTP_PORT, lpURL, str);
    switch (ret)
    {
    case DownOK:
        AddInfo(_T("下载完成。\r\n\r\n"));
        break;
    case DownDisconnected:
        AddInfo(_T("\r\nERROR: 无法连接服务器。\r\n\r\n"));
        break;
    case DownFileNotExists:
        AddInfo(_T("\r\nERROR: 远程文件不存在。\r\n\r\n"));
        break;
    case DownCannotOpenFile:
        AddInfo(_T("\r\nERROR: 无法创建本地文件。\r\n\r\n"));
        break;
    }

    if (DownOK != ret)
    {
        delete [] lpURL;
        return;
    }

    // 生成命令行
    LPTSTR lpCmd = new TCHAR[32 + lstrlen(str) + lstrlen(dlg.m_szFileName)];
    wsprintf(lpCmd, _T("expand.exe \"%s\" \"%s\""),
        str, dlg.m_szFileName);

    // 创建通信管道
    SECURITY_ATTRIBUTES sa;
    ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    HANDLE hReadPipe, hWritePipe;
    CreatePipe(&hReadPipe, &hWritePipe, &sa, BUF_SIZE);

    // 创建目标进程
    STARTUPINFO sui;
    ZeroMemory(&sui, sizeof(STARTUPINFO));
    sui.cb = sizeof(STARTUPINFO);
    sui.dwFlags = STARTF_USESTDHANDLES;
    sui.hStdError = hWritePipe;
    sui.hStdOutput = hWritePipe;
    PROCESS_INFORMATION pi;
    if (CreateProcess(NULL, lpCmd, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL,
        &sui, &pi))
    {
        // 捕获所有进程输出
        DWORD dw;
        PSTR buf = new char[BUF_SIZE];
        ZeroMemory(buf, BUF_SIZE);
        ReadFile(hReadPipe, buf, BUF_SIZE, &dw, NULL);
        m_edtInfo.SetSelA(-1, -1);
        m_edtInfo.ReplaceSel(buf);
        delete [] buf;

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        // 删除 download.error 临时文件
        DeleteFile(str);
        EnableDlgItem(IDC_BTN_DOWNLOAD, FALSE);
    }
    else
    {
        AddInfo(_T("ERROR: 解压文件失败。\r\n\r\n"));
    }

    CloseHandle(hWritePipe);
    CloseHandle(hReadPipe);
    delete [] lpCmd;
    delete [] lpURL;
}

void CDownDlg::OnOK(void)
{
    EndDialog(0);
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDownDlg::OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled)
{
    m_edtInfo = GetDlgItem(IDC_EDIT_INFO);
    EnableDlgItem(IDC_BTN_DOWNLOAD, FALSE);

    TCHAR szDefault[MAX_PATH];
    GetModuleFileName(NULL, szDefault, MAX_PATH);
    *_tcsrchr(szDefault, _T('\\')) = _T('\0');

    return TRUE;
}

void CDownDlg::OnClose(BOOL& bHandled)
{
    EndDialog(0);
}
