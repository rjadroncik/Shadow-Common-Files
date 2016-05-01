#include "ComparerBoundValue.h"

using namespace SCFBase;
using namespace SCFXMLPrivate;

CXMLComparerBoundValue::CXMLComparerBoundValue()
{
}

CXMLComparerBoundValue::~CXMLComparerBoundValue()
{
}

bool CXMLComparerBoundValue::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	return ((CString&)r1st) <= ((CString&)r2nd);
}

bool CXMLComparerBoundValue::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	return ((CString&)r1st) == ((CString&)r2nd);
}