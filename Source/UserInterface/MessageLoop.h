#pragma once

#include "Classes.h"

namespace SCFUI
{
	class SCFUI_API CMessageLoop
	{
	public:
		CMessageLoop();
		virtual ~CMessageLoop();

		int Enter();
		void Quit();

	public:
	};
};
