#include "Globals.h"
#include "Registry.h"

using namespace SCFBase;
using namespace SCFScripting;

void InitializeGlobals()
{
	CRegistry::ClassRegister(ClassBool,   CLASS_REGISTER_ARGS_CREATE(SCFBase::CBool, SCFBase::CObject));

	//CRegistry::ClassRegister(STRING("Int"),     ClassInt,    Int);
	//CRegistry::ClassRegister(STRING("Float"),   ClassFloat,  Float);
	//CRegistry::ClassRegister(STRING("Float2"),  ClassFloat2, Float2);
	//CRegistry::ClassRegister(STRING("Float3"),  ClassFloat3, Float3);
	//CRegistry::ClassRegister(STRING("Float4"),  ClassFloat4, Float4);

	//CRegistry::ClassRegister(STRING("String"),  ClassString, String);
	//CRegistry::ClassRegister(STRING("Vector"),  ClassVector, Vector);
}


