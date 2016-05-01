#pragma once
#include "Array.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CArrayFloat : public CArray
	{
	public:
		UINT Parse(_IN CString& rString);

	public:
		static CArrayFloat& Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
	
	public:
		CArrayFloat(_IN CArrayFloat& rArray);
		CArrayFloat(_IN UINT uiSize);
		virtual ~CArrayFloat();

	public:
		void AtPut(_IN UINT uiIndex, _IN CValue& rValue) _SET;

	public:
		float At         (_IN UINT uiIndex) _GET;
		float operator [](_IN UINT uiIndex) _GET;

	public:
		void AtPut(_IN UINT uiIndex, _IN float fValue) _SET;

	protected:
		CArrayFloat() {}
	};
};
