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
	CFloat& rFloat1st = (CFloat&)r1st;
	CFloat& rFloat2nd = (CFloat&)r2nd;

	return rFloat1st <= rFloat2nd;
}

bool CComparerFloat::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	CFloat& rFloat1st = (CFloat&)r1st;
	CFloat& rFloat2nd = (CFloat&)r2nd;

	return rFloat1st == rFloat2nd;
}