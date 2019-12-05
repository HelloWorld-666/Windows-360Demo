#include "stdafx.h"
#include "360.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
     CPaintManagerUI::SetInstance(hInstance);
     CDuiString s = CPaintManagerUI::GetInstancePath();
     CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\360skin"));
     //CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr))
    {
        return 0;
    }
    C360SafeFrameWnd* pFrame = new C360SafeFrameWnd();
    if (NULL == pFrame)
    {
        return 0;
    }
    pFrame->Create(NULL, _T("360"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pFrame->CenterWindow();
    pFrame->ShowWindow(*pFrame, SW_SHOW);

    CPaintManagerUI::MessageLoop();
    ::CoUninitialize();

    return 0;
}
