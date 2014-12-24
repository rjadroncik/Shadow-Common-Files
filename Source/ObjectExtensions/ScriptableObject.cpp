#include "StdAfx.h"
#include "ScriptableObject.h"

CScriptableObject::CScriptableObject()
{
}

CScriptableObject::~CScriptableObject()
{
}

void CScriptableObject::PropertySet(_IN SCF::ENUM eName, _IN CObject& rValue) _SET 
{
	SCF_UNREFERENCED_PARAMETER(eName);
	SCF_UNREFERENCED_PARAMETER(rValue);
}

CObject* CScriptableObject::PropertyGet(_IN SCF::ENUM eName) _GET
{
	SCF_UNREFERENCED_PARAMETER(eName);
	return NULL; 
}

CObject* CScriptableObject::MethodExecute(_IN SCF::ENUM eName)
{
	SCF_UNREFERENCED_PARAMETER(eName);
	return NULL; 
}

CObject* CScriptableObject::MethodExecute(_IN SCF::ENUM eName, _IN CObject& rArgument)
{
	SCF_UNREFERENCED_PARAMETER(eName);
	SCF_UNREFERENCED_PARAMETER(rArgument);
	return NULL; 
}

CObject* CScriptableObject::MethodExecute(_IN SCF::ENUM eName, _IN CVector& rArguments)
{
	SCF_UNREFERENCED_PARAMETER(eName);
	SCF_UNREFERENCED_PARAMETER(rArguments);
	return NULL; 
}