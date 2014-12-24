#include "MethodSignature.h"
#include "Class.h"

using namespace SCFBase;
using namespace SCFScripting;

CMethodSignature::CMethodSignature()
{
	m_pReturnType = NULL;
	m_dwProperties = CallThis;
}

CMethodSignature::~CMethodSignature()
{
	m_Arguments.AllDelete();
}

void CMethodSignature::ArgumentAdd(_IN _REF CArgument& rArgument) _SET
{
	m_Arguments.LastAdd(rArgument);
}