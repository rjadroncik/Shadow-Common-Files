#pragma once

#include "String.h"
#include "BagNodeInt64.h"
#include "FSBHeap.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CBagInt64 : public CContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorBagInt64;

	public:
		SCF::ENUM ClassKey() _GET { return ClassBagInt64; }
		CString   ToString() _GET { return STRING("{BagInt64}"); }

	public:
		CEnumerator* EnumeratorNew() _GET;

	public:
		CBagInt64();
		virtual ~CBagInt64();

	public:
		void Add     (_IN SCF::UINT64 ui64Value);
		void Remove  (_IN SCF::UINT64 ui64Value);
		bool Contains(_IN SCF::UINT64 ui64Value) _GET;

	public:
		//Removes all objects without deleting them
		void AllRemove();

		//Removes & deletes all contained objects
		void AllDelete();

		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	public:
		SCF::UINT Size()    _GET { return m_uiCount; }
		bool      IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CBagNodeInt64* m_pNodeRoot;
		SCF::UINT m_uiCount;

	protected:
		//SCFPrivate::CFSBHeap m_Heap;
	};
};
