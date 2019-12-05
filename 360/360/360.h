#ifndef _360_H_
#define _360_H_

#include "resource.h"
#include "UIlib.h"
#include "Control/UIGifAnim.h"

using namespace DuiLib;

class C360SafeFrameWnd : public CWindowWnd, public INotifyUI, public CControlUI, /*public WindowImplBase,*/ public CGifAnimUI
{
public:
    C360SafeFrameWnd();
    ~C360SafeFrameWnd();
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;             //˫����Ϣ
    void OnFinalMessage(HWND /*hWnd*/);     //�رմ��ڲ������Լ�
    void Init();
    void OnPrepare();
    virtual void Notify(TNotifyUI& msg);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        
    virtual void PaintBkImage(HDC hDC);         //����CGifAnimUI���е�CControlUI
public:
    CPaintManagerUI m_pm;
    
private:
    CButtonUI*  m_pCloseBtn;
    CButtonUI*  m_pMaxBtn;
    CButtonUI*  m_pMinBtn;
    CButtonUI*  m_pRestoreBtn;
    CGifAnimUI* m_pgifLogoPaying;               //gif

    RECT m_rcItem1;
    Gdiplus::Image	*m_pGifImage1;
    UINT	    m_nFramePosition1;			// ��ǰ�ŵ��ڼ�֡pmy
    CControlUI  c;
    //ȫ��
    //Gdiplus::GdiplusStartupInput	*m_pGdiplusStartupInput;
    //ULONG_PTR						m_gdiplusToken;
};
#endif
