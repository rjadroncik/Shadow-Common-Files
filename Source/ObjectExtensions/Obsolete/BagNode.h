#pragma once

#include "FSBHeap.h"

using namespace SCFObjectExtensions;

namespace SCFPrivate
{
	class CBagNode
	{
	private:
		CBagNode();
		~CBagNode();

	public:
		//Used instead of constructors 
		static CBagNode* Create(_INOUT CFSBHeap& rHeap, _IN CObject& rObject);
		static void      Delete(_INOUT CFSBHeap& rHeap, _IN CBagNode* pNode);

	public:
		inline CObject* Object()                          _GET { return m_pObject; }
		inline void     Object(_IN _REF CObject* rObject) _SET { if (m_pObject) { m_pObject->Release(); } m_pObject = (CObject*)rObject; if (m_pObject) { m_pObject->AddRef(); } }

	public:
		inline CBagNode* Parent()                           _GET { return m_pParent; }
		inline void      Parent(_IN _REF CBagNode* pParent) _SET { m_pParent = (CBagNode*)pParent; }

		inline CBagNode* ChildLeft()                              _GET { return m_pChildLeft; }
		inline void      ChildLeft(_IN _REF CBagNode* pChildLeft) _SET { m_pChildLeft = (CBagNode*)pChildLeft; }

		inline CBagNode* ChildRight()                               _GET { return m_pChildRight; }
		inline void      ChildRight(_IN _REF CBagNode* pChildRight) _SET { m_pChildRight = (CBagNode*)pChildRight; }

		inline UINT Level()                 _GET { return m_uiLevel; }
		inline void Level(_IN UINT uiLevel) _SET { m_uiLevel = uiLevel; }

	public:
		inline bool IsRoot() { return (m_pParent == NULL); }
		inline bool IsLeaf() { return ((m_pChildLeft == NULL) && (m_pChildRight == NULL)); }

	public:
		void Rebalance();
		void Remove();

		//Removes the node if it is the root & returns the new root..
		CBagNode* RemoveRoot();

	protected:
		void Skew();
		bool Split();

	public:
		CBagNode* LeafSmallest();
		CBagNode* LeafBiggest();

	private:
		CObject* m_pObject;

	private:
		CBagNode* m_pParent;

		CBagNode* m_pChildLeft;
		CBagNode* m_pChildRight;

		UINT m_uiLevel;
	};
};