#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	class CDictionaryNodeObject
	{
	private:
		CDictionaryNodeObject();
		~CDictionaryNodeObject();

	public:
		//Used instead of constructors 
		static CDictionaryNodeObject* Create(_IN CObject& rKey, _IN CObject& rObject);
		
		static void Delete                (_IN CDictionaryNodeObject* pNode);
		static void DeleteWithObject      (_IN CDictionaryNodeObject* pNode);
		static void DeleteWithObjectAndKey(_IN CDictionaryNodeObject* pNode);
		static void DeleteWithKey         (_IN CDictionaryNodeObject* pNode);

	public:
		inline CObject& Key()                  _GET { return *m_pKey; }
		inline void     Key(_IN CObject& rKey) _SET { RELEASE(*m_pKey); m_pKey = &(CObject&)rKey; ADDREF(*m_pKey); }

		inline CObject* Object()                          _GET { return m_pObject; }
		inline void     Object(_IN _REF CObject* rObject) _SET { BETAONLY(if (m_pObject) { m_pObject->Release(); }) m_pObject = (CObject*)rObject; BETAONLY(if (m_pObject) { m_pObject->AddRef(); }) }

	public:
		inline CDictionaryNodeObject* Parent()                                        _GET { return m_pParent; }
		inline void                   Parent(_IN _REF CDictionaryNodeObject* pParent) _SET { m_pParent = (CDictionaryNodeObject*)pParent; }

		inline CDictionaryNodeObject* ChildLeft()                                           _GET { return m_pChildLeft; }
		inline void                   ChildLeft(_IN _REF CDictionaryNodeObject* pChildLeft) _SET { m_pChildLeft = (CDictionaryNodeObject*)pChildLeft; }

		inline CDictionaryNodeObject* ChildRight()                                            _GET { return m_pChildRight; }
		inline void                   ChildRight(_IN _REF CDictionaryNodeObject* pChildRight) _SET { m_pChildRight = (CDictionaryNodeObject*)pChildRight; }

		inline UINT Level()                 _GET { return m_uiLevel; }
		inline void Level(_IN UINT uiLevel) _SET { m_uiLevel = uiLevel; }

	public:
		inline bool IsRoot() { return (m_pParent == nullptr); }
		inline bool IsLeaf() { return ((m_pChildLeft == nullptr) && (m_pChildRight == nullptr)); }

	public:
		void Rebalance();
		void Remove();

		//Removes the node if it is the root & returns the new root..
		CDictionaryNodeObject* RemoveRoot();

	protected:
		void Skew();
	    bool Split();

	public:
		CDictionaryNodeObject* LeafSmallest();
		CDictionaryNodeObject* LeafBiggest();

	private:
		CObject* m_pKey;
		CObject* m_pObject;

	private:
		CDictionaryNodeObject* m_pParent;

		CDictionaryNodeObject* m_pChildLeft;
		CDictionaryNodeObject* m_pChildRight;

		UINT m_uiLevel;
	};
};