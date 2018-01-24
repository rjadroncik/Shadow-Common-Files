#include "Attribute.h"
#include "Writer.h"

using namespace SCFXML;

CXMLAttribute::CXMLAttribute()
{
}

CXMLAttribute::~CXMLAttribute()
{
}

CString CXMLAttribute::ToString() _GET
{
	CString              Result;
	CStreamStringWrite   ResultStreamWrite(Result);
	CStreamWriteTextUCS2 ResultStreamWriteText(ResultStreamWrite, false);

	CXMLWriter(ResultStreamWriteText).WriteAttribute(*this);

	return Result;
}

bool CXMLAttribute::operator ==(_IN CXMLAttribute& rAttribute)
{
	if ((Value() && !rAttribute.Value()) || (!Value() && rAttribute.Value())) { return false; }

	return (*Value()) == (*rAttribute.Value());
}