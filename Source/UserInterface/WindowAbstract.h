#pragma once

#include "Definitions.h"

using namespace Gdiplus;
using namespace SCFMathematics;
using namespace SCFBase;

#include "Action.h"
#include "Event.h"
#include "EventWindow.h"
#include "EventMouse.h"
#include "EventKey.h"

namespace SCFUI
{
	class CEdit;

	class SCFUI_API CWindowAbstract : public CObject
	{
		friend CEdit;

	public:
		//Sizing
		SCFGraphics::Rect4i RectWindow()                               _GET;
		void                RectWindow(_IN SCFGraphics::Rect4i& rRect) _SET;

// 		SCFGraphics::Rect4i RectClient()                               _GET;
// 		void                RectClient(_IN SCFGraphics::Rect4i& rRect) _SET;

	public:
		//State
		bool Enabled()                       _GET { return IsWindowEnabled(m_hWnd) != 0; }
		void Enabled(_IN bool bEnabled )     _SET { EnableWindow(m_hWnd, bEnabled); }

		bool Visible()                       _GET { return IsWindowVisible(m_hWnd) != 0; }
		void Visible(_IN bool bVisible)      _SET { if (bVisible) { ShowWindow(m_hWnd, SW_SHOW); } else { ShowWindow(m_hWnd, SW_HIDE); } }

		bool Focused()                       _GET { return (GetFocus() == m_hWnd); }
		void Focused(_IN bool bFocused)      _SET { if (bFocused) { SetFocus(m_hWnd); } else { if (GetFocus() == m_hWnd) { SetFocus(NULL); } } }

 		bool MouseCapture()                  _GET { return (GetCapture() == m_hWnd); }
		void MouseCapture(_IN bool bCapture) _SET { if (bCapture) { SetCapture(m_hWnd); } else { if (GetCapture() == m_hWnd) { ReleaseCapture(); } } }

	public:
		//Basic properties
		LPTSTR Label()                   _GET { return m_szLabel; }
		void   Label(_IN LPTSTR pString) _SET; 

		void* Tag()            _GET { return m_vpTag; }
		void  Tag(void* vpTag) _SET { m_vpTag = vpTag; }
	 
	public:
		//Misc
		void Redraw(_IN bool bErase);

		void EventListenerAdd(_IN EventListener pFuction);
		void ActionHandlerAdd(_IN ActionHandler pFuction);

		void EventListenerAdd(_INOUT IEventListener* pInterface);
		void ActionHandlerAdd(_INOUT IActionHandler* pInterface);
		
		HWND WindowHandle() _GET { return m_hWnd; }

		void TimerStart(_IN UINT uiID, _IN UINT uiMilliseconds);
		void TimerStop (_IN UINT uiID);

	protected:
		CWindowAbstract();
		virtual ~CWindowAbstract();

	protected:
		//Drawing
		virtual int  OnWindowCreate         (_IN CEventWindow& rEvent);
		virtual void OnWindowDestroy        (_IN CEventWindow& rEvent);
		virtual void OnWindowMove           (_IN CEventWindow& rEvent);
		virtual bool OnWindowMoving         (_IN CEventWindow& rEvent);
 		virtual void OnWindowSize           (_IN CEventWindow& rEvent);
		virtual bool OnWindowSizing         (_IN CEventWindow& rEvent);
		virtual void OnWindowPaint          (_IN CEventWindow& rEvent) = 0;
		virtual bool OnWindowPaintBackground(_IN CEventWindow& rEvent);

	protected:
		//Mouse tracking
		virtual void OnMouseLeftDown         (_IN CEventMouse& rEvent);
		virtual void OnMouseLeftUp           (_IN CEventMouse& rEvent);
		virtual void OnMouseLeftDoubleClick  (_IN CEventMouse& rEvent);
		virtual void OnMouseMiddleDown       (_IN CEventMouse& rEvent);
		virtual void OnMouseMiddleUp         (_IN CEventMouse& rEvent);
		virtual void OnMouseMiddleDoubleClick(_IN CEventMouse& rEvent);
		virtual void OnMouseRightDown        (_IN CEventMouse& rEvent);
		virtual void OnMouseRightUp          (_IN CEventMouse& rEvent);
		virtual void OnMouseRightDoubleClick (_IN CEventMouse& rEvent);
		virtual void OnMouseMove             (_IN CEventMouse& rEvent);
		virtual void OnMouseWheel            (_IN CEventMouse& rEvent);
		virtual void OnMouseCaptuereLost     (_IN CEventMouse& rEvent);
		
	protected:
		//Keyboard tracking
		virtual void OnKeyChar         (_IN CEventKey& rEvent);
		virtual void OnKeyDown         (_IN CEventKey& rEvent);
		virtual void OnKeyUp           (_IN CEventKey& rEvent);
		virtual void OnKeySystemDown   (_IN CEventKey& rEvent);
		virtual void OnKeySystemUp     (_IN CEventKey& rEvent);
		virtual void OnKeyFocusReceived(_IN CEventKey& rEvent);
		virtual void OnKeyFocusLost    (_IN CEventKey& rEvent);

		virtual void OnTimer(_IN UINT uiID);

	protected:
		//Dispatches events/actions - this means that functions registered as handlers are called for each action/event 
		bool ActionPerform(_IN CAction& rAction);
		bool EventAnnounce(_IN CEvent&  rEvent);

	protected:
		//Utility function for setting styles, styles are currently stored in one 32-bit variable to save up some memory :D
  		void Style(_IN DWORD dwStyle, _IN bool bValue) _SET { m_dwStyle |= dwStyle; if (!bValue) { m_dwStyle ^= dwStyle; } }
		bool Style(_IN DWORD dwStyle)                  _GET { return (m_dwStyle & dwStyle) != 0; }

	protected:
		//Utility function used to construct windows, this limits the dependency on the OS to this class, all other classes use this method or re-implement it
		virtual bool WindowCreate(_IN LPTSTR szClass, _IN DWORD dwStyle, _IN DWORD dwStyleEx, _IN CWindowAbstract* pParent);

		virtual bool RegisterWin32Class() = 0;
		virtual LRESULT DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam);

	protected:
		//////////////////////////////// Static functions /////////////////////////////
 		static LRESULT CALLBACK	WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
		
	private:
		//////////////////////////////// Member variables /////////////////////////////
		SCFGraphics::Rect4i m_Rect;
		LPTSTR m_szLabel;

		void* m_vpTag;
		UINT  m_dwStyle;

	private:
		struct SEventBinding
		{
			bool bClass;
 			union
			{
				EventListener   pFunction;
				IEventListener* pInterface;
			};

		} *m_pEventListeners;
			
	private:
		struct SActionBinding
		{
			bool bClass;
			union
			{
				ActionHandler   pFunction;			
				IActionHandler* pInterface;			
			};

		} *m_pActionHandlers;

	private:
		UINT m_uiEventListenerCount;
		UINT m_uiActionHandlerCount;

		HWND m_hWnd;
	};
};
