#pragma once

#include "StringRange.h"
#include "Descriptor.h"

#define CLASS_CREATE(classKey, class, module, classParent) *(new SCFBase::CClass(classKey, STRING(#class), sizeof(class), module, SCFBase::CClass::ByQName(#classParent)))

namespace SCFBase
{
	typedef void (__thiscall *OBJECT_CONSTRUCTOR_DEFAULT) ();

	class OBJECT_EXTENSIONS_API CClass : public CDescriptor
	{
	//////////////////////////// CClass - static part ////////////////////////////////

	//These support dynamic object creation, once a class is registered
	public:
		//Registering a class requires it's constructor to be exported form a DLL,
		//destructors must always be virtual & implicit linking is presumed
		static bool Register(_IN CClass& rClass);
		static bool Unregister(_IN SCF::ENUM eClassKey);

	public:
		static bool IsRegistered(_IN SCF::ENUM eClassKey);

	public:
		//You can get a class descriptor based on a name or the class-key of the described class
		static CClass* ByQName(_IN CString& rClassName);
		static CClass* ByKey  (_IN SCF::ENUM eClassKey);

	public:
		static SCF::ENUM QNameToKey(_IN CString& rClassName);
		static CString   KeyToQName(_IN SCF::ENUM eClassKey);

	public:
		//Create a new object based on a registered class key
		static CObject* New(_IN SCF::ENUM eClassKey);

	public:
		//The class key returns the identifier of the class object, but bot the class it desribes
		SCF::ENUM ClassKey() _GET { return ClassClass; }
		CString   AsString() _GET { return STRING("{Class}"); }

	//////////////////////////// CClass - definition /////////////////////////////////
	public:
		//Creates a new object of the type described by the used class object
		CObject* New() _GET;

	public:
		CClass(_IN SCF::ENUM eKey, _IN CString& rQName, _IN SCF::UINT uiObjectSize, _IN void* hModule, _IN _REF CClass* pParent = NULL);
		virtual ~CClass();
	
	public:
		//Key & qualified name assigned to the class object (the ones belonging to the described class)
		inline SCF::ENUM     Key()   _GET { return m_eKey; }
		inline const CString QName() _GET { return STRINGREF(m_QName); }

	public:
		//Short name is stripped of the namespace part
		inline const CString Name()   _GET { return m_uiPrefix ? CStringRange(m_QName, m_uiPrefix + 1) : STRINGREF(m_QName); }
		inline const CString Prefix() _GET { return CStringRange(m_QName, 0, m_uiPrefix); }

	public:
		CClass* Parent() _GET { return (CClass*)m_pParent; }

	public:
		SCF::UINT ObjectSize() _GET { return m_uiObjectSize; }

	public:
		OBJECT_CONSTRUCTOR_DEFAULT ConstructorDefault() _GET { return m_pConstructorDefault; }

	protected:
		CString m_QName;
		SCF::UINT m_uiPrefix;

	protected:
		SCF::ENUM m_eKey;

	protected:
		const CClass* m_pParent;

	protected:
		SCF::UINT  m_uiObjectSize;

	protected:
		OBJECT_CONSTRUCTOR_DEFAULT m_pConstructorDefault;
	};
};
