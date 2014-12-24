#pragma once

#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CTreeSimple : public CObject
	{
	public:
		SCF::UINT ClassKey() _GET { return ClassTreeSimple; }
		CString   ToString() _GET { return STRING("{TreeSimple}"); }

		//Destructor DOES NOT delete the value, it just releases it! (can override this behavior)
		CTreeSimple();
		CTreeSimple(_IN _REF CObject& rObject);
		virtual ~CTreeSimple();

	public:
		//Destroys the tree along with ALL the values it holds
		void DeleteWithValues();

	public:
		///////////////////////// Miscellaneous ///////////////////////////

		//Replaces the current node with a different one within a tree hierarchy, DOES NOT delete the one being replaced!
		void ReplaceWith(_INOUT CTreeSimple& rNode);
		void DeleteSiblings();

		//Leaf related functions
		inline bool IsLeaf() _GET { return (m_pChildFirst == NULL) ? (1) : (0); }
		SCF::UINT Level(_IN SCF::UINT uiLevelsTraversed = 0) _GET;

	public:
		///////////////////////// Relatives ///////////////////////////

		inline CTreeSimple* Parent()                       _GET { return m_pParent; }
		inline void         Parent(_IN CTreeSimple* pNode) _SET { m_pParent = (CTreeSimple*)pNode; }

		inline CTreeSimple* Previous()                       _GET { return m_pPrevious; }
		inline void         Previous(_IN CTreeSimple* pNode) _SET { m_pPrevious = (CTreeSimple*)pNode; }

		inline CTreeSimple* Next()                       _GET { return m_pNext; }
		inline void         Next(_IN CTreeSimple* pNode) _SET { m_pNext = (CTreeSimple*)pNode; }

	public:
		///////////////////////// Children ///////////////////////////

		inline CTreeSimple* ChildFirst()                       _GET { return m_pChildFirst; }
		inline void         ChildFirst(_IN CTreeSimple* pNode) _SET { m_pChildFirst = (CTreeSimple*)pNode; }

		inline CTreeSimple* ChildLast()                       _GET { return m_pChildLast; }
		inline void         ChildLast(_IN CTreeSimple* pNode) _SET { m_pChildLast = (CTreeSimple*)pNode; }

		//Add or removes a node from the child list, DOES NOT create or delete the node!
		void ChildAdd     (_INOUT _REF CTreeSimple& rChild);
		void ChildAddFirst(_INOUT _REF CTreeSimple& rChild);
		void ChildRemove  (_INOUT      CTreeSimple& rChild);

	public:
		void ChildReplace(_INOUT CTreeSimple& rChildOld, _INOUT CTreeSimple& rChildNew);

		//Add a node at a specific position in the list
		//void ChildInsertBefore(_IN CTreeSimple* pChildNew, _IN CTreeSimple* pChildNext); 
		//void ChildInsertAfter (_IN CTreeSimple* pChildNew, _IN CTreeSimple* pChildPrevious); 

	public:
		///////////////////////// Value ///////////////////////////

		inline bool     ValueHas()                      _GET { return (m_pValue == NULL) ? (0) : (1); }
		inline CObject* Value()                         _GET { return m_pValue; }
		inline void     Value(_IN _REF CObject* pValue) _SET { BETAONLY(if (m_pValue) { m_pValue->Release(); }) m_pValue = (CObject*)pValue; BETAONLY(if (m_pValue) { m_pValue->AddRef(); }) }

		inline const CString ValueString() _GET { if (m_pValue) { return m_pValue->ToString(); } else { return CString(); } }

	protected:
		//Basic data
		CObject* m_pValue;

		//Information about relatives
		CTreeSimple* m_pParent;
		CTreeSimple* m_pPrevious;
		CTreeSimple* m_pNext;

		//Information about children (subtree with this node as its root)
		CTreeSimple* m_pChildFirst;
		CTreeSimple* m_pChildLast;
	};
};