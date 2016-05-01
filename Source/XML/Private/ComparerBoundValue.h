#pragma once
#include <SCFObjectExtensions.h>
#include "Classes.h"

namespace SCFXMLPrivate
{
	class CXMLComparerBoundValue : public CComparer
	{
	public:
		ENUM ClassKey() _GET { return ClassXMLComparerBoundValue; }
		CString   ToString() _GET { return STRING("{XMLComparerBoundValue}"); }

	public:
		CXMLComparerBoundValue();
		virtual ~CXMLComparerBoundValue();

	public:
		bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET;
		bool Equal      (_IN CObject& r1st, _IN CObject& r2nd) _GET;
	};
};
