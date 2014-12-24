#pragma once
#include "WindowChild.h"
#include "ButtonPush.h"
#include "Edit.h"

namespace SCFUI
{
	class SCFUI_API CSpinner : public CWindowChild, public IEventListener
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassSpinner; }
		CString   ToString() _GET { return TEXT("Spinner"); }

	public:
		CSpinner(_INOUT CWindowAbstract* pParent);
		virtual ~CSpinner();

	public:
		CEdit* Buddy()                    _GET { return m_pBuddy; }
		void   Buddy(_INOUT CEdit* pEdit) _SET { m_pBuddy = pEdit; UpdateBuddy(); }

		float Value()                 _GET { return m_Value.fValue; }
		void  Value(_IN float fValue) _SET { m_Value.fValue = fValue; UpdateBuddy(); }

		float Step()                  _GET { return m_Value.fStep; }
		void  Step(_IN float fStep)   _SET { m_Value.fStep = fStep; }

		bool StepRelative()                   _GET { return m_Value.bRelative; }
		void StepRelative(_IN bool bRelative) _SET { m_Value.bRelative = bRelative; }

		UINT Style()                                         _GET;
		void Style(_IN UINT dwStyle, _IN bool clear = FALSE) _SET;

	protected:
		void OnWindowSize           (_IN CEventWindow& rEvent);
		bool OnWindowSizing         (_IN CEventWindow& rEvent);

		void OnWindowPaint          (_IN CEventWindow& rEvent);
		bool OnWindowPaintBackground(_IN CEventWindow& rEvent);

		void OnTimer(_IN UINT uiID);

		bool RegisterWin32Class();
		bool OnEvent(_IN CEvent& rEvent);

		void  UpdateBuddy();
		float StepValue();

	private:
		CButtonPush* m_pButtonPositive;
		CButtonPush* m_pButtonNegative;

		struct SValueState
		{
			float fValue;

			Float2 Range;
			UINT   uiDigits;

			float fStep;
			bool  bRelative;

		} m_Value;

		struct SDragState
		{
			float   fValueStart;

			SCFGraphics::Point2i MouseStartPos;
			bool    bDragging;

		} m_DragState;

		CEdit* m_pBuddy;
	};
};