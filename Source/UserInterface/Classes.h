#pragma once

//Reserved class keys: 0x201 - 0x0300

namespace SCFUI
{
	enum Classes
	{
		ClassWindow = 0x201,
		ClassButtonClick,
		ClassButtonPush,
		ClassButtonRadio,
		ClassButtonCheck,
		ClassEdit,
		ClassSpinner,
		ClassComboBox,
	};

	#ifdef SCFUSERINTERFACE_EXPORTS
	#define SCFUI_API __declspec(dllexport)
	#else
	#define SCFUI_API __declspec(dllimport)
	#endif
};