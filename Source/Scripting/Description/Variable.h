#pragma once

#include "Descriptor.h"
#include "Visibilities.h"

namespace SCFScripting
{
    class SCRIPTING_API CClass;

    class SCRIPTING_API CVariable : public CDescriptor
    {
    public:
        CString ToString() _GET { return STRING("{Variable} ") + m_Name; }

    public:
        CVariable();
        virtual ~CVariable();

    public:
        void         Visibility(_IN Visibilities eVisibility) _SET { m_eVisibility = eVisibility; }
        Visibilities Visibility()                             _GET { return m_eVisibility; }

        void          Type(_IN CClass& rType) _SET { m_pType = &rType; }
        const CClass& Type()                  _GET { return *m_pType; }

    private:
        bool m_bIsStatic;
        bool m_bIsReadOnly;
        const CClass* m_pType;
        Visibilities m_eVisibility;
    };
}
