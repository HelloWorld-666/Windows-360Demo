// 360.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "360.h"
#include "ControlEx.h"              //对应126行
#include "atlstr.h"
//#include "c:\Program Files (x86)\Windows Kits\8.1\Include\um\gdiplusgraphics.h"

////全局
//Gdiplus::GdiplusStartupInput *gdiplusStartupInput;
//ULONG_PTR gdiplusToken;

C360SafeFrameWnd::C360SafeFrameWnd()
{

}

C360SafeFrameWnd::~C360SafeFrameWnd()
{

}

LPCTSTR C360SafeFrameWnd::GetWindowClassName() const { return _T("UIMainFrame"); };
UINT C360SafeFrameWnd::GetClassStyle() const { return CS_DBLCLKS; };
void C360SafeFrameWnd::OnFinalMessage(HWND /*hWnd*/) { delete this; };

void C360SafeFrameWnd::Init() 
{
    //初始化GDI+：
    //m_pGdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
    //Gdiplus::GdiplusStartup(&m_gdiplusToken, m_pGdiplusStartupInput, NULL); // 加载GDI接口

    m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
    m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
    m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
    m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));

    m_pgifLogoPaying = static_cast<CGifAnimUI*>(m_pm.FindControl(_T("abc")));       //gif
    m_pgifLogoPaying->SetVisible(true);
}

void C360SafeFrameWnd::OnPrepare() {
}

//void C360SafeFrameWnd::Notify(TNotifyUI& msg)
//{
//    CString strType = msg.sType;
//    if (msg.pSender == this)
//    {
//        if (strType == _T("timer"))
//        {
//            Invalidate();
//        }
//    }
//}

void C360SafeFrameWnd::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("windowinit"))
    { 
        OnPrepare();
    } 
    else if (msg.sType == _T("click")) 
    {
        if (msg.pSender == m_pCloseBtn) 
        {
            PostQuitMessage(0);
            return;
        }
        else if (msg.pSender == m_pMinBtn) 
        {
            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); return;
        }
        else if (msg.pSender == m_pMaxBtn) 
        {
            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return;
        }
        else if (msg.pSender == m_pRestoreBtn) 
        {
            SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return;
        }
    }
    else if (msg.sType == _T("selectchanged"))
    {
        CDuiString name = msg.pSender->GetName();
        CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
        if (name == _T("examine"))
        {
            pControl->SelectItem(0);
        }
        else if (name == _T("trojan"))
        {
            pControl->SelectItem(1);
        }
        else if (name == _T("plugins"))
        {
            pControl->SelectItem(2);
        }
        else if (name == _T("vulnerability"))
        {
            pControl->SelectItem(3);
        }
        else if (name == _T("rubbish"))
        {
            pControl->SelectItem(4);
        }
        else if (name == _T("cleanup"))
        {
            pControl->SelectItem(5);
        }
        else if (name == _T("fix"))
        {
            pControl->SelectItem(6);
        }
        else if (name == _T("tool"))
        {
            pControl->SelectItem(7);
        }
    }
    else if (msg.sType == _T("timer"))                  //pmy
    {
        if (msg.pSender == m_pgifLogoPaying)
        {
             c.Invalidate();
             m_pgifLogoPaying->SetVisible(true);
        }
    }
}
/*************************************/
//https://blog.csdn.net/reloner12/article/details/41847179
//void C360SafeFrameWnd::PaintBkImage(HDC hDC)
//{
//    Graphics graphics(pageGuid);             //gdi
//    graphics.DrawImage(pageGuid, m_rcPaint.left, m_rcPaint.top, m_rcPaint.right - m_rcPaint.left, m_rcPaint.bottom - m_rcPaint.top);
//
//    GUID pageGuid = Gdiplus::FrameDimensionTime;
//    hDC->m_pGifImage->SelectActiveFrame(&pageGuid->Guid, pageGuid->fcount++);
//    if (pageGuid->fcount == pageGuid->frameCount)
//        pageGuid->fcount = 0;
//}

void C360SafeFrameWnd::PaintBkImage(HDC hDC)
{
    if (NULL == hDC || NULL == m_pGifImage1) return;
    GUID pageGuid = Gdiplus::FrameDimensionTime;
    Gdiplus::Graphics graphics(hDC);
    graphics.DrawImage(m_pGifImage1, m_rcItem1.left, m_rcItem1.top, m_rcItem1.right - m_rcItem1.left, m_rcItem1.bottom - m_rcItem1.top);
    m_pGifImage1->SelectActiveFrame(&pageGuid, m_nFramePosition1);
}
/*************************************/


LRESULT C360SafeFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);                                                                    //该函数用来获取指定窗口的属性
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);                                      //该函数用来改变指定窗口的属性

    m_pm.Init(m_hWnd);
    CDialogBuilder builder;
    CDialogBuilderCallbackEx cb;
    CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pm);                                                //指定xml文件名
    ASSERT(pRoot && "Failed to parse XML");
    m_pm.AttachDialog(pRoot);
    m_pm.AddNotifier(this);

    Init();
    return 0;
}

LRESULT C360SafeFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT C360SafeFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    ::PostQuitMessage(0L);

    bHandled = FALSE;
    return 0;
}

LRESULT C360SafeFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //当需要改变非客户区以指明活动或非活动状态时调用这个函数
{
    if (::IsIconic(*this)) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT C360SafeFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //当需要计算非客户区的大小和位置时调用这个函数
{
    return 0;
}
    
LRESULT C360SafeFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                        //当非客户区需要重画时调用这个函数
{
    return 0;
}

LRESULT C360SafeFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                      //如果CWnd中包含了光标，或者用SetCapture捕获了鼠标输入时，每当鼠标移动时，Windows调用这个函数
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);                                                                                   //将给定点或显示器上矩形的屏幕坐标转换为客户坐标

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);                                                                              //获得CWnd客户区域的大小

    // 		if( !::IsZoomed(*this) ) {
    // 			RECT rcSizeBox = m_pm.GetSizeBox();
    // 			if( pt.y < rcClient.top + rcSizeBox.top ) {
    // 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
    // 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
    // 				return HTTOP;
    // 			}
    // 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
    // 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
    // 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
    // 				return HTBOTTOM;
    // 			}
    // 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
    // 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    // 		}

    RECT rcCaption = m_pm.GetCaptionRect();
    if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
        CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
        if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
            _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
            _tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
            return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT C360SafeFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                          //当CWnd的大小被改变以后调用这个函数
{
    SIZE szRoundCorner = m_pm.GetRoundCorner();
    if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {                                  //确定CWnd是否被最小化（图标化）
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++; rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
    }

    bHandled = FALSE;
    return 0;
}

LRESULT C360SafeFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                  //每当Windows需要知道最大化的位置和大小，或者最小或最大跟踪尺寸时，就调用这个函数
{
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);

    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
    lpMMI->ptMaxPosition.x = rcWork.left;
    lpMMI->ptMaxPosition.y = rcWork.top;
    lpMMI->ptMaxSize.x = rcWork.right;
    lpMMI->ptMaxSize.y = rcWork.bottom;

    bHandled = FALSE;
    return 0;
}

LRESULT C360SafeFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //当用户从控制菜单中选择命令，或者当用户选择了最大化或最小化按钮时，调用这个函数
{
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    if (wParam == SC_CLOSE) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);                                                                               //确定CWnd是否被最大化
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if (::IsZoomed(*this) != bZoomed) {
        if (!bZoomed) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if (pControl) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if (pControl) pControl->SetVisible(true);
        }
        else {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
            if (pControl) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
            if (pControl) pControl->SetVisible(false);
        }
    }
    return lRes;
}
#define WM_DATA_MESSAGE			(WM_USER + 102)  //数据更新消息

LRESULT C360SafeFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch (uMsg) {
    case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
    case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
    case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
    case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
    case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
    case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
    case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
    case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
    case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
    case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
    case WM_TIMER:
        if (wParam == 122)
        {

        }
        else
        {
            bHandled = FALSE;
        }
        break;
    default:
        bHandled = FALSE;
    }
    if (bHandled) return lRes;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}







