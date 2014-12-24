#include "ComparerFloat.h"
#include "Float.h"

using namespace SCFBase;
using namespace SCF;

CComparerFloat::CComparerFloat()
{
}

CComparerFloat::~CComparerFloat()
{
}

bool CComparerFloat::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CFloat Float1st(0);
	static CFloat Float2nd(0);

	if (r1st.ClassKey() == ClassFloat) { Float1st = (CFloat&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Float1st = CFloat::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassFloat) { Float2nd = (CFloat&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Float2nd = CFloat::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	return Float1st <= Float2nd;
}

bool CComparerFloat::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CFloat Float1st(0);
	static CFloat Float2nd(0);

	if (r1st.ClassKey() == ClassFloat) { Float1st = (CFloat&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Float1st = CFloat::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassFloat) { Float2nd = (CFloat&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Float2nd = CFloat::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	return Float1st == Float2nd;
}