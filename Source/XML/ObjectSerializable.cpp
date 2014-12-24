#include "ObjectSerializable.h"

using namespace SCFXML;
using namespace SCFBase;

CXMLObjectSerializable::CXMLObjectSerializable()
{
}

CXMLObjectSerializable::~CXMLObjectSerializable()
{
}

CString CXMLObjectSerializable::ID() _GET
{
	return CPointer(this).ToString();
}

void CXMLObjectSerializable::XMLSerialize(_INOUT IXMLStreamWrite& rWriter) const
{
	SCF_UNREFERENCED_PARAMETER(rWriter);
}

void CXMLObjectSerializable::XMLDeserialize(_INOUT IXMLStreamRead& rReader)
{
	SCF_UNREFERENCED_PARAMETER(rReader);
}

void CXMLObjectSerializable::XMLDependentsSerialize(_INOUT IXMLStreamWriteObject& rWriter) const
{
	SCF_UNREFERENCED_PARAMETER(rWriter);
}

void CXMLObjectSerializable::XMLDependentsDeserialize(_INOUT IXMLStreamReadObject& rReader)
{
	SCF_UNREFERENCED_PARAMETER(rReader);
}

typedef void (__thiscall *OBJECT_CONSTRUCTOR_DEFAULT) ();

namespace SCFXML
{
	class CClass : public CObject
	{
	public:
		//The class key returns the identifier of the class object, but bot the class it describes
		SCF::ENUM ClassKey() _GET { return 0xFFFFFFFF; }
		CString   ToString() _GET { return STRING("{Class}"); }

	public:
		CClass(_IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule);
		virtual ~CClass();

	public:
		inline const CString QName() _GET { return STRINGREF(m_QName); }

	public:
		inline SCF::UINT ObjectSize() _GET { return m_uiObjectSize; }

	public:
		inline OBJECT_CONSTRUCTOR_DEFAULT Constructor() _GET { return m_Constructor; }

	protected:
		CString m_QName;

	protected:
		SCF::UINT m_uiObjectSize;

	protected:
		OBJECT_CONSTRUCTOR_DEFAULT m_Constructor;
	};
}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CClass::CClass(_IN CString& rQualifiedName, _IN SCF::UINT uiObjectSize, _IN void* hModule)
{
	m_uiObjectSize = uiObjectSize;
	m_QName = rQualifiedName;

	static char s_szFunctionName[256];

	CStringSearch Search(rQualifiedName);
	int iNameSpacePos = Search.FindString(":");
	if (iNameSpacePos == -1)
	{
		//Public constructor: ??0 <ClassName> @@QAE@XZ
		CString FunctionName = STRING("??0C") + rQualifiedName + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_Constructor)
		{
			//Protected constructor: ??0 <ClassName> @@IAE@XZ
			FunctionName = STRING("??0C") + rQualifiedName + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_Constructor)
		{
			//Private constructor: ??0 <ClassName> @@AAE@XZ
			FunctionName = STRING("??0C") + rQualifiedName + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}
	else
	{
		CStringRange Name  (rQualifiedName, iNameSpacePos + 1);
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

OBJECT_ALLOCATOR          XMLObjectSerializable_Allocator = NULL;
CDictionaryString<CClass> XMLObjectSerializable_Classes;
CDictionaryInt64          XMLObjectSerializable_ClassesByKey;

bool SCFXMLObjectSerializableInitialize()
{
#ifdef _BETA
	XMLObjectSerializable_Allocator = (OBJECT_ALLOCATOR)GetProcAddress(GetModuleHandle(TEXT("SCFObjectExtensionsD.dll")), "??2CObject@SCFBase@@SGPAXI@Z");
#else
	XMLObjectSerializable_Allocator = (OBJECT_ALLOCATOR)GetProcAddress(GetModuleHandle(TEXT("SCFObjectExtensions.dll")), "??2CObject@SCFBase@@SGPAXI@Z");
#endif
	return TRUE;
}

bool SCFXMLObjectSerializableCleanUp()
{
	XMLObjectSerializable_Allocator = NULL;
	XMLObjectSerializable_ClassesByKey.AllRemove();
	XMLObjectSerializable_Classes.AllDelete();

	return TRUE;
}

bool CXMLObjectSerializable::ClassIsRegistered(_IN CString& rQualifiedName)
{
	return XMLObjectSerializable_Classes.ContainsName(rQualifiedName);
}
bool CXMLObjectSerializable::ClassIsRegistered(_IN SCF::ENUM eClassKey)
{
	return XMLObjectSerializable_ClassesByKey.ContainsKey(eClassKey);
}

CString CXMLObjectSerializable::ClassKeyToQName(_IN SCF::ENUM eClassKey) 
{ 
	CClass* pClass = (CClass*)XMLObjectSerializable_ClassesByKey.At(eClassKey);
	if (pClass)
	{
		return pClass->QName(); 
	}

	return CString();
}

bool CXMLObjectSerializable::ClassRegister(_IN SCF::ENUM eClassKey, _IN CString& rQualifiedNameCPP, _IN SCF::UINT uiObjectSize, _IN void* hModule)
{
	static CString Name;

	CStringSearch Search(rQualifiedNameCPP);
	int iNameSpacePos = Search.FindString("::");
	if (iNameSpacePos == -1)
	{
		Name = CStringRange(rQualifiedNameCPP, 1);
	}
	else
	{
		Name = CStringRange(rQualifiedNameCPP, 0, iNameSpacePos + 1) + CStringRange(rQualifiedNameCPP, iNameSpacePos + 3);
	}

	if (Name.Length() && !XMLObjectSerializable_Classes.ContainsName(Name))
	{
		CClass* pClass = new CClass(Name, uiObjectSize, hModule);
		if (pClass->Constructor())
		{
			XMLObjectSerializable_Classes.AtPut(Name, *pClass);
			XMLObjectSerializable_ClassesByKey.AtPut(eClassKey, *pClass);
		}
		return TRUE;
	}

	SCFError(ErrorObjectFailedConstructorRegistration);
	return FALSE;
}

bool CXMLObjectSerializable::ClassUnregister(_IN CString& rQualifiedName)
{
	CClass* pClass = (CClass*)XMLObjectSerializable_Classes.RemoveKey(rQualifiedName);
	if (pClass)
	{
		delete pClass;
		return TRUE;
	}

	return FALSE;
}

CXMLObjectSerializable* CXMLObjectSerializable::New(_IN CString& rQualifiedName)
{
	CClass* pClass = (CClass*)XMLObjectSerializable_Classes.At(rQualifiedName);
	if (pClass)
	{
		CXMLObjectSerializable* pObject = (CXMLObjectSerializable*)XMLObjectSerializable_Allocator(pClass->ObjectSize());
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
