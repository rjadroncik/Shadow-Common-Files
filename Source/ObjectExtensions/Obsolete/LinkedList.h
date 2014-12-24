#pragma once
#include "object.h"

namespace SCFObjectExtensions
{
	class CLinkedList : public CObject
	{
	public:
		CLinkedList();
		virtual ~CLinkedList();

		_PENDING void AddLast (_INREF CObject& rObject);
		_PENDING void AddFirst(_INREF CObject& rObject);

		_PENDING void Insert  (_INREF CObject& rObject, _IN CObject* pPrevious);
		_PENDING; //void Insert  (_INREF CObject& rObject, _IN CEnumerator& rEnumerator);

	private:

		//Each node _IN the linked list holds pointers to its siblings
		//The siblings are automatically modified on node creation/deletion to maintain list consistency
		class CNode //: public CObject
		{
		public:
			CNode(_IN CObject& rObject, _IN CNode* pPrevious, _IN CNode* pNext);
			virtual ~CNode();

			inline CObject* Object()                        { return m_pObject; }
			inline void     Object(_INREF CObject* pObject) { m_pObject = (CObject*)pObject; }

			inline CNode* Previous()                        { return m_pPrevious; }
			inline void   Previous(_INREF CNode* pPrevious) { m_pPrevious = (CNode*)pPrevious; }

			inline CNode* Next()                            { return m_pNext; }
			inline void   Next(_INREF CNode* pNext)         { m_pNext = (CNode*)pNext; }

		private:

			CObject* m_pObject;

 			CNode*   m_pPrevious;
			CNode*   m_pNext;
		};
	};
};
