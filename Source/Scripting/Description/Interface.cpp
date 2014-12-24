#include "Interface.h"

using namespace SCFScripting;

CInterface::CInterface()
{

}

CInterface::~CInterface()
{
}

CMethodSignature* CInterface::Method(_IN CString& rName, _IN CList<CClass>& rArgumentTypes) _GET { return NULL; }
