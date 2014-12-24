#pragma once

#include "WindowAbstract.h"

namespace SCFUI
{
	class SCFUI_API CWindow : public CWindowAbstract
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassWindow; }
		CString   ToString() _GET { return TEXT("Window"); }

		CWindow();
		virtual ~CWindow();
		
		//Container
		virtual HRESULT WindowAdd   (_IN CWindowAbstract* pWindow);
		virtual HRESULT WindowRemove(_IN CWindowAbstract* pWindow);

		//Manual action performing
		void ActionClose();

	protected:
		virtual void OnWindowPaint(_IN CEventWindow& rEvent);

		virtual LRESULT	DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam);
		virtual bool    RegisterWin32Class();
	};
};
