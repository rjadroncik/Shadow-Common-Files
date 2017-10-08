#include "Variable.h"

using namespace SCFBase;
using namespace SCFScripting;

CVariable::CVariable()
{
   m_pType = NULL;
   m_eVisibility = VisibilityPrivate;
   m_bIsReadOnly = false;
   m_bIsStatic = false;
}

CVariable::~CVariable()
{
}
