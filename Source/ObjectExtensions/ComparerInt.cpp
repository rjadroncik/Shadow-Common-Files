#include "ComparerInt.h"
#include "Int.h"

using namespace SCFBase;
using namespace SCF;

CComparerInt::CComparerInt()
{
}

CComparerInt::~CComparerInt()
{
}

bool CComparerInt::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CInt Int1st(0);
	static CInt Int2nd(0);

	if (r1st.ClassKey() == ClassInt) { Int1st = (CInt&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Int1st = CInt::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassInt) { Int2nd = (CInt&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Int2nd = CInt::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	return Int1st <= Int2nd;
}

bool CComparerInt::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CInt Int1st(0);
	static CInt Int2nd(0);

	if (r1st.ClassKey() == ClassInt) { Int1st = (CInt&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Int1st = CInt::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassInt) { Int2nd = (CInt&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Int2nd = CInt::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	return Int1st == Int2nd;
}