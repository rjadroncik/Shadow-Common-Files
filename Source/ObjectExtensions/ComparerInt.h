#pragma once
#include "Comparer.h"

namespace SCFBase
{
	//Performs default identity comparisons
	class OBJECT_EXTENSIONS_API CComparerInt : public CComparer
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassComparerInt; }
		CString   ToString() _GET { return STRING("{ComparerInt}"); }

	public:
		CComparerInt();
		virtual ~CComparerInt();

	public:
		bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET;
		bool Equal      (_IN CObject& r1st, _IN CObject& r2nd) _GET;
	};
};
