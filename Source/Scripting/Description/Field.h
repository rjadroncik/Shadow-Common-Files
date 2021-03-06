#pragma once

#include "Descriptor.h"
#include "Visibilities.h"
#include "../Compiler/Parser/Expression.h"

namespace SCFScripting
{
	class SCRIPTING_API CClass;

	class SCRIPTING_API CField : public CDescriptor
	{
	public:
		CString ToString() _GET{ return STRING("{Field} ") + m_Name; }

	public:
        CField();
        virtual ~CField();

	public:
        void         Visibility(_IN Visibilities eVisibility) _SET { m_eVisibility = eVisibility; }
        Visibilities Visibility()                             _GET { return m_eVisibility; }

        void          Type(_IN CClass& rType) _SET { m_pType = &rType; }
        const CClass& Type()                  _GET { return *m_pType; }

        void               Initializer(_IN _REF CExpression& rInitializer) _SET { m_pInitializer = &rInitializer; }
        const CExpression& Initializer()                                   _GET { return *m_pInitializer; }

        void IsStatic(_IN bool bIsStatic) _SET { m_bIsStatic = bIsStatic; }
        bool IsStatic()                   _GET { return m_bIsStatic; }

        void IsReadOnly(_IN bool bIsReadOnly) _SET { m_bIsReadOnly = bIsReadOnly; }
        bool IsReadOnly()                     _GET { return m_bIsReadOnly; }

	private:
        bool m_bIsStatic;
        bool m_bIsReadOnly;

        const CClass* m_pType;
        const CExpression* m_pInitializer;

        Visibilities m_eVisibility;
	};
};
