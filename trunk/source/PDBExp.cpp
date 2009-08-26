///////////////////////////////////////////////////////////////////////////////
// �ļ�����  PDBExp.cpp
// ����ʱ�䣺2007-10-28
// ���ߣ�    ����
// ��Ȩ���У�Titi Studio (?) 2001-2007
//-----------------------------------------------------------------------------
// ˵����    �������
///////////////////////////////////////////////////////////////////////////////

#include <pdl_base.h>
#include <pdl_module.h>
#include <pdl_commctrl.h>
#include <pdl_com.h>
#include "MainFrame.h"
#include "Version.h"

#include "resource.h"

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine, int nShowCmd)
{
    LComCtlInit comctl(ICC_WIN95_CLASSES);
    LComInit    com;
    LOleInit    ole;

    LAppModule* theApp = LAppModule::Initialize(hInstance);

    WNDCLASS wc = { 0 };
    wc.hbrBackground = PDL_SYSBRUSH(COLOR_BTNFACE);
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = theApp->LoadIcon(MAKEINTRESOURCE(IDI_MAIN_ICON));
    wc.hInstance = theApp->GetInstance();
    wc.lpszClassName = _T("PDBExp");
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);

    CMainFrame frameWnd(&wc);
    if (!frameWnd.Create(NULL, PDBEXP_WNDCAPTION,
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, NULL))
    {
        return -1;
    }

    frameWnd.CenterWindow(NULL, WNDPOS_HCENTER | WNDPOS_VCENTER);
    frameWnd.ShowWindow(nShowCmd);

    MSG msg;
    HACCEL hAccTable = LoadAccelerators(hInstance,
        MAKEINTRESOURCE(IDR_ACCELERATOR));
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (0 == TranslateAccelerator(frameWnd.GetSafeHWND(),
            hAccTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    LAppModule::Destroy();
    return (int)msg.wParam;
}
