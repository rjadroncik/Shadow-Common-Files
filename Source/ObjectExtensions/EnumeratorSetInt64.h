#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "SetNodeInt64.h"

namespace SCFBase
{
    class OBJECT_EXTENSIONS_API CSetInt64;

    class OBJECT_EXTENSIONS_API CEnumeratorSetInt64 : public CEnumeratorRaw, IEnumerator<UINT64>
    {
        friend class OBJECT_EXTENSIONS_API CSetInt64;

    public:
        CString ToString() _GET { return STRING("{EnumeratorSetInt64}"); }

    public:
        CEnumeratorSetInt64(_IN CSetInt64& rBag);
        virtual ~CEnumeratorSetInt64();

    public:
        //Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
        bool Next() { return CEnumeratorRaw::ProtectedNext(); }

    public:
        //Returns true while there still is a next element to be enumerated
        bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }

    public:
        //This is correct :)
        UINT64* Current() _GET { return (UINT64*)m_paNodes[MAX_DEPTH_BAG_INT64]; }

    public:
        void CurrentRemove();
        void CurrentDelete();

    protected:
        bool NextStart();
        bool NextContinue();
        bool NextEnd() { m_pCurrent = NULL; return FALSE; }

    protected:
        SCFPrivate::CSetNodeInt64* m_paNodes  [MAX_DEPTH_BAG_INT64 + 1];
        char                       m_caIndexes[MAX_DEPTH_BAG_INT64];
    };
};