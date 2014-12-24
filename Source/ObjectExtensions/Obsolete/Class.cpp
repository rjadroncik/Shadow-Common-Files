#include "Class.h"

#include "StringRange.h"
#include "StringSearch.h"
#include "DictionaryInt64.h"
#include "DictionaryString.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

typedef void* (__stdcall  *OBJECT_ALLOCATOR) (unsigned int);

CDictionaryInt64  Class_ClassesByKey;
CDictionaryString Class_ClassesByName;

OBJECT_ALLOCATOR Class_ObjectAllocator = NULL;

CClass::CClass(_IN SCF::ENUM eKey, _IN CString& rQName, _IN SCF::UINT uiObjectSize, _IN void* hModule, _IN _REF CClass* pParent)
{
	static char s_szFunctionName[256];

	m_eKey = eKey;

	m_uiObjectSize = uiObjectSize;
	m_pParent = pParent;

	CStringSearch Search(rQName);
	int iNameSpacePos = Search.FindString("::");
	if (iNameSpacePos == -1)
	{
		m_QName = CStringRange(rQName, 1);
		m_uiPrefix = 0;

		//Public constructor: ??0 <ClassName> @@QAE@XZ
		CString FunctionName = STRING("??0C") + Name() + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_pConstructorDefault)
		{
			//Protected constructor: ??0 <ClassName> @@IAE@XZ
			FunctionName = STRING("??0C") + Name() + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_pConstructorDefault)
		{
			//Private constructor: ??0 <ClassName> @@AAE@XZ
			FunctionName = STRING("??0C") + Name() + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}
	else
	{
		m_QName = CStringRange(rQName, 0, iNameSpacePos + 1) + CStringRange(rQName, iNameSpacePos + 3);
		m_uiPrefix = iNameSpacePos;

		//Public constructor: ??0 <ClassName> @ <Namespace> @@QAE@XZ
		CString FunctionName = STRING("??0C") + Name() + STRING("@") + Prefix() + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_pConstructorDefault)
		{
			//Protected constructor: ??0 <ClassName> @ <Namespace> @@IAE@XZ
			FunctionName = STRING("??0C") + Name() + STRING("@") + Prefix() + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_pConstructorDefault)
		{
			//Private constructor: ??0 <ClassName> @ <Namespace> @@AAE@XZ
			FunctionName = STRING("??0C") + Name() + STRING("@") + Prefix() + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_pConstructorDefault = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}

	if (!m_pConstructorDefault) { SCFError(ErrorObjectFailedConstructorRegistration); }
}

CClass::~CClass()
{
}

bool CClass::Register(_IN CClass& rClass)
{
	CClass* pClass = (CClass*)Class_ClassesByKey.At(rClass.Key());
	if (!pClass)
	{
		if (rClass.ConstructorDefault())
		{
			Class_ClassesByName.AtPut(rClass.QName(), rClass);
			if (rClass.Key() != 0) { Class_ClassesByKey.AtPut (rClass.Key(), rClass); }
			return TRUE;
		}
	}

	SCFError(ErrorObjectFailedConstructorRegistration);
	return FALSE;
}

bool CClass::Unregister(_IN SCF::ENUM eClassKey)
{
	CClass* pClass = (CClass*)Class_ClassesByKey.Remove(eClassKey);
	if (pClass)
	{
		delete pClass;
		return TRUE;
	}

	return FALSE;
}

bool CClass::IsRegistered(_IN SCF::ENUM eClassKey)
{
	return Class_ClassesByKey.ContainsKey(eClassKey);
}

CObject* CClass::New(_IN SCF::ENUM eClassKey)
{
	CClass* pClass = (CClass*)Class_ClassesByKey.At(eClassKey);
	if (pClass)
	{
		CObject* pObject = (CObject*)Class_ObjectAllocator(pClass->ObjectSize());
		OBJECT_CONSTRUCTOR_DEFAULT ObjectConstructor = pClass->ConstructorDefault();

		_asm 
		{ 
			mov ecx, pObject
			call ObjectConstructor
		}

		return pObject;
	}

	SCFError(ErrorObjectFailedConstructorQuery); 
	return NULL;
}

CClass* CClass::ByQName(_IN CString& rClassQName) { return (CClass*)Class_ClassesByName.At(rClassQName); }
CClass* CClass::ByKey  (_IN SCF::ENUM eClassKey)  { return (CClass*)Class_ClassesByKey.At (eClassKey); }

SCF::ENUM CClass::QNameToKey(_IN CString& rClassQName) { return ((CClass*)Class_ClassesByName.At(rClassQName))->Key(); }
CString   CClass::KeyToQName(_IN SCF::ENUM eClassKey)  { return ((CClass*)Class_ClassesByKey.At (eClassKey))->QName(); }

