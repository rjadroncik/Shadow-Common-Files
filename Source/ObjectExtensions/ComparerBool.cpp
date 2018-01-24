#include "ComparerBool.h"
#include "Bool.h"

using namespace SCFBase;

CComparerBool::CComparerBool()
{
}

CComparerBool::~CComparerBool()
{
}

bool CComparerBool::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	CBool& rBool1st = (CBool&)r1st;
	CBool& rBool2nd = (CBool&)r2nd;

	if (!rBool1st.Value() || rBool2nd.Value()) { return true; }

	return false;
}

bool CComparerBool::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	CBool& rBool1st = (CBool&)r1st;
	CBool& rBool2nd = (CBool&)r2nd;

	return rBool1st == rBool2nd;
}