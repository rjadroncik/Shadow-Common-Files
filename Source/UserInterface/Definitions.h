#pragma once

#include "Classes.h"

#ifndef WINVER				
#define WINVER 0x0501		
#endif

#ifndef _WIN32_WINNT		                
#define _WIN32_WINNT 0x0501	
#endif						

#ifndef _WIN32_WINDOWS		
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE			
#define _WIN32_IE 0x0600
#endif

#include <SCFMathematics.h>
#include <SCFGraphics.h>
#include <SCFObjectExtensions.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Ole2.h>
#include <Windowsx.h>
#include <Gdiplus.h>

namespace SCFUI
{
	enum Styles
	{
		StyleHotTrack        = 0x00000001,
		StyleAllowDeactivate = 0x00000002,
		StyleAutosize        = 0x00000004,
		StyleFocusFrame      = 0x00000008,
		StyleBorder          = 0x00000010,
		StyleTransparent     = 0x00000020,
	};

	enum Events
	{
		EventWindowCreate			= WM_CREATE,
		EventWindowDestroy			= WM_DESTROY,
		EventWindowMove				= WM_MOVE,
		EventWindowMoving			= WM_MOVING,
		EventWindowSize				= WM_SIZE,
		EventWindowSizing			= WM_SIZING,
		EventWindowPaint			= WM_PAINT,
		EventWindowPaintBackground	= WM_ERASEBKGND,
		EventMouseLeftDown			= WM_LBUTTONDOWN,
		EventMouseLeftUp			= WM_LBUTTONUP,
		EventMouseLeftDoubleClick	= WM_LBUTTONDBLCLK,
		EventMouseMiddleDown		= WM_MBUTTONDOWN,
		EventMouseMiddleUp			= WM_MBUTTONUP,
		EventMouseMiddleDoubleClick	= WM_MBUTTONDBLCLK,
		EventMouseRightDown			= WM_RBUTTONDOWN,
		EventMouseRightUp			= WM_RBUTTONUP,
		EventMouseRightDoubleClick	= WM_RBUTTONDBLCLK,
		EventMouseMove				= WM_MOUSEMOVE,
		EventMouseWheel				= WM_MOUSEWHEEL,
		EventMouseCaptuereLost		= WM_CAPTURECHANGED,
		EventKeyChar				= WM_CHAR,
		EventKeyDown				= WM_KEYDOWN,
		EventKeyUp					= WM_KEYUP,
		EventKeySystemDown			= WM_SYSKEYDOWN,
		EventKeySystemUp			= WM_SYSKEYUP,
		EventKeyFocusReceived		= WM_SETFOCUS,
		EventKeyFocusLost			= WM_KILLFOCUS,
	};

	enum Actions
	{
		//Button actions
		ActionClicked = 1,
		ActionActivated,
		ActionDeactivated,

		//Window actions
		ActionClosed,
	};

	class CAction;
	class CEvent;

	//Return values indicate whether action/event should be further processed: TRUE - continue, FALSE - stop
	typedef bool (__stdcall *ActionHandler) (_IN CAction& rAction); 
	typedef bool (__stdcall *EventListener) (_IN CEvent&  rEvent); 

	class IEventListener
	{
	public:
		virtual bool OnEvent(_IN CEvent&  rEvent) = 0;
	};

	class IActionHandler
	{
	public:
		virtual bool OnAction(_IN CAction& rAction) = 0;
	};
};