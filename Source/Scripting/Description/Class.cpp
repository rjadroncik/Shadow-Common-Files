#include "Class.h"

using namespace SCFScripting;

CClass::CClass() : m_pBaseClass(nullptr)
{
}

CClass::~CClass()
{
   if (m_pBaseClass) { delete m_pBaseClass; }

   m_Fields.AllDelete();
   m_Properties.AllDelete();
   m_Constructors.AllDelete();
   m_Interfaces.AllDelete();
   m_Methods.AllDelete();
}

void CClass::FieldAdd   (_IN _REF CField&    rField)    _SET { m_Fields.LastAdd(rField); }
void CClass::MethodAdd  (_IN _REF CMethod&   rMethod)   _SET { m_Methods.LastAdd(rMethod); }
void CClass::PropertyAdd(_IN _REF CProperty& rProperty) _SET { m_Properties.LastAdd(rProperty); }

void CClass::ConstructorAdd(_IN _REF CMethod&    rConstructor) _SET { m_Constructors.LastAdd(rConstructor); }
void CClass::InterfaceAdd  (_IN _REF CInterface& rInterface)   _SET { m_Interfaces.LastAdd  (rInterface); }

CMethod* CClass::Method(_IN CString& rName, _IN CList<CClass>& rArgumentTypes) _GET 
{
	return nullptr; 
}

CProperty* CClass::Property(_IN CString& rName) _GET 
{
	return nullptr; 
}