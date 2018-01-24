#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	class CSetNodeObject
	{
	private:
		CSetNodeObject();
		~CSetNodeObject();

	public:
		//Used instead of constructors 
		static CSetNodeObject* Create(_IN CObject& rObject);

		static void Delete          (_IN CSetNodeObject* pNode);
		static void DeleteWithObject(_IN CSetNodeObject* pNode);

	public:
		inline CObject& Object()                          _GET { return *m_pObject; }
		inline void     Object(_IN _REF CObject& rObject) _SET { RELEASE(*m_pObject); m_pObject = &(CObject&)rObject; ADDREF(*m_pObject); }

	public:
		inline CSetNodeObject* Parent()                                 _GET { return m_pParent; }
		inline void            Parent(_IN _REF CSetNodeObject* pParent) _SET { m_pParent = (CSetNodeObject*)pParent; }

		inline CSetNodeObject* ChildLeft()                                    _GET { return m_pChildLeft; }
		inline void            ChildLeft(_IN _REF CSetNodeObject* pChildLeft) _SET { m_pChildLeft = (CSetNodeObject*)pChildLeft; }

		inline CSetNodeObject* ChildRight()                                     _GET { return m_pChildRight; }
		inline void            ChildRight(_IN _REF CSetNodeObject* pChildRight) _SET { m_pChildRight = (CSetNodeObject*)pChildRight; }

		inline UINT Level()                 _GET { return m_uiLevel; }
		inline void Level(_IN UINT uiLevel) _SET { m_uiLevel = uiLevel; }

	public:
		inline bool IsRoot() { return (m_pParent == nullptr); }
		inline bool IsLeaf() { return ((m_pChildLeft == nullptr) && (m_pChildRight == nullptr)); }

	public:
		void Rebalance();
		void Remove();

		//Removes the node if it is the root & returns the new root..
		CSetNodeObject* RemoveRoot();

	protected:
		void Skew();
		bool Split();

	public:
		CSetNodeObject* LeafSmallest();
		CSetNodeObject* LeafBiggest();

	private:
		CObject* m_pObject;

	private:
		CSetNodeObject* m_pParent;

		CSetNodeObject* m_pChildLeft;
		CSetNodeObject* m_pChildRight;

		UINT m_uiLevel;
	};
};