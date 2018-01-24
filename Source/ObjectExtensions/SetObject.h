#pragma once

#include "String.h"
#include "SetNodeObject.h"
#include "FSBHeap.h"
#include "Comparer.h"
#include "Container.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CSetObject : public CObject, public IContainer<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorSetObject;

	public:
		CString ToString() _GET { return STRING("{BagObject}"); }

	public:
		CSetObject(_IN CComparer& rComparer, _IN bool bTakeOwnage = true);
		virtual ~CSetObject();

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
		SCFPrivate::CSetNodeObject* m_pNodeRoot;
		UINT m_uiCount;

	protected:
		//A fixed-size block heap used to store the bag nodes
		//SCFPrivate::CFSBHeap m_Heap;

	protected:
		const CComparer* m_pComparer;
		bool m_bComparerOwned;

	protected:
		CSetObject();
	};
};
