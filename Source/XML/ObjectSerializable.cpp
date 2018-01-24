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
		ENUM    ClassKey() _GET { return 0xFFFFFFFF; }
		CString ToString() _GET { return STRING("{Class}"); }

	public:
		CClass(_IN CString& rFullName, _IN UINT uiObjectSize, _IN void* hModule);
		virtual ~CClass();

	public:
		inline const CString FullName() _GET { return STRINGREF(m_FullName); }

		inline const CStringRange Name()   _GET { return CStringRange(m_FullName, m_uiNamespaceLength + 2); }
		inline const CStringRange Prefix() _GET { return CStringRange(m_FullName, 0, m_uiNamespaceLength); }

	public:
		inline UINT ObjectSize() _GET { return m_uiObjectSize; }

	public:
		inline OBJECT_CONSTRUCTOR_DEFAULT Constructor() _GET { return m_Constructor; }

	protected:
		CString m_FullName;
		UINT m_uiNamespaceLength;

	protected:
		UINT m_uiObjectSize;

	protected:
		OBJECT_CONSTRUCTOR_DEFAULT m_Constructor;
	};
}

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CClass::CClass(_IN CString& rFullName, _IN UINT uiObjectSize, _IN void* hModule)
{
	m_uiObjectSize = uiObjectSize;
	m_FullName = rFullName;

	static char s_szFunctionName[256];

	CStringSearch Search(rFullName);
	int iDelimiterPos = Search.FindString("::");
	if (iDelimiterPos == -1)
	{
		m_uiNamespaceLength = 0;

		//Public constructor: ??0 <ClassName> @@QAE@XZ
		CString FunctionName = STRING("??0") + rFullName + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_Constructor)
		{
			//Protected constructor: ??0 <ClassName> @@IAE@XZ
			FunctionName = STRING("??0") + rFullName + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_Constructor)
		{
			//Private constructor: ??0 <ClassName> @@AAE@XZ
			FunctionName = STRING("??0") + rFullName + STRING("@@AAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}
	}
	else
	{
		m_uiNamespaceLength = iDelimiterPos;

		CStringRange Name  (rFullName, m_uiNamespaceLength + 2);
		CStringRange Prefix(rFullName, 0, m_uiNamespaceLength);

		//Public constructor: ??0 <ClassName> @ <Namespace> @@QAE@XZ
		CString FunctionName = STRING("??0") + Name + STRING("@") + Prefix + STRING("@@QAE@XZ");
		FunctionName.ToASCII(s_szFunctionName);
		m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);

		if (!m_Constructor)
		{
			//Protected constructor: ??0 <ClassName> @ <Namespace> @@IAE@XZ
			FunctionName = STRING("??0") + Name + STRING("@") + Prefix + STRING("@@IAE@XZ");
			FunctionName.ToASCII(s_szFunctionName);
			m_Constructor = (OBJECT_CONSTRUCTOR_DEFAULT)GetProcAddress((HMODULE)hModule, s_szFunctionName);
		}

		if (!m_Constructor)
		{
			//Private constructor: ??0 <ClassName> @ <Namespace> @@AAE@XZ
			FunctionName = STRING("??0") + Name + STRING("@") + Prefix + STRING("@@AAE@XZ");
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

OBJECT_ALLOCATOR          XMLObjectSerializable_Allocator = nullptr;
CDictionaryString<CClass> XMLObjectSerializable_Classes;

bool SCFXMLObjectSerializableInitialize()
{
#ifdef _BETA
	XMLObjectSerializable_Allocator = (OBJECT_ALLOCATOR)GetProcAddress(GetModuleHandle(TEXT("SCFObjectExtensionsD.dll")), "??2CObject@SCFBase@@SGPAXI@Z");
#else
	XMLObjectSerializable_Allocator = (OBJECT_ALLOCATOR)GetProcAddress(GetModuleHandle(TEXT("SCFObjectExtensions.dll")), "??2CObject@SCFBase@@SGPAXI@Z");
#endif
	return true;
}

bool SCFXMLObjectSerializableCleanUp()
{
	XMLObjectSerializable_Allocator = nullptr;
	XMLObjectSerializable_Classes.AllDelete();

	return true;
}

bool CXMLObjectSerializable::ClassIsRegistered(_IN CString& rClassName)
{
	return XMLObjectSerializable_Classes.ContainsName(rClassName);
}

bool CXMLObjectSerializable::ClassRegister(_IN CString& rXmlName, _IN CString& rFullClassName, _IN UINT uiObjectSize, _IN void* hModule)
{
	if (XMLObjectSerializable_Classes.ContainsName(rXmlName))
	{
		return true;
	}

	CClass* pClass = new CClass(rFullClassName, uiObjectSize, hModule);
	if (pClass->Constructor())
	{
		XMLObjectSerializable_Classes.AtPut(rXmlName, *pClass);
		return true;
	}

	SCFError(ErrorObjectFailedConstructorRegistration);
	return false;
}

bool CXMLObjectSerializable::ClassUnregister(_IN CString& rXmlName)
{
	CClass* pClass = (CClass*)XMLObjectSerializable_Classes.RemoveKey(rXmlName);
	if (pClass)
	{
		delete pClass;
		return true;
	}

	return false;
}

CXMLObjectSerializable* CXMLObjectSerializable::New(_IN CString& rXmlName)
{
	CClass* pClass = (CClass*)XMLObjectSerializable_Classes.At(rXmlName);
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
	return nullptr;
}
