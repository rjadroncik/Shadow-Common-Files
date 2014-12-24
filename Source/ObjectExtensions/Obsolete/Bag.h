#pragma once

#include "BagNode.h"
#include "FSBHeap.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CBag : public CObject
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 
		ENUM ClassKey() _GET { return ClassVector; }

	public:
		CBag();
		virtual ~CBag();

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
		CString AsString() _GET { return STRING("{Bag}"); }

	public:
		inline UINT Size()    _GET { return m_Heap.AllocatedBytes() / sizeof(SCFPrivate::CBagNode); }
		inline bool IsEmpty() _GET { return (m_Heap.AllocatedBytes() == 0); }

	protected:
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CBagNode* m_pNodeRoot;

	protected:
		SCFPrivate::CFSBHeap m_Heap;
	};
};
