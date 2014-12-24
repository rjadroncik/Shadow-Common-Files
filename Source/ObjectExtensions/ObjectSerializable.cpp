#include "ObjectSerializable.h"

#include "String.h"
#include "Pointer.h"
#include "DictionaryInt64.h"
#include "Vector.h"
#include "Int.h"

using namespace SCFBase;

CObjectSerializable::CObjectSerializable()
{
}

CObjectSerializable::~CObjectSerializable()
{
}

void CObjectSerializable::Serialize(_INOUT IStreamWrite& rStream) const
{
	SCF_UNREFERENCED_PARAMETER(rStream);
}

void CObjectSerializable::Deserialize(_INOUT IStreamRead& rStream)
{
	SCF_UNREFERENCED_PARAMETER(rStream);
}

void CObjectSerializable::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	SCF_UNREFERENCED_PARAMETER(rStream);
}

void CObjectSerializable::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	SCF_UNREFERENCED_PARAMETER(rStream);
}

typedef void (__thiscall *OBJECT_CONSTRUCTOR_DEFAULT) ();

class CClass : public CObject
{
public:
	//The class key returns the identifier of the class object, but bot the class it desribes
	SCF::ENUM ClassKey() _GET { return 0xFFFFFFFF; }
	CString   ToString() _GET { return STRING("{Class}"); }

public:
	CClass(_IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule);
	virtual ~CClass();

public:
	inline SCF::UINT ObjectSize() _GET { return m_uiObjectSize; }

public:
	inline OBJECT_CONSTRUCTOR_DEFAULT Constructor() _GET { return m_Constructor; }

protected:
	SCF::UINT m_uiObjectSize;

protected:
	OBJECT_CONSTRUCTOR_DEFAULT m_Constructor;
};

#include "StringRange.h"
#include "StringSearch.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CClass::CClass(_IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule)
{
	m_uiObjectSize = uiObjectSize;

	static char s_szFunctionName[256];

	CStringSearch Search(rQualifiedName);
	int iNameSpacePos = Search.FindString("::");
	if (iNameSpacePos == -1)
	{
		CStringRange Name(rQualifiedName, 1);

		//Public constructor: ??0 <ClassName> @@QAE@XZ
		CString FunctionName = STRING("??0C") + Name + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_Constructor)
		{
			//Protected constructor: ??0 <ClassName> @@IAE@XZ
			FunctionName = STRING("??0C") + Name + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_Constructor)
		{
			//Private constructor: ??0 <ClassName> @@AAE@XZ
			FunctionName = STRING("??0C") + Name + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}
	else
	{
		CStringRange Name  (rQualifiedName, iNameSpacePos + 3);
		CStringRange Prefix(rQualifiedName, 0, iNameSpacePos);

		//Public constructor: ??0 <ClassName> @ <Namespace> @@QAE@XZ
		CString FunctionName = STRING("??0C") + Name + STRING("@") + Prefix + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_Constructor)
		{
			//Protected constructor: ??0 <ClassName> @ <Namespace> @@IAE@XZ
			FunctionName = STRING("??0C") + Name + STRING("@") + Prefix + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_Constructor)
		{
			//Private constructor: ??0 <ClassName> @ <Namespace> @@AAE@XZ
			FunctionName = STRING("??0C") + Name + STRING("@") + Prefix + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}

	if (!m_Constructor) { SCFError(ErrorObjectFailedConstructorRegistration); }
}

CClass::~CClass()
{
}

typedef void* (__stdcall *OBJECT_ALLOCATOR) (unsigned int);

OBJECT_ALLOCATOR ObjectSerializable_Allocator = NULL;
CDictionaryInt64 ObjectSerializable_Classes;

bool SCFObjectSerializableInitialize(void* hModule)
{
	ObjectSerializable_Allocator = (OBJECT_ALLOCATOR)GetProcAddress((HMODULE)hModule, "??2CObject@SCFBase@@SGPAXI@Z");
	return TRUE;
}

bool SCFObjectSerializableCleanUp()
{
	ObjectSerializable_Allocator = NULL;
	ObjectSerializable_Classes.AllDelete();

	return TRUE;
}

bool CObjectSerializable::ClassRegister(_IN SCF::ENUM eClassKey, _IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule)
{
	if (eClassKey && !ObjectSerializable_Classes.ContainsKey(eClassKey))
	{
		CClass* pClass = new CClass(rQualifiedName, uiObjectSize, hModule);
		if (pClass->Constructor())
		{
			ObjectSerializable_Classes.AtPut(eClassKey, *pClass);
			return TRUE;
		}
	}

	SCFError(ErrorObjectFailedConstructorRegistration);
	return FALSE;
}

bool CObjectSerializable::ClassUnregister(_IN SCF::ENUM eClassKey)
{
	CClass* pClass = (CClass*)ObjectSerializable_Classes.Remove(eClassKey);
	if (pClass)
	{
		delete pClass;
		return TRUE;
	}

	return FALSE;
}

bool CObjectSerializable::ClassIsRegistered(_IN SCF::ENUM eClassKey)
{
	return ObjectSerializable_Classes.ContainsKey(eClassKey);
}

CObjectSerializable* CObjectSerializable::New(_IN SCF::ENUM eClassKey)
{
	CClass* pClass = (CClass*)ObjectSerializable_Classes.At(eClassKey);
	if (pClass)
	{
		CObjectSerializable* pObject = (CObjectSerializable*)ObjectSerializable_Allocator(pClass->ObjectSize());
		OBJECT_CONSTRUCTOR_DEFAULT ObjectConstructor = pClass->Constructor();

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
