///////////////////////////////////////////////////////////////////////////////
// 文件名：  ModifyDlg.cpp
// 创建时间：2007-11-11
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    整理对话框实现
///////////////////////////////////////////////////////////////////////////////

#include <stdPDL.h>
#include "ModifyDlg.h"

#include "resource.h"

CModifyDlg::CModifyDlg(CDiaHelper* pDia, IDiaSymbol* pSymbol,
                       DWORD_PTR dwTypeInfo) : LDialog(IDD_DLG_MODIFY)
{
    m_pDia       = pDia;
    m_pSymbol    = pSymbol;
    m_dwTypeInfo = dwTypeInfo;
    m_bInited    = FALSE;
}

BOOL CModifyDlg::cbAddMember(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    CModifyDlg* pThis = (CModifyDlg*)pParam;

    BSTR bsName = NULL;
    pCurSymbol->get_name(&bsName);

    int cnt = pThis->m_list.GetItemCount();
    int idx = pThis->m_list.InsertItem(cnt, bsName, 0, 0);
    ::SysFreeString(bsName);

    if (SymTagUDT == HIWORD(pThis->m_dwTypeInfo))
    {
        LString str;
        LONG    lOffset = 0;
        pCurSymbol->get_offset(&lOffset);
        str.Format(_T("+0x%x"), lOffset);
        pThis->m_list.SetItemText(idx, 1, str);

        ULONGLONG           ulSize = 0;
        LComPtr<IDiaSymbol> pType  = NULL;
        pCurSymbol->get_type(&pType);
        pType->get_length(&ulSize);
        str.Format(_T("0x%x"), static_cast<DWORD>(ulSize));
        pThis->m_list.SetItemText(idx, 2, str);
    }
    else
    {
        pThis->m_list.SetItemText(idx, 1, _T("N/A"));
        pThis->m_list.SetItemText(idx, 2, _T("N/A"));
    }

    pThis->m_list.SetCheckState(idx, TRUE);
    return FALSE;
}

void CModifyDlg::cbDumpString(LPCWSTR pszString, LPVOID pParam)
{
    CModifyDlg* pThis = (CModifyDlg*)pParam;
    pThis->m_view.AddText(pszString);
}

BOOL CModifyDlg::cbEnumModify(int index, LPVOID pParam)
{
    CModifyDlg *pThis = (CModifyDlg*)pParam;
    return pThis->m_list.GetCheckState(index);
}

void CModifyDlg::DumpModified(void)
{
    m_view.Clear();
    switch (HIWORD(m_dwTypeInfo))
    {
    case SymTagUDT:
        {
            m_pDia->DumpUDTHeader(m_pSymbol,
                static_cast<UdtKind>(LOWORD(m_dwTypeInfo)), cbDumpString,
                this);
        }
        break;
    case SymTagEnum:
        {
            m_pDia->DumpEnumHeader(m_pSymbol, cbDumpString, this);
        }
        break;
    default:
        {
            // TODO: unimplemented
        }
    }
    m_pDia->EnumMemberModify(m_pSymbol, cbEnumModify, cbDumpString, this);
    m_view.AddText(L"};");
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CModifyDlg::ProcessNotifyMessage(int idCtrl, LPNMHDR pnmh,
                                         BOOL& bHandled)
{
    if (m_bInited && LVN_ITEMCHANGED == pnmh->code && IDC_LIST == pnmh->idFrom)
    {
        DumpModified();
    }
    else
    {
        bHandled = FALSE;
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void CModifyDlg::ProcessCommandMessage(WORD wNotifyCode, WORD wID,
                                       HWND hWndCtrl, BOOL& bHandled)
{
    switch (wID)
    {
    case IDCANCEL:
    case IDOK:          OnClose(bHandled);      break;
    case IDC_BTN_ALL:   OnBtnAll();             break;
    case IDC_BTN_COPY:  OnBtnCopy();            break;
    default:
        bHandled = FALSE;
    }
}

void CModifyDlg::OnBtnAll(void)
{
    m_bInited = FALSE;

    BOOL bSel = TRUE;
    int  cnt  = m_list.GetItemCount();
    for (int i = 0; i < cnt; ++i)
    {
        if (!m_list.GetCheckState(i))
        {
            bSel = FALSE;
            break;
        }
    }

    bSel = !bSel;
    for (int i = 0; i < cnt; ++i)
        m_list.SetCheckState(i, bSel);

    m_bInited = TRUE;
    DumpModified();
}

void CModifyDlg::OnBtnCopy(void)
{
    m_view.CopyAll();
    MessageBox(_T("所有文本已复制到剪贴板。"), _T("PDB Explorer"),
        MB_ICONINFORMATION);
}

///////////////////////////////////////////////////////////////////////////////

void CModifyDlg::OnClose(BOOL& bHandled)
{
    EndDialog(IDOK);
}

BOOL CModifyDlg::OnInitDialog(HWND hCtrlFocus, LPARAM lParam, BOOL& bHandled)
{
    LStringW str;
    BSTR     bsName = NULL;
    m_pSymbol->get_name(&bsName);
    str.Format(L"整理 %s 的符号", bsName);
    ::SysFreeString(bsName);
    SetWindowTextW(str);

    // 初始化列表
    m_list = GetDlgItem(IDC_LIST);
    m_list.InsertColumn(0, _T("名称"), 260);
    m_list.InsertColumn(1, _T("偏移"), 70);
    m_list.InsertColumn(2, _T("大小"), 70);
    m_list.SetExtendedListViewStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES
        | LVS_EX_FULLROWSELECT);
    m_pDia->EnumTag(m_pSymbol, SymTagData, cbAddMember, this, NULL);

    LWnd wndRect = GetDlgItem(IDC_ST_RECT);
    RECT rcView;
    wndRect.GetParentRect(&rcView);
    wndRect.ShowWindow(SW_HIDE);

    m_view.Create(&rcView, m_hWnd, IDC_VIEW_DETAIL,
        reinterpret_cast<LPCWSTR>(lParam));
    m_view.EnableHyperLink(FALSE);
    m_view.SetEventHandler(this);

    m_bInited = TRUE;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////

void CModifyDlg::OnNavigateComplete(void)
{
    m_pDia->DumpSymbol(m_pSymbol, m_dwTypeInfo, cbDumpString, this);
    m_view.SetCurrentSymbol(m_pSymbol);
}
