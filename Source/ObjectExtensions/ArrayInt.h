#pragma once
#include "Array.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CArrayInt : public CArray
	{
	public:
		SCF::UINT Parse(_IN CString& rString);

	public:
		static CArrayInt& Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);

	public:
		CArrayInt(_IN CArrayInt& rArray);
		CArrayInt(_IN SCF::UINT uiSize);
		virtual ~CArrayInt();

	public:
		void AtPut(_IN SCF::UINT uiIndex, _IN CValue& rValue) _SET;

	public:
		int At         (_IN SCF::UINT uiIndex) _GET;
		int operator [](_IN SCF::UINT uiIndex) _GET;

	public:
		void AtPut(_IN SCF::UINT uiIndex, _IN int iValue) _SET;

	protected:
		CArrayInt() {}
	};
};
