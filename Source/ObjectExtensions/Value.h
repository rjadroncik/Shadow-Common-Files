#pragma once
#include "Object.h"

#include <SCFMathematicsTypes.h> 

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CValue : public CObject
	{
	public:
		//Parses a string & sets the parsed value as the value of the object
		virtual UINT Parse(_IN CString& rString) = 0;

	public:
		virtual ~CValue() {}

	public:
		virtual bool IsValue() _GET { return true; }

	public:
		//Creates a new value object based on the supplied class key & uses it to parse the given string
		//NOTE: Need to delete the returned object
		static CValue& Parse(_IN ENUM eClassKey, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);

	public:
		//Creates a new value object from parsing the given string, type of the returned value depends on the string contents
		//Return value can by of the following type: ClassInt, ClassFloat, ClassBool
		//NOTE: Need to delete the returned object
		static CValue& Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
	
	protected:
		CValue() {}
	};
};
