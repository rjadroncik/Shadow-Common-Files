#pragma once

#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CTreeSimpleRaw : public CObject
	{
	public:
		CString ToString() _GET { return STRING("{TreeSimple}"); }

	public:
		//Destructor DOES NOT delete the value, it just releases it! (can override this behavior)
		CTreeSimpleRaw();
		CTreeSimpleRaw(_IN _REF CObject& rValue);
		virtual ~CTreeSimpleRaw();

	public:
		//Destroys the tree along with ALL the values it holds
		void DeleteWithValues();

	protected:
		///////////////////////// Miscellaneous ///////////////////////////

		//Replaces the current node with a different one within a tree hierarchy, DOES NOT delete the one being replaced!
		void ReplaceWith(_INOUT CTreeSimpleRaw& rNode);
	
	public:
		void DeleteSiblings();

		//Leaf related functions
		inline bool IsLeaf() _GET { return (m_pChildFirst == nullptr) ? (1) : (0); }
		UINT Level(_IN UINT uiLevelsTraversed = 0) _GET;

	protected:
		///////////////////////// Relatives ///////////////////////////

		inline CTreeSimpleRaw* Parent()                          _GET { return m_pParent; }
		inline void            Parent(_IN CTreeSimpleRaw* pNode) _SET { m_pParent = (CTreeSimpleRaw*)pNode; }

		inline CTreeSimpleRaw* Previous()                          _GET { return m_pPrevious; }
		inline void            Previous(_IN CTreeSimpleRaw* pNode) _SET { m_pPrevious = (CTreeSimpleRaw*)pNode; }

		inline CTreeSimpleRaw* Next()                          _GET { return m_pNext; }
		inline void            Next(_IN CTreeSimpleRaw* pNode) _SET { m_pNext = (CTreeSimpleRaw*)pNode; }

	protected:
		///////////////////////// Children ///////////////////////////

		inline CTreeSimpleRaw* ChildFirst()                          _GET { return m_pChildFirst; }
		inline void            ChildFirst(_IN CTreeSimpleRaw* pNode) _SET { m_pChildFirst = (CTreeSimpleRaw*)pNode; }

		inline CTreeSimpleRaw* ChildLast()                          _GET { return m_pChildLast; }
		inline void            ChildLast(_IN CTreeSimpleRaw* pNode) _SET { m_pChildLast = (CTreeSimpleRaw*)pNode; }

		//Add or removes a node from the child list, DOES NOT create or delete the node!
		void ChildAdd     (_INOUT _REF CTreeSimpleRaw& rChild);
		void ChildAddFirst(_INOUT _REF CTreeSimpleRaw& rChild);
		void ChildRemove  (_INOUT      CTreeSimpleRaw& rChild);

	public:
		void ChildReplace(_INOUT CTreeSimpleRaw& rChildOld, _INOUT CTreeSimpleRaw& rChildNew);

		//Add a node at a specific position in the list
		//void ChildInsertBefore(_IN CTreeSimple* pChildNew, _IN CTreeSimple* pChildNext); 
		//void ChildInsertAfter (_IN CTreeSimple* pChildNew, _IN CTreeSimple* pChildPrevious); 

	protected:
		///////////////////////// Value ///////////////////////////

		inline bool     ValueHas()                      _GET { return (m_pValue == nullptr) ? (0) : (1); }
		inline CObject* Value()                         _GET { return m_pValue; }
		inline void     Value(_IN _REF CObject* pValue) _SET { BETAONLY(if (m_pValue) { m_pValue->Release(); }) m_pValue = (CObject*)pValue; BETAONLY(if (m_pValue) { m_pValue->AddRef(); }) }

		inline const CString ValueString() _GET { if (m_pValue) { return m_pValue->ToString(); } else { return CString(); } }

	protected:
		//Basic data
		CObject* m_pValue;

		//Information about relatives
		CTreeSimpleRaw* m_pParent;
		CTreeSimpleRaw* m_pPrevious;
		CTreeSimpleRaw* m_pNext;

		//Information about children (subtree with this node as its root)
		CTreeSimpleRaw* m_pChildFirst;
		CTreeSimpleRaw* m_pChildLast;
	};
};