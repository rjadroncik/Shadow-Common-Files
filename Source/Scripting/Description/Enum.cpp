#include "Enum.h"

using namespace SCFBase;
using namespace SCFScripting;

SCFScripting::CEnum::CEnum()
{
}

SCFScripting::CEnum::~CEnum()
{
	m_Values.AllDelete();
	m_Names.AllDelete();
}

void SCFScripting::CEnum::Literal(_IN CString& rName, _IN ENUM eValue) _SET
{
	m_Values.AtPut(rName, *(new SCFBase::CEnum(eValue)));
	m_Names.AtPut(eValue, *(new CString(rName)));
}

SCFBase::CEnum* SCFScripting::CEnum::LiteralValue(_IN CString& rName) _GET
{
	return m_Values.At(rName);
}

CString* SCFScripting::CEnum::LiteralName(_IN ENUM eValue) _GET
{
	return m_Names.At(eValue);
}
