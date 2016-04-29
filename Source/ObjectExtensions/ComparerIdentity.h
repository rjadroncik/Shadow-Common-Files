#pragma once
#include "Comparer.h"

namespace SCFBase
{
	//Performs default identity comparisons
	class OBJECT_EXTENSIONS_API CComparerIdentity : public CComparer
	{
	public:
		CString ToString() _GET { return STRING("{ComparerIdentity}"); }

	public:
		CComparerIdentity();
		virtual ~CComparerIdentity();

	public:
		bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET;
		bool Equal      (_IN CObject& r1st, _IN CObject& r2nd) _GET;
	};
};