//LPCTSTR C360SafeFrameWnd::GetWindowClassName() const 
//{ 
//    return _T("UIMainFrame"); 
//};
//
//UINT C360SafeFrameWnd::GetClassStyle() const
//{
//    return CS_DBLCLKS;
//}
//
//void C360SafeFrameWnd::OnFinalMessage(HWND /*hWnd*/)
//{
//    delete this;
//}
//
//void C360SafeFrameWnd::Init()
//{
//    m_pCloseBtn   = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
//    m_pMaxBtn     = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
//    m_pMinBtn     = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
//    m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
//}
//
//void C360SafeFrameWnd::OnPrepare()
//{
//
//}
//
//void C360SafeFrameWnd::Notify(TNotifyUI& msg)
//{
//    if (msg.sType == _T("windowinit"))
//    {
//        OnPrepare();
//    }
//    else if (msg.sType == _T("click"))
//    {
//        if (msg.pSender == m_pCloseBtn)
//        {
//            PostMessage(0);
//        }
//        else if (msg.pSender == m_pMinBtn)
//        {
//            SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
//            return;
//        }
//        else if (msg.pSender == m_pMaxBtn)
//        {
//            SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
//        }
//        else if (msg.pSender == m_pRestoreBtn)
//        {
//            SendMessage(WM_COMMAND, SC_RESTORE, 0);
//        }
//    }
//    else if (msg.sType == _T("selectchanged"))                          //选择标签页 ————> 切换不同页面
//    {
//        CDuiString name = msg.pSender->GetName();
//        CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
//        if (name == _T("examine"))
//        {
//            pControl->SelectItem(0);
//        }
//        else if (name == _T("trojan"))
//        {
//            pControl->SelectItem(1);
//        }
//        else if (name == _T("plugins"))
//        {
//            pControl->SelectItem(2);
//        }
//        else if (name == _T("vvlnerability"))
//        {
//            pControl->SelectItem(3);
//        }
//        else if (name == _T("rubbish"))
//        {
//            pControl->SelectItem(4);
//        }
//        else if (name == _T("cleanup"))
//        {
//            pControl->SelectItem(5);
//        }
//        else if (name == _T("fix"))
//        {
//            pControl->SelectItem(6);
//        }
//        else if (name == _T("tool"))
//        {
//            pControl->SelectItem(7);
//        }
//    }
//}
//
//LRESULT C360SafeFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);            //得到窗口风格
//    styleValue &= ~WS_CAPTION;                                      //~WS_CAPTION去掉系统自带标题栏
//    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
//
//    m_pm.Init(m_hWnd);                                              //主窗口类与窗口句柄关联
//    CDialogBuilder builder;
//    CDialogBuilderCallbackEx cb;
//    CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pm);
//    ASSERT(pRoot && "Failed to parse XML");
//    m_pm.AttachDialog(pRoot);
//    m_pm.AddNotifier(this);
//
//    Init();
//
//    return 0;
//}
//
////LRESULT C360SafeFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
////{
////    //去掉系统自带标题栏
////    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);        //用这个函数能够获得指定窗口的信息
////	styleValue &= ~WS_CAPTION;          //WS_CAPTION 是Window Stytle的宏，表示标题栏   在对话框编辑器中对一个窗体去掉属性中“标题栏”的勾选。
////	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
////
////	m_pm.Init(m_hWnd);//主窗口类与窗口句柄关联
////	CDialogBuilder builder;
////
////	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_pm);//加载XML并动态创建界面元素，与布局界面元素，核心函数单独分析		xml 加载的时机是在 WM_CREATE 的消息相应函数中处理的
////	//注意：CDialogBuilder 并不是一个对话框类
////	ASSERT(pRoot && "Failed to parse XML");
////	if (NULL == pRoot)//如果找不到皮肤文件则退出
////	{
////		MessageBox(NULL, TEXT("Cant not find the skin!"), NULL, MB_ICONHAND);
////		return 0;
////	}
////	m_pm.AttachDialog(pRoot);//附加控件数据到HASH表中……为pRoot作为对话框结点，为其创建控件树	
////	m_pm.AddNotifier(this); //增加通知处理，添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
////
////	/**************************************************************/
////	Init();	//初始化界面按键
////	/**************************************************************/
////
////	return 0;
////}
//
//LRESULT C360SafeFrameWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    bHandled = FALSE;
//    return 0;
//}
//
//LRESULT C360SafeFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    ::PostQuitMessage(0L);
//
//    bHandled = FALSE;
//    return 0;
//}
//
////???
//LRESULT C360SafeFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    if (::IsIconic(*this))
//    {
//        bHandled = FALSE;
//    }
//    return (wParam == 0) ? TRUE : FALSE;
//}
//
////改变标题栏等的高度
//LRESULT C360SafeFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    return 0;
//}
//
//LRESULT C360SafeFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    return 0;
//}
//
//LRESULT C360SafeFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
//    ::ScreenToClient(*this, &pt);
//
//    RECT rcClient;
//    ::GetClientRect(*this, &rcClient);
//
//    // 		if( !::IsZoomed(*this) ) {
//    // 			RECT rcSizeBox = m_pm.GetSizeBox();
//    // 			if( pt.y < rcClient.top + rcSizeBox.top ) {
//    // 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
//    // 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
//    // 				return HTTOP;
//    // 			}
//    // 			else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
//    // 				if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
//    // 				if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
//    // 				return HTBOTTOM;
//    // 			}
//    // 			if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
//    // 			if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
//    // 		}
//
//    RECT rcCaption = m_pm.GetCaptionRect();
//    if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
//        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
//        CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
//        if (pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 &&
//            _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
//            _tcscmp(pControl->GetClass(), _T("TextUI")) != 0)
//            return HTCAPTION;
//    }
//
//    return HTCLIENT;
//}
//
//LRESULT C360SafeFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    SIZE szRoundCorner = m_pm.GetRoundCorner();
//    if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
//        CDuiRect rcWnd;
//        ::GetWindowRect(*this, &rcWnd);
//        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
//        rcWnd.right++; rcWnd.bottom++;
//        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
//        ::SetWindowRgn(*this, hRgn, TRUE);
//        ::DeleteObject(hRgn);
//    }
//
//    bHandled = FALSE;
//    return 0;
//}
//
//LRESULT C360SafeFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    MONITORINFO oMonitor = {};
//    oMonitor.cbSize = sizeof(oMonitor);
//    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
//    CDuiRect rcWork = oMonitor.rcWork;
//    rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
//
//    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
//    lpMMI->ptMaxPosition.x = rcWork.left;
//    lpMMI->ptMaxPosition.y = rcWork.top;
//    lpMMI->ptMaxSize.x = rcWork.right;
//    lpMMI->ptMaxSize.y = rcWork.bottom;
//
//    bHandled = FALSE;
//    return 0;
//}
//
//LRESULT C360SafeFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
//    if (wParam == SC_CLOSE) {
//        ::PostQuitMessage(0L);
//        bHandled = TRUE;
//        return 0;
//    }
//    BOOL bZoomed = ::IsZoomed(*this);
//    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//    if (::IsZoomed(*this) != bZoomed) {
//        if (!bZoomed) {
//            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
//            if (pControl) pControl->SetVisible(false);
//            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
//            if (pControl) pControl->SetVisible(true);
//        }
//        else {
//            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
//            if (pControl) pControl->SetVisible(true);
//            pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
//            if (pControl) pControl->SetVisible(false);
//        }
//    }
//    return lRes;
//}
//
//LRESULT C360SafeFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//    LRESULT lRes = 0;
//    BOOL bHandled = TRUE;
//    switch (uMsg) {
//    case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
//    case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
//    case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
//    case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
//    case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
//    case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
//    case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
//    case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
//    case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
//    case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
//    default:
//        bHandled = FALSE;
//    }
//    if (bHandled) return lRes;
//    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
//    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
//}

