#pragma once
#include "ButtonMultiState.h"

namespace SCFUI
{
	class CSpinner;

	class SCFUI_API CButtonPush : public CButtonMultiState
	{
		friend CSpinner;

	public:
		SCF::ENUM ClassKey() _GET { return ClassButtonPush; }
		CString   ToString() _GET { return TEXT("ButtonPush"); }

	public:
		CButtonPush(_INOUT CWindowAbstract* pParent);
		virtual ~CButtonPush();

 		SCFGraphics::ColorRGBA ColorRaised();
		SCFGraphics::ColorRGBA ColorPushed();

		void ColorRaised(_IN SCFGraphics::ColorRGBA color);
		void ColorPushed(_IN SCFGraphics::ColorRGBA color);

		//Compute automatic size
		void AutoRect(_OUT SCFGraphics::Rect4i& rOutRect);

	protected:
 		void OnWindowPaint(_IN CEventWindow& rEvent);

	protected:
 		struct SColorInfo
		{
			Color*	pRaised;
			Color*	pPushed;

		} m_Colors;
	};
};