#include "Package.h"

using namespace SCFScripting;

CPackage::CPackage()
{
}

CPackage::~CPackage()
{
   m_Classes.AllDelete();
}

bool CPackage::ClassAdd(_IN _REF CClass& rClass) _SET
{
 	if (m_Classes.ContainsName(rClass.Name())) { return false; }
 
	m_Classes.AtPut(rClass.Name(), rClass);
	return true;
}

CClass* CPackage::ClassNamed(_IN CString& rName) _GET 
{ 
	return m_Classes.At(rName);
}

