#pragma once
#include "Object.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CTreeRegular : public CObject
	{
	public:
		SCF::UINT ClassKey() _GET { return ClassTreeRegular; }

		//Destructor DOES NOT delete the value, it just releases it! (can override this behavior)
		CTreeRegular();
		CTreeRegular(_IN _REF CObject& rObject);
		virtual ~CTreeRegular();

	public:
		///////////////////////// Miscellaneous ///////////////////////////

		//Replaces the current node with a different one within a tree hierarchy, DOES NOT delete the one being replaced!
		void ReplaceWith(_INOUT CTreeRegular& rNode);
		void DeleteSiblings();

		//Leaf related functions
		inline bool IsLeaf() _GET { return (m_pChildFirst == NULL) ? (1) : (0); }
		SCF::UINT Level(_IN SCF::UINT uiLevelsTraversed = 0) _GET;

	public:
		///////////////////////// Relatives ///////////////////////////

		inline CTreeRegular* Parent()                        _GET { return m_pParent; }
		inline void	         Parent(_IN CTreeRegular* pNode) _SET { m_pParent = (CTreeRegular*)pNode; }

		inline CTreeRegular* Previous()                        _GET { return m_pPrevious; }
		inline void          Previous(_IN CTreeRegular* pNode) _SET { m_pPrevious = (CTreeRegular*)pNode; }

		inline CTreeRegular* Next()                        _GET { return m_pNext; }
		inline void          Next(_IN CTreeRegular* pNode) _SET { m_pNext = (CTreeRegular*)pNode; }

	public:
		///////////////////////// Children ///////////////////////////

		inline CTreeRegular* ChildFirst()                        _GET { return m_pChildFirst; }
		inline void          ChildFirst(_IN CTreeRegular* pNode) _SET { m_pChildFirst = (CTreeRegular*)pNode; }

		inline CTreeRegular* ChildLast()                        _GET { return m_pChildLast; }
		inline void          ChildLast(_IN CTreeRegular* pNode) _SET { m_pChildLast = (CTreeRegular*)pNode; }

		//Add or removes a node from the child list, DOES NOT create or delete the node!
		void ChildAdd   (_INOUT _REF CTreeRegular& rChild);
		void ChildRemove(_INOUT      CTreeRegular& rChild);

		//Add a node at a specific position in the list
		//void ChildInsertBefore(_IN CTreeRegular* pChildNew, _IN CTreeRegular* pChildNext); 
		//void ChildInsertAfter (_IN CTreeRegular* pChildNew, _IN CTreeRegular* pChildPrevious); 

	public:
		///////////////////////// Value ///////////////////////////

		inline bool     ValueHas()                      _GET { return (m_pValue == NULL) ? (0) : (1); }
		inline CObject* Value()                         _GET { return m_pValue; }
		inline void     Value(_IN _REF CObject* pValue) _SET { if (m_pValue) { m_pValue->Release(); } m_pValue = (CObject*)pValue; if (m_pValue) { m_pValue->AddRef(); } }

		inline const CString ValueString() _GET { if (m_pValue) { return m_pValue->AsString(); } else { return CString(); } }

	protected:
		//Basic data
		CObject* m_pValue;

		//Information about relatives
		CTreeRegular* m_pParent;
		CTreeRegular* m_pPrevious;
		CTreeRegular* m_pNext;

		//Information about children (subtree with this node as its root)
		CTreeRegular* m_pChildFirst;
		CTreeRegular* m_pChildLast;
	};
};