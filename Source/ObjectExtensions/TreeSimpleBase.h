#pragma once

#include "TreeSimpleRaw.h"

namespace SCFBase
{
	template<class TValue, class TNode>
	class CTreeSimpleBase : public CTreeSimpleRaw
	{
	public:
		//Destructor DOES NOT delete the value, it just releases it! (can override this behavior)
		CTreeSimpleBase() {}
		CTreeSimpleBase(_IN _REF TValue& rValue) : CTreeSimpleRaw(rValue) {}
		virtual ~CTreeSimpleBase() {}

	public:
		///////////////////////// Miscellaneous ///////////////////////////

		//Replaces the current node with a different one within a tree hierarchy, DOES NOT delete the one being replaced!
		inline void ReplaceWith(_INOUT TNode& rNode) { CTreeSimpleRaw::ReplaceWith(rNode); }

	public:
		///////////////////////// Relatives ///////////////////////////

		inline TNode* Parent()                 _GET { return (TNode*)m_pParent; }
		inline void   Parent(_IN TNode* pNode) _SET { m_pParent = (TNode*)pNode; }

		inline TNode* Previous()                 _GET { return (TNode*)m_pPrevious; }
		inline void   Previous(_IN TNode* pNode) _SET { m_pPrevious = (TNode*)pNode; }

		inline TNode* Next()                 _GET { return (TNode*)m_pNext; }
		inline void   Next(_IN TNode* pNode) _SET { m_pNext = (TNode*)pNode; }

	public:
		///////////////////////// Children ///////////////////////////

		inline TNode* ChildFirst()                 _GET { return (TNode*)m_pChildFirst; }
		inline void   ChildFirst(_IN TNode* pNode) _SET { m_pChildFirst = (TNode*)pNode; }

		inline TNode* ChildLast()                 _GET { return (TNode*)m_pChildLast; }
		inline void   ChildLast(_IN TNode* pNode) _SET { m_pChildLast = (TNode*)pNode; }

		//Add or removes a node from the child list, DOES NOT create or delete the node!
		inline void ChildAdd     (_INOUT _REF TNode& rChild) { CTreeSimpleRaw::ChildAdd(rChild); }
		inline void ChildAddFirst(_INOUT _REF TNode& rChild) { CTreeSimpleRaw::ChildAddFirst(rChild); }
		inline void ChildRemove  (_INOUT      TNode& rChild) { CTreeSimpleRaw::ChildRemove(rChild); }

	public:
		inline void ChildReplace(_INOUT TNode& rChildOld, _INOUT TNode& rChildNew) { CTreeSimpleRaw::ChildReplace(rChildOld, rChildNew); }

		//Add a node at a specific position in the list
		//void ChildInsertBefore(_IN TNode* pChildNew, _IN TNode* pChildNext); 
		//void ChildInsertAfter (_IN TNode* pChildNew, _IN TNode* pChildPrevious); 

	public:
		///////////////////////// Value ///////////////////////////

		inline bool    ValueHas()                     _GET { return (m_pValue == nullptr) ? (0) : (1); }
		inline TValue* Value()                        _GET { return (TValue*)m_pValue; }
		inline void    Value(_IN _REF TValue* pValue) _SET { BETAONLY(if (m_pValue) { m_pValue->Release(); }) m_pValue = (CObject*)pValue; BETAONLY(if (m_pValue) { m_pValue->AddRef(); }) }

		inline const CString ValueString() _GET { if (m_pValue) { return m_pValue->ToString(); } else { return CString(); } }
	};
};