#pragma once

#include "Message.h"

namespace SCFUI
{
	class SCFUI_API CEvent : public CMessage
	{
	public:
		virtual ~CEvent();

	public:
		LPTSTR IDText() _GET;

	protected:
		CEvent();
	};
};