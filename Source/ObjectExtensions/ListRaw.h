#pragma once

#include "String.h"
#include "ListNode.h"
#include "FSBHeap.h"
#include "Comparer.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CListRaw : public CContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorList;

	public:
		SCF::ENUM ClassKey() _GET { return ClassList; }
		CString   ToString() _GET { return STRING("{List}"); }

	public:
		CEnumerator* EnumeratorNew() _GET;

	public:
		CListRaw();
		virtual ~CListRaw();

	public:
		CObject& At         (_IN SCF::UINT uiIndex) _GET;
		CObject& operator [](_IN SCF::UINT uiIndex) _GET;

		void AtPut(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject) _SET;

	public:
		inline CObject& Last() _GET { _ASSERTE(m_uiCount > 0); return m_pNodeLast->Object(m_pNodeLast->Count() - 1); }

	public:
		void LastAdd(_IN _REF CObject& rObject);
		void LastRemove();
		void LastDelete();

	public:
		void FirstAdd(_IN _REF CObject& rObject);
		void FirstRemove();
		void FirstDelete();

	public:
		void Insert(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject);
		void Remove(_IN SCF::UINT uiIndex);
		void Delete(_IN SCF::UINT uiIndex);

	public:
		void Remove  (_IN CObject& rObject);
		bool Contains(_IN CObject& rObject) _GET;

	public:
		//Adds all objects provided by the enumerator
		void AllAdd(_INOUT CEnumerator& rEnumerator);

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

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		//The first & last node of the bidirectional linked list used to store the data
		SCFPrivate::CListNode* m_pNodeFirst;
		SCFPrivate::CListNode* m_pNodeLast;

		//The number of stored objects, NOT the number of nodes
		SCF::UINT m_uiCount;

	protected:
		//A fixed-size block heap used to store the list nodes
		//SCFPrivate::CFSBHeap m_Heap;
	};
};
