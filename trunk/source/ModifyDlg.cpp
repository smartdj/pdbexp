///////////////////////////////////////////////////////////////////////////////
// �ļ�����  ModifyDlg.cpp
// ����ʱ�䣺2007-11-11
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    ����Ի���ʵ��
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include "ModifyDlg.h"
#include "SymWrap.h"

#include "resource.h"

typedef struct _tagModifyParam {
    CModifyDlg* This;    // �Ի���ָ��
    enum SymTagEnum tag; // �������ŵ�����
    int idx;             // ��ǰö�ٵĳ�Ա����
    int idxUseless;      // useless ������
    LONG offset;         // useless ��ƫ����
    LStringW str;        // ��Ա�ַ���
} MODIFYPARAM, *PMODIFYPARAM;

CModifyDlg::CModifyDlg(CDiaHelper* pDia, IDiaSymbol* pSymbol) : LDialog(IDD_DLG_MODIFY)
{
    m_pDia = pDia;
    m_pSymbol = pSymbol;
    m_bInited = FALSE;
}

BOOL CModifyDlg::cbAddMember(IDiaSymbol* pCurSymbol, PVOID pParam)
{
    CModifyDlg* pThis = (CModifyDlg*)pParam;

    LBStr bsName;
    pCurSymbol->get_name(&bsName);

    int cnt = pThis->m_list.GetItemCount();
    int idx = pThis->m_list.InsertItem(cnt, bsName, 0, 0);

    LString str;
    LONG lOffset = 0;
    pCurSymbol->get_offset(&lOffset);
    str.Format(_T("+0x%x"), lOffset);
    pThis->m_list.SetItemText(idx, 1, str);

    ULONGLONG ulSize = 0;
    SymPtr pType;
    pCurSymbol->get_type(&pType);
    pType->get_length(&ulSize);
    str.Format(_T("0x%x"), (DWORD)ulSize);
    pThis->m_list.SetItemText(idx, 2, str);

    pThis->m_list.SetCheckState(idx, TRUE);
    return FALSE;
}

BOOL CModifyDlg::cbEnumModify(IDiaSymbol* pCurSymbol, LPVOID pParam)
{
    PMODIFYPARAM p = (PMODIFYPARAM)pParam;
    LStringW str;

    if (SymTagEnum == p->tag)
    {
        if (p->This->m_list.GetCheckState(p->idx))
        {
            LVariant v;
            LBStr bsName;
            pCurSymbol->get_name(&bsName);
            pCurSymbol->get_value(&v);
            str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;%s = 0x%x;<br />\r\n",
                (PCWSTR)bsName, v.intVal);
            p->str += str;
        }
        ++p->idx;
        return FALSE;
    }

    if (p->This->m_list.GetCheckState(p->idx))
    {
        // �����ǰ���ܵ� useless ��
        LONG offset;
        pCurSymbol->get_offset(&offset);
        LONG size = offset - p->offset;
        if (-1 != p->offset)
        {
            str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;BYTE useless%d[%d]; "  \
                L"<font class=\"comment\">// +0x%x(%0x)</font><br />",
                p->idxUseless, size, p->offset, size);
            p->str += str;
            ++p->idxUseless;
            p->offset = -1;
        }
        CSym* sym = CSym::NewSym(pCurSymbol);
        sym->Format(&str);
        p->str += str;
        CSym::Delete(sym);
    }
    else
    {
        // ���� useless ��
        if (-1 == p->offset)
            pCurSymbol->get_offset(&p->offset);
    }

    ++p->idx;
    return FALSE;
}

void CModifyDlg::DumpModified(void)
{
    m_view.Clear();

    LStringW str;
    CSym* sym = CSym::NewSym(m_pSymbol);
    sym->GetHeader(&str);
    CSym::Delete(sym);
    m_view.AddText(str);

    MODIFYPARAM mp;
    mp.This = this;
    m_pSymbol->get_symTag((PDWORD)&mp.tag);
    mp.idx = 0;
    mp.idxUseless = 0;
    mp.offset = -1;
    CSym::Enum(m_pSymbol, SymTagData, cbEnumModify, &mp);
    m_view.AddText(mp.str);

    // ʣ��� useless ��Ŀ
    if (-1 != mp.offset)
    {
        ULONGLONG size;
        m_pSymbol->get_length(&size);
        size -= mp.offset;
        str.Format(L"&nbsp;&nbsp;&nbsp;&nbsp;BYTE useless%d[%d]; "  \
            L"<font class=\"comment\">// +0x%x(%0x)</font><br />",
            mp.idxUseless, (DWORD)size, mp.offset, (DWORD)size);
        m_view.AddText(str);
    }

    m_view.AddText(L"};");
}

///////////////////////////////////////////////////////////////////////////////

LRESULT CModifyDlg::OnNotify(
    int idCtrl,
    LPNMHDR pnmh,
    BOOL& bHandled)
{
    if (m_bInited && LVN_ITEMCHANGED == pnmh->code && IDC_LIST == pnmh->idFrom)
    {
        DumpModified();
        return 0;
    }
    else
    {
        bHandled = FALSE;
        return LDialog::OnNotify(idCtrl, pnmh, bHandled);
    }
}

///////////////////////////////////////////////////////////////////////////////

void CModifyDlg::OnCommand(
    WORD wNotifyCode,
    WORD wID,
    HWND hWndCtrl,
    BOOL& bHandled)
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
    MessageBox(_T("�����ı��Ѹ��Ƶ������塣"), _T("PDB Explorer"),
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
    str.Format(L"���� %s �ķ���", bsName);
    ::SysFreeString(bsName);
    SetWindowTextW(str);

    // ��ʼ���б�
    m_list = GetDlgItem(IDC_LIST);
    m_list.InsertColumn(0, _T("����"), 260);
    m_list.InsertColumn(1, _T("ƫ��"), 70);
    m_list.InsertColumn(2, _T("��С"), 70);
    m_list.SetExtendedListViewStyle(LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES
        | LVS_EX_FULLROWSELECT);
    CSym::Enum(m_pSymbol, SymTagData, cbAddMember, this);

    LWnd wndRect = GetDlgItem(IDC_ST_RECT);
    RECT rcView;
    wndRect.GetRectInParent(&rcView);
    wndRect.ShowWindow(SW_HIDE);

    m_view.Create(&rcView, m_hWnd, IDC_VIEW_DETAIL, (PVOID)lParam);
    m_view.EnableHyperLink(FALSE);
    m_view.SetEventHandler(this);

    m_bInited = TRUE;
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////

void CModifyDlg::OnNavigateComplete(void)
{
    DumpModified();
}
