#pragma once
#include "WindowChild.h"

namespace SCFUI
{
	class SCFUI_API CEdit : public CWindowChild
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassEdit; }
		CString   ToString() _GET { return TEXT("Edit"); }

	public:
		CEdit(CWindowAbstract* pParent);
		virtual ~CEdit();
		
	public:
		Int2& Selection();
		void  Selection(_IN Int2& rRange);

	protected:
		bool RegisterWin32Class();

		//Drawing
		void OnWindowPaint          (_IN CEventWindow& rEvent);
		bool OnWindowPaintBackground(_IN CEventWindow& rEvent);
		
		LRESULT	DoDefaultProcessing(_IN UINT uiMessage, _IN WPARAM wParam, _IN LPARAM lParam);

		static LRESULT CALLBACK	WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);

	protected:
		Int2 m_Selection;
	};
};