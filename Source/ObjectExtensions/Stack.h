#pragma once

#include "StackRaw.h"

namespace SCFBase
{
	template<class TValue>
	class CStack : public CStackRaw
	{
	public:
		CStack() {}
		virtual ~CStack() {}

	public:
		void Push(_IN _REF TValue& rObject) { CStackRaw::Push(rObject); }

	public:
		TValue* Top()    _GET { return (TValue*)CStackRaw::Top(); }
		TValue* Bottom() _GET { return (TValue*)CStackRaw::Bottom(); }
	
		TValue* At(_IN UINT uiIndex) _GET { return (TValue*)CStackRaw::At(uiIndex); }
	};
};
