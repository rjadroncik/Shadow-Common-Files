#pragma once

#include "String.h"
#include "BagNodeObject.h"
#include "FSBHeap.h"
#include "Comparer.h"
#include "Container.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CBagObject : public CObject, public IContainer<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorBagObject;

	public:
		CString ToString() _GET { return STRING("{BagObject}"); }

	public:
		CBagObject(_IN CComparer& rComparer, _IN bool bTakeOwnage = TRUE);
		virtual ~CBagObject();

	public:
		void Add     (_IN _REF CObject& rObject);
		void Remove  (_IN      CObject& rObject);
		bool Contains(_IN      CObject& rObject) _GET;

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

	protected:
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CBagNodeObject* m_pNodeRoot;
		UINT m_uiCount;

	protected:
		//A fixed-size block heap used to store the bag nodes
		//SCFPrivate::CFSBHeap m_Heap;

	protected:
		const CComparer* m_pComparer;
		bool m_bComparerOwned;

	protected:
		CBagObject();
	};
};
