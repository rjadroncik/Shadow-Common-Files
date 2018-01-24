#include "Property.h"
#include "Method.h"

using namespace SCFBase;
using namespace SCFScripting;

CProperty::CProperty()
{
	m_pType = nullptr;
	m_eVisibility = VisibilityPrivate;
	m_bIsReadOnly = false;
	m_bIsStatic = false;
}

CProperty::~CProperty()
{

}

