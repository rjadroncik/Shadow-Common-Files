#include "Package.h"

using namespace SCFScripting;

CPackage::CPackage(_IN CString& rName)
{
	m_Name = rName;
}

CPackage::~CPackage()
{
}


CClass* CPackage::ClassNamed(_IN CString& rName) _GET 
{ 
	return (CClass*)m_Classes.At(rName);
}

