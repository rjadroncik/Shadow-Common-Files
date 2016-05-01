#include "ComparerInt.h"
#include "Int.h"

using namespace SCFBase;

CComparerInt::CComparerInt()
{
}

CComparerInt::~CComparerInt()
{
}

bool CComparerInt::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	CInt& rInt1st = (CInt&)r1st;
	CInt& rInt2nd = (CInt&)r2nd;

	return rInt1st <= rInt2nd;
}

bool CComparerInt::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	CInt& rInt1st = (CInt&)r1st;
	CInt& rInt2nd = (CInt&)r2nd;

	return rInt1st == rInt2nd;
}