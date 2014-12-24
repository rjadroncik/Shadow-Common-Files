#include "Comparer.h"

using namespace SCFBase;
using namespace SCF;

CComparer::CComparer()
{
}

CComparer::~CComparer()
{
}

bool CComparer::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	return (LessOrEqual(r1st, r2nd) && LessOrEqual(r2nd, r1st));
}
