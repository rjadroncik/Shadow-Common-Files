#pragma once
#include "Array.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CArrayInt : public CArray
	{
	public:
		UINT Parse(_IN CString& rString);

	public:
		ValueKind Kind() _GET { return ValueArrayInt; }

	public:
		static CArrayInt& Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);

	public:
		CArrayInt(_IN CArrayInt& rArray);
		CArrayInt(_IN UINT uiSize);
		virtual ~CArrayInt();

	public:
		void AtPut(_IN UINT uiIndex, _IN CValue& rValue) _SET;

	public:
		int At         (_IN UINT uiIndex) _GET;
		int operator [](_IN UINT uiIndex) _GET;

	public:
		void AtPut(_IN UINT uiIndex, _IN int iValue) _SET;

	protected:
		CArrayInt() {}
	};
};
