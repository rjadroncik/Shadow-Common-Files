#pragma once

#include "String.h"
#include "SetNodeInt64.h"
#include "FSBHeap.h"
#include "Container.h"
#include "EnumeratorSetInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CSetInt64 : public CObject, public IContainer<UINT64>
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorSetInt64;

	public:
		CString ToString() _GET { return STRING("{BagInt64}"); }

	public:
		CSetInt64();
		virtual ~CSetInt64();

	public:
		void Add     (_IN UINT64 ui64Value);
		void Remove  (_IN UINT64 ui64Value);
		bool Contains(_IN UINT64 ui64Value) _GET;

	public:
		//Removes all objects without deleting them
		void AllRemove();

		//Removes & deletes all contained objects
		void AllDelete();

		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	public:
		UINT Size()    _GET { return m_uiCount; }
		bool IsEmpty() _GET { return (m_uiCount == 0); }

		inline IEnumerator<UINT64>& NewEnumerator() _GET { return *(new CEnumeratorSetInt64(*this)); }

	protected:
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CSetNodeInt64* m_pNodeRoot;
		UINT m_uiCount;

	protected:
		//SCFPrivate::CFSBHeap m_Heap;
	};
};
