#include "TokenNumber.h"

using namespace SCFCompiler;

CString CTokenNumber::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	if (ValueClassKey() == ClassInt)   { for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'i'); } }
	if (ValueClassKey() == ClassFloat) { for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'f'); } }

	return RetVal;
}

CTokenNumber::CTokenNumber(_INOUT _REF CStringRange& rText) : CToken(rText)
{
	m_pValue = NULL;
}

CTokenNumber::~CTokenNumber()
{
	delete m_pValue;
}

void CTokenNumber::ValueClassKey(SCF::ENUM eClassKey) _SET
{
	//Must not be called 2x
	_ASSERTE(m_pValue == NULL);

	if (eClassKey == ClassInt)   { m_pValue = new CInt(0); }
	if (eClassKey == ClassFloat) { m_pValue = new CFloat(0); }
}


bool CTokenNumber::Update()
{
	if (m_pValue) { m_pValue->Parse(*m_pText); }
	else          { m_pValue = new CInt(*m_pText); }

	return TRUE;
}
