#pragma once

#include "TreeSimpleBase.h"

namespace SCFBase
{
	template<class TValue>
	class CTreeSimple : public CTreeSimpleBase<TValue, CTreeSimple<TValue>>
	{
	public:
		//Destructor DOES NOT delete the value, it just releases it! (can override this behavior)
		CTreeSimple() {}
		CTreeSimple(_IN _REF TValue& rValue) : CTreeSimpleBase(rValue) {}
		virtual ~CTreeSimple() {}
	};
};