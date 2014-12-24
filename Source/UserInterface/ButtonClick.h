#pragma once
#include "Button.h"

namespace SCFUI
{
	class SCFUI_API CButtonClick : public CButton
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassButtonClick; }
		CString   ToString() _GET { return TEXT("ButtonClick"); }

	public:
		CButtonClick(_INOUT CWindowAbstract* pParent);
		virtual ~CButtonClick();

		//Manual action performing
 		void ActionClick();

		//Compute automatic size
		void AutoRect(_OUT SCFGraphics::Rect4i& rOutRect);

	protected:
		void OnWindowPaint  (_IN CEventWindow& rEvent);
		void OnMouseLeftDown(_IN CEventMouse& rEvent);
		void OnMouseLeftUp  (_IN CEventMouse& rEvent);
		void OnMouseMove    (_IN CEventMouse& rEvent);

	protected: 
		bool m_bActive;
	};
};
