#pragma once

#include "Method.h"
#include "Property.h"
#include "Field.h"
#include "Interface.h"

namespace SCFScripting
{
	class SCRIPTING_API CClass : public CDescriptor
	{
	public:
		CString ToString() _GET { return STRING("{Class} ") + m_Name; }

	public:
		CClass();
		virtual ~CClass();

	public:
		CMethod*   Method  (_IN CString& rName, _IN CList<CClass>& rArgumentTypes) _GET;
		CProperty* Property(_IN CString& rName) _GET;

	public:
		void MethodAdd  (_IN _REF CMethod&   rMethod)   _SET;
		void PropertyAdd(_IN _REF CProperty& rProperty) _SET;

		void ConstructorAdd(_IN _REF CMethod&    rConstructor) _SET;
		void InterfaceAdd  (_IN _REF CInterface& rInterface)   _SET;
	
	public:
		const CList<CMethod>&   Methods()    _GET { return m_Methods; }
		const CList<CProperty>& Properties() _GET { return m_Properties; }

		const CList<CMethod>&    Constructors() _GET { return m_Constructors; }
		const CList<CInterface>& Interfaces()   _GET { return m_Interfaces; }

	public:
		CClass* BaseClass()                       _GET { return m_pBaseClass; }
		void    BaseClass(_IN CClass* pBaseClass) _SET { m_pBaseClass = (CClass*)pBaseClass; }

	private:
		#pragma warning(disable:4251)
		CList<CMethod>   m_Methods;
		CList<CProperty> m_Properties;
		CList<CField>    m_Fields;

		CList<CMethod>    m_Constructors; 
		CList<CInterface> m_Interfaces; 
		#pragma warning(default:4251)

	private:
		CClass* m_pBaseClass;
	};
};
