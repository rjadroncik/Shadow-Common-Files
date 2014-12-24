#pragma once
#include "ButtonMultiState.h"

namespace SCFUI
{
	#define BTN_STYLE_CLICK_CIRCLE_SIZE	13

	class SCFUI_API CButtonCheck : public CButtonMultiState
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassButtonCheck; }
		CString   ToString() _GET { return TEXT("ButtonCheck"); }

	public:
		CButtonCheck(_INOUT CWindowAbstract* pParent);
		virtual ~CButtonCheck();

		//Compute automatic size
		void AutoRect(_OUT SCFGraphics::Rect4i& rOutRect);

	protected:
		void OnWindowPaint(_IN CEventWindow& rEvent);
	};
};
