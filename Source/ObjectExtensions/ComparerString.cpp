#include "ComparerString.h"
#include "String.h"

using namespace SCFBase;
using namespace SCF;

CComparerString::CComparerString()
{
}

CComparerString::~CComparerString()
{
}

bool CComparerString::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	if (IS_STRING(r1st))
	{
		if (IS_STRING(r2nd)) 
		{
			return ((CString&)r1st) <= ((CString&)r2nd);
		}
		else 
		{
			return ((CString&)r1st) <= r2nd.ToString();
		}
	}

	if (IS_STRING(r2nd))
	{
		return r1st.ToString() <= ((CString&)r2nd);
	}

	return r1st.ToString() <= r2nd.ToString();
}

bool CComparerString::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	if (IS_STRING(r1st))
	{
		if (IS_STRING(r2nd)) 
		{
			return ((CString&)r1st) == ((CString&)r2nd);
		}
		else 
		{
			return ((CString&)r1st) == r2nd.ToString();
		}
	}

	if (IS_STRING(r2nd))
	{
		return r1st.ToString() == ((CString&)r2nd);
	}

	return r1st.ToString() == r2nd.ToString();
}