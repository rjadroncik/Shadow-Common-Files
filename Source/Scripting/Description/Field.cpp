#include "Field.h"
#include "Method.h"

using namespace SCFBase;
using namespace SCFScripting;

CField::CField()
{
    m_pType = NULL;
    m_eVisibility = VisibilityPrivate;
    m_bIsStatic = false;
    m_bIsReadOnly = false;
}

CField::~CField()
{

}

