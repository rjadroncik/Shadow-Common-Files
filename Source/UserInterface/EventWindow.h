#pragma once
#include "Event.h"

namespace SCFUI
{
	class SCFUI_API CEventWindow : public CEvent
	{
	public:
		CEventWindow(_IN ULONG uiID, _INOUT CWindowAbstract* pSource);
		virtual ~CEventWindow();

	public:
		Gdiplus::Graphics* Canvas() _GET { return m_pCanvas; }

		SCFGraphics::Rect4i RectWindow() _GET { return m_RectWindow; }
		SCFGraphics::Rect4i RectClient() _GET { return m_RectClient; }
		SCFGraphics::Rect4i RectUpdate() _GET { return m_RectUpdate; }
		
	public:
		void Canvas(_INOUT Gdiplus::Graphics* pCanvas) _SET { m_pCanvas = pCanvas; }

		void RectWindow(_IN SCFGraphics::Rect4i& rRect) _SET { m_RectWindow = rRect; } 
		void RectClient(_IN SCFGraphics::Rect4i& rRect) _SET { m_RectClient = rRect; }
		void RectUpdate(_IN SCFGraphics::Rect4i& rRect) _SET { m_RectUpdate = rRect; }

	private:
		Gdiplus::Graphics* m_pCanvas;

		SCFGraphics::Rect4i m_RectWindow;
		SCFGraphics::Rect4i m_RectClient;
		SCFGraphics::Rect4i m_RectUpdate;
	};
};