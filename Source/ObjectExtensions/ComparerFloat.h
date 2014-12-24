#pragma once
#include "Comparer.h"

namespace SCFBase
{
	//Performs default identity comparisons
	class OBJECT_EXTENSIONS_API CComparerFloat : public CComparer
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassComparerFloat; }
		CString   ToString() _GET { return STRING("{ComparerFloat}"); }

	public:
		CComparerFloat();
		virtual ~CComparerFloat();

	public:
		bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET;
		bool Equal      (_IN CObject& r1st, _IN CObject& r2nd) _GET;
	};
};
