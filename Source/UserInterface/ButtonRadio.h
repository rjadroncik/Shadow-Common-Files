#pragma once
#include "ButtonMultiState.h"

namespace SCFUI
{
	#define BTN_STYLE_CLICK_CIRCLE_SIZE	13

	class SCFUI_API CButtonRadio : public CButtonMultiState
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassButtonRadio; }
		CString   ToString() _GET { return STRING("{ButtonRadio}"); }

	public:
		CButtonRadio(_INOUT CWindowAbstract* pParent);
		virtual ~CButtonRadio();

	public:
		//Compute automatic size
		void AutoRect(_OUT SCFGraphics::Rect4i& rOutRect);

	protected:
		void OnWindowPaint(_IN CEventWindow& rEvent);
	};
};
