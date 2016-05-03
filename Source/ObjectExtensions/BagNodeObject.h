#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	class CBagNodeObject
	{
	private:
		CBagNodeObject();
		~CBagNodeObject();

	public:
		//Used instead of constructors 
		static CBagNodeObject* Create(_IN CObject& rObject);

		static void Delete          (_IN CBagNodeObject* pNode);
		static void DeleteWithObject(_IN CBagNodeObject* pNode);

	public:
		inline CObject& Object()                          _GET { return *m_pObject; }
		inline void     Object(_IN _REF CObject& rObject) _SET { RELEASE(*m_pObject); m_pObject = &(CObject&)rObject; ADDREF(*m_pObject); }

	public:
		inline CBagNodeObject* Parent()                                 _GET { return m_pParent; }
		inline void            Parent(_IN _REF CBagNodeObject* pParent) _SET { m_pParent = (CBagNodeObject*)pParent; }

		inline CBagNodeObject* ChildLeft()                                    _GET { return m_pChildLeft; }
		inline void            ChildLeft(_IN _REF CBagNodeObject* pChildLeft) _SET { m_pChildLeft = (CBagNodeObject*)pChildLeft; }

		inline CBagNodeObject* ChildRight()                                     _GET { return m_pChildRight; }
		inline void            ChildRight(_IN _REF CBagNodeObject* pChildRight) _SET { m_pChildRight = (CBagNodeObject*)pChildRight; }

		inline UINT Level()                 _GET { return m_uiLevel; }
		inline void Level(_IN UINT uiLevel) _SET { m_uiLevel = uiLevel; }

	public:
		inline bool IsRoot() { return (m_pParent == NULL); }
		inline bool IsLeaf() { return ((m_pChildLeft == NULL) && (m_pChildRight == NULL)); }

	public:
		void Rebalance();
		void Remove();

		//Removes the node if it is the root & returns the new root..
		CBagNodeObject* RemoveRoot();

	protected:
		void Skew();
		bool Split();

	public:
		CBagNodeObject* LeafSmallest();
		CBagNodeObject* LeafBiggest();

	private:
		CObject* m_pObject;

	private:
		CBagNodeObject* m_pParent;

		CBagNodeObject* m_pChildLeft;
		CBagNodeObject* m_pChildRight;

		UINT m_uiLevel;
	};
};