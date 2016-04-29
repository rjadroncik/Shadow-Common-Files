#pragma once
#include "Object.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CComparer : public CObject
	{
	public:
		//These are implemented for simplicity of defining "inline" comparers
		CString ToString() _GET { return STRING("{Comparer}"); }

	public:
		virtual ~CComparer();

	public:
		//Returns true if first is <= second, false otherwise
		virtual bool LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET = 0;

	public:
		//Additional equality comparison, by default calls [LessOrEqual(..)] twice :)
		virtual bool Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET;

	protected:
		CComparer();
	};
};
