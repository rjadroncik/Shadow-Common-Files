#include "ComparerIdentity.h"

using namespace SCFBase;

CComparerIdentity::CComparerIdentity()
{
}

CComparerIdentity::~CComparerIdentity()
{
}

bool CComparerIdentity::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	return (&r1st <= &r2nd);
}

bool CComparerIdentity::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	return (&r1st == &r2nd);
}