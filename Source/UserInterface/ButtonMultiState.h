#pragma once
#include "Button.h"

namespace SCFUI
{
	class SCFUI_API CButtonMultiState : public CButton
	{
	public:
 		bool Active()					{ return m_bActive; }
		void Active(_IN bool bActive)	{ m_bActive = bActive; }

		void ActionActivate();
		void ActionDeactivate();

	protected:
		CButtonMultiState();
		virtual ~CButtonMultiState();

		void OnMouseLeftDown(_IN CEventMouse& rEvent);
		void OnMouseLeftUp  (_IN CEventMouse& rEvent);
		void OnMouseMove    (_IN CEventMouse& rEvent);

	protected:
		bool m_bActive;
		bool m_bActiveLast;
	};
};