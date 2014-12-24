#pragma once
#include "WindowAbstract.h"

namespace SCFUI
{
	class SCFUI_API CWindowChild : public CWindowAbstract
	{
	public:
		bool Hot()              _GET { return m_bHot; }
		void Hot(_IN bool bHot) _SET { m_bHot = bHot; }

		CWindowAbstract* Parent()                                _GET { return m_pParent; }
		void             Parent(_INOUT CWindowAbstract* pParent) _SET { m_pParent = pParent; }

	public:
		//Style
		bool AutoSize()                 _GET;
		void AutoSize(_IN bool bEnable) _SET;

		bool HotTrack()                 _GET;
		void HotTrack(_IN bool bEnable) _SET;

		bool AllowDeactivate()                _GET;
		void AllowDeactivate(_IN bool bAllow) _SET;

		bool FocusFrame()                 _GET;
		void FocusFrame(_IN bool bEnable) _SET;

		bool Border()                 _GET;
		void Border(_IN bool bEnable) _SET;

		bool Transparent()                      _GET;
		void Transparent(_IN bool bTransparent) _SET;

	protected:
		CWindowChild();
		virtual ~CWindowChild();

		virtual void OnMouseLeftDown(_IN CEventMouse& rEvent);
		virtual void OnMouseLeftUp  (_IN CEventMouse& rEvent);
		virtual void OnMouseMove    (_IN CEventMouse& rEvent);

		virtual bool WindowCreate(_IN LPTSTR szClass, _IN DWORD dwStyle, _IN DWORD dwStyleEx, _INOUT CWindowAbstract* pParent);

	protected:
 		bool OnWindowPaintBackground(_IN CEventWindow& rEvent);
	
	protected:
		LRESULT DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam);

	protected:
		bool m_bHot;
		CWindowAbstract* m_pParent;
	};
};