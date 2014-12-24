#include "ComparerBool.h"
#include "Bool.h"

using namespace SCFBase;
using namespace SCF;

CComparerBool::CComparerBool()
{
}

CComparerBool::~CComparerBool()
{
}

bool CComparerBool::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CBool Bool1st(0);
	static CBool Bool2nd(0);

	if (r1st.ClassKey() == ClassBool) { Bool1st = (CBool&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Bool1st = CBool::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassBool) { Bool2nd = (CBool&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Bool2nd = CBool::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (!Bool1st.Value() || Bool2nd.Value()) { return TRUE; }

	return FALSE;
}

bool CComparerBool::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	static CBool Bool1st(0);
	static CBool Bool2nd(0);

	if (r1st.ClassKey() == ClassBool) { Bool1st = (CBool&)r1st; }
	else
	{
		if (IS_STRING(r1st)) 
		{
			UINT uiCharsParsed = 0;
			Bool1st = CBool::Parse((CString&)r1st, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	if (r2nd.ClassKey() == ClassBool) { Bool2nd = (CBool&)r2nd; }
	else
	{
		if (IS_STRING(r2nd)) 
		{
			UINT uiCharsParsed = 0;
			Bool2nd = CBool::Parse((CString&)r2nd, &uiCharsParsed);

			if (!uiCharsParsed) { return FALSE;}
		}
		else { return FALSE; }
	}

	return Bool1st == Bool2nd;
}