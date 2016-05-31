#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFBase { class OBJECT_EXTENSIONS_API CEnumeratorList; };

namespace SCFPrivate
{
	class CListNode
	{
	private:
		CListNode();
		~CListNode();

	public:
		//Used instead of constructors 
		static CListNode* Create(_IN CObject& rObject, _IN CListNode* pPrevious = NULL, _IN CListNode* pNext = NULL);

		static void Delete           (_IN CListNode* pNode);
		static void DeleteWithObjects(_IN CListNode* pNode);

	public:
		inline bool IsFull()  _GET { return (m_ucCount >= ALLOC_GRANULARITY_PTRS); }
		inline bool IsEmpty() _GET { return (m_ucCount == 0); }
	
	public:
		void ObjectLastAdd(_IN _REF CObject& rObject);
		void ObjectLastRemove();
		void ObjectLastDelete();

		void ObjectFirstAdd(_IN _REF CObject& rObject);
		void ObjectFirstRemove();
		void ObjectFirstDelete();

	public:
		void ObjectInsert(_IN UINT uiIndex, _IN _REF CObject& rObject);
		void ObjectRemove(_IN UINT uiIndex);
		void ObjectDelete(_IN UINT uiIndex);

	public:
		inline CObject& Object(_IN BYTE ucIndex)                            _GET { _ASSERTE(ucIndex < m_ucCount); return *(m_paObjects[ucIndex]); }
		inline void     Object(_IN BYTE ucIndex, _IN _REF CObject& rObject) _SET { _ASSERTE(ucIndex < m_ucCount); RELEASE(*(m_paObjects[ucIndex])); m_paObjects[ucIndex] = &(CObject&)rObject; ADDREF(*(m_paObjects[ucIndex])); }

		inline BYTE Count()                 _GET { return m_ucCount; }
		inline void Count(_IN BYTE ucCount) _SET { m_ucCount = ucCount; }

	public:
		inline CListNode* Previous()                              _GET { return m_pPrevious; }
		inline void       Previous(_IN _REF CListNode* pPrevious) _SET { m_pPrevious = (CListNode*)pPrevious; }

		inline CListNode* Next()                          _GET { return m_pNext; }
		inline void       Next(_IN _REF CListNode* pNext) _SET { m_pNext = (CListNode*)pNext; }


	//Deserialization versions of a few functions, these don't perform some of the steps included in the originals. depending on the needs of the deserialization process
	public:
		//Does not assign the value of the [Next] attribute nor does it assign any object
		static CListNode* Deserialization_Create(_IN CListNode* pPrevious = NULL);
		
		//Does not release previous object (as there is none during deserialization
		inline void Deserialization_Object(_IN BYTE ucIndex, _IN _REF CObject& rObject) _SET { _ASSERTE(ucIndex < m_ucCount); m_paObjects[ucIndex] = &(CObject&)rObject; ADDREF(*(m_paObjects[ucIndex])); }

	private:
		CListNode* m_pPrevious;
		CListNode* m_pNext;

	private:
		CObject* m_paObjects[ALLOC_GRANULARITY_PTRS];
		BYTE m_ucCount;
	};
};