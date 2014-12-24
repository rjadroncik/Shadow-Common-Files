#pragma once

#include "Event.h"

namespace SCFUI
{
	class SCFUI_API CEventMouse : public CEvent
	{
		friend CWindowAbstract;

	public:
		SCFGraphics::Point2i Position()   _GET { return m_Position; }
		int          WheelDelta() _GET { return m_iDelta; }

		bool ButtonLeft()   _GET { return (m_dwFlags & MK_LBUTTON) != 0; }
		bool ButtonMiddle() _GET { return (m_dwFlags & MK_MBUTTON) != 0; }
		bool ButtonRight()  _GET { return (m_dwFlags & MK_RBUTTON) != 0; }

		bool ButtonX1()   _GET { return (m_dwFlags & MK_XBUTTON1) != 0; }
		bool ButtonX2()   _GET { return (m_dwFlags & MK_XBUTTON2) != 0; }

		bool KeyShift()   _GET { return (m_dwFlags & MK_SHIFT)   != 0; }
		bool KeyControl() _GET { return (m_dwFlags & MK_CONTROL) != 0; }

	protected:

		CEventMouse(_IN ULONG uiID, _INOUT CWindowAbstract* pSource);
		virtual ~CEventMouse();

		void Position  (_IN SCFGraphics::Point2i& rPosition) _SET { m_Position = (SCFGraphics::Point2i&)rPosition; }
		void Flags     (_IN DWORD dwFlags)      _SET { m_dwFlags  = dwFlags; }
 		void WheelDelta(_IN int iDelta)         _SET { m_iDelta   = iDelta; }

	private: 
		SCFGraphics::Point2i m_Position;
 		DWORD   m_dwFlags;

		int     m_iDelta;
	};
};