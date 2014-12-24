#include "Property.h"
#include "Method.h"

using namespace SCFBase;
using namespace SCFScripting;

CProperty::CProperty()
{
	m_pType = NULL;
	m_dwProperties = PropertyIsSettable | CallThis;
}

CProperty::~CProperty()
{

}

