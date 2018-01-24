#include "Interface.h"

using namespace SCFScripting;

CInterface::CInterface()
{

}

CInterface::~CInterface()
{
   m_BaseInterfaces.AllDelete();
   m_Methods.AllDelete();
}

void CInterface::BaseInterfaceAdd(_IN _REF CInterface& rInterface) _SET
{
	m_BaseInterfaces.LastAdd(rInterface);
}

void CInterface::MethodAdd(_IN _REF CMethod& rMethod) _SET
{
	m_Methods.LastAdd(rMethod);
}

CMethodSignature* CInterface::Method(_IN CString& rName, _IN CList<CClass>& rArgumentTypes) _GET 
{
	return nullptr; 
}
