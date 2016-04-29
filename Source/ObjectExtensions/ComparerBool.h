#pragma once
#include "Comparer.h"

namespace SCFBase
{
	//Performs default identity comparisons
	class OBJECT_EXTENSIONS_API CComparerBool : public CComparer
	{
	public:
		CString ToString() _GET { return STRING("{ComparerBool}"); }

	public:
		CComparerBool();
		virtual ~CComparerBool();

	public:
		bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET;
		bool Equal      (_IN CObject& r1st, _IN CObject& r2nd) _GET;
	};
};
