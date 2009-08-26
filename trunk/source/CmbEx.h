///////////////////////////////////////////////////////////////////////////////
// 文件名：  CmbEx.h
// 创建时间：2007-10-28
// 作者：    李马
// 版权所有：Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// 说明：    PDB Explorer ComboBox类
///////////////////////////////////////////////////////////////////////////////

#ifndef PDBEXP_CMBEX
#define PDBEXP_CMBEX

#pragma once

#include <PDLWindow.h>
#include <PDLCtrl.h>

class CLbEx : public LListBox, protected LSubclassWnd
{
public:
    CLbEx& operator=(__in HWND hWnd);
protected:
    void OnLButtonDblClk(UINT uFlags, int x, int y, BOOL& bHandled);
};

class CCmbEx : public LComboBox, protected LSubclassWnd
{
public:
    BOOL Create(__in LPCTSTR lpWindowName, __in DWORD dwStyle,
        __in LPCRECT lpRect, __in HWND hWndParent, __in UINT nID,
        __in LPVOID lpParam);
public:
    int FindString(__in LPCTSTR lpszString);
protected:
    void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, BOOL& bHandled);
    void ProcessCommandMessage(WORD wNotifyCode, WORD wID, HWND hWndCtrl,
        BOOL& bHandled);
private:
    CLbEx m_list;
    LEdit m_edit;
};

#endif // PDBEXP_CMBEX
