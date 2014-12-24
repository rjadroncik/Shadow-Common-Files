#pragma once
#include "WindowAbstract.h"

namespace SCFUI
{
	class SCFUI_API CComboBox : public CWindowAbstract
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassComboBox; }
		CString   ToString() _GET { return TEXT("ComboBox"); }

	public:
		CComboBox();
		virtual ~CComboBox();
	};
};