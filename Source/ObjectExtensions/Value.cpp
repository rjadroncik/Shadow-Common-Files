#include "Value.h"
#include "String.h"
#include "Int.h"
#include "Float.h"

using namespace SCFBase;
using namespace SCF;

//Accepts 3 basic number formats
//0x[0-9,a-f]* : HEX integer
//[0-9]* : DEC integer
//[0-9]+[.]{1}[0-9]+ : DEC float
CValue& CValue::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	UINT uiIntParsedChars   = 0;
	UINT uiFloatParsedChars = 0;

	int   iIntParsed   =   CInt::Parse(rString, &uiIntParsedChars);
	float fFloatParsed = CFloat::Parse(rString, &uiFloatParsedChars);

	if (uiIntParsedChars >= uiFloatParsedChars) 
	{ 
		if (uipOutCharsParsed) { *uipOutCharsParsed = uiIntParsedChars; } 

		return *(new CInt(iIntParsed));
	}
	else                                        
	{
		if (uipOutCharsParsed) { *uipOutCharsParsed = uiFloatParsedChars; } 

		return *(new CFloat(fFloatParsed)); 
	}
}
	
