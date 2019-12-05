// 360.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "360.h"
#include "ControlEx.h"              //��Ӧ126��
#include "atlstr.h"
//#include "c:\Program Files (x86)\Windows Kits\8.1\Include\um\gdiplusgraphics.h"

////ȫ��
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
    //��ʼ��GDI+��
    //m_pGdiplusStartupInput = new Gdiplus::GdiplusStartupInput;
    //Gdiplus::GdiplusStartup(&m_gdiplusToken, m_pGdiplusStartupInput, NULL); // ����GDI�ӿ�

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
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);                                                                    //�ú���������ȡָ�����ڵ�����
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);                                      //�ú��������ı�ָ�����ڵ�����

    m_pm.Init(m_hWnd);
    CDialogBuilder builder;
    CDialogBuilderCallbackEx cb;
    CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pm);                                                //ָ��xml�ļ���
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

LRESULT C360SafeFrameWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //����Ҫ�ı�ǿͻ�����ָ�����ǻ״̬ʱ�����������
{
    if (::IsIconic(*this)) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT C360SafeFrameWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //����Ҫ����ǿͻ����Ĵ�С��λ��ʱ�����������
{
    return 0;
}
    
LRESULT C360SafeFrameWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                        //���ǿͻ�����Ҫ�ػ�ʱ�����������
{
    return 0;
}

LRESULT C360SafeFrameWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                      //���CWnd�а����˹�꣬������SetCapture�������������ʱ��ÿ������ƶ�ʱ��Windows�����������
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);                                                                                   //�����������ʾ���Ͼ��ε���Ļ����ת��Ϊ�ͻ�����

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);                                                                              //���CWnd�ͻ�����Ĵ�С

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

LRESULT C360SafeFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                          //��CWnd�Ĵ�С���ı��Ժ�����������
{
    SIZE szRoundCorner = m_pm.GetRoundCorner();
    if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {                                  //ȷ��CWnd�Ƿ���С����ͼ�껯��
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

LRESULT C360SafeFrameWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                  //ÿ��Windows��Ҫ֪����󻯵�λ�úʹ�С��������С�������ٳߴ�ʱ���͵����������
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

LRESULT C360SafeFrameWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)                     //���û��ӿ��Ʋ˵���ѡ��������ߵ��û�ѡ������󻯻���С����ťʱ�������������
{
    // ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
    if (wParam == SC_CLOSE) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);                                                                               //ȷ��CWnd�Ƿ����
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
#define WM_DATA_MESSAGE			(WM_USER + 102)  //���ݸ�����Ϣ

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
//    else if (msg.sType == _T("selectchanged"))                          //ѡ���ǩҳ ��������> �л���ͬҳ��
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
//    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);            //�õ����ڷ��
//    styleValue &= ~WS_CAPTION;                                      //~WS_CAPTIONȥ��ϵͳ�Դ�������
//    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
//
//    m_pm.Init(m_hWnd);                                              //���������봰�ھ������
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
////    //ȥ��ϵͳ�Դ�������
////    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);        //����������ܹ����ָ�����ڵ���Ϣ
////	styleValue &= ~WS_CAPTION;          //WS_CAPTION ��Window Stytle�ĺ꣬��ʾ������   �ڶԻ���༭���ж�һ������ȥ�������С����������Ĺ�ѡ��
////	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
////
////	m_pm.Init(m_hWnd);//���������봰�ھ������
////	CDialogBuilder builder;
////
////	CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_pm);//����XML����̬��������Ԫ�أ��벼�ֽ���Ԫ�أ����ĺ�����������		xml ���ص�ʱ������ WM_CREATE ����Ϣ��Ӧ�����д����
////	//ע�⣺CDialogBuilder ������һ���Ի�����
////	ASSERT(pRoot && "Failed to parse XML");
////	if (NULL == pRoot)//����Ҳ���Ƥ���ļ����˳�
////	{
////		MessageBox(NULL, TEXT("Cant not find the skin!"), NULL, MB_ICONHAND);
////		return 0;
////	}
////	m_pm.AttachDialog(pRoot);//���ӿؼ����ݵ�HASH���С���ΪpRoot��Ϊ�Ի����㣬Ϊ�䴴���ؼ���	
////	m_pm.AddNotifier(this); //����֪ͨ������ӿؼ�����Ϣ��Ӧ��������Ϣ�ͻᴫ�ﵽduilib����Ϣѭ�������ǿ�����Notify����������Ϣ����
////
////	/**************************************************************/
////	Init();	//��ʼ�����水��
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
////�ı�������ȵĸ߶�
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
//    // ��ʱ�����յ�WM_NCDESTROY���յ�wParamΪSC_CLOSE��WM_SYSCOMMAND
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

