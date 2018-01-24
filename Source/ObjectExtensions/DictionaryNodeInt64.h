#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	class CDictionaryNodeInt64
	{
	private:
		CDictionaryNodeInt64();
		~CDictionaryNodeInt64();

	public:
		//Used instead of constructors 
		static CDictionaryNodeInt64* Create(_IN UINT64 ui64Key, _IN CObject& rObject);
		
		static void Delete          (_IN CDictionaryNodeInt64* pNode);
		static void DeleteWithObject(_IN CDictionaryNodeInt64* pNode);

	public:
		inline UINT64 Key()                   _GET { return m_ui64Key; }
		inline void   Key(_IN UINT64 ui64Key) _SET { m_ui64Key = ui64Key; }

		inline CObject* Object()                          _GET { return m_pObject; }
		inline void     Object(_IN _REF CObject* rObject) _SET { BETAONLY(if (m_pObject) { m_pObject->Release(); }) m_pObject = (CObject*)rObject; BETAONLY(if (m_pObject) { m_pObject->AddRef(); }) }

	public:
		inline CDictionaryNodeInt64* Parent()                                       _GET { return m_pParent; }
		inline void                  Parent(_IN _REF CDictionaryNodeInt64* pParent) _SET { m_pParent = (CDictionaryNodeInt64*)pParent; }

		inline CDictionaryNodeInt64* ChildLeft()                                          _GET { return m_pChildLeft; }
		inline void                  ChildLeft(_IN _REF CDictionaryNodeInt64* pChildLeft) _SET { m_pChildLeft = (CDictionaryNodeInt64*)pChildLeft; }

		inline CDictionaryNodeInt64* ChildRight()                                           _GET { return m_pChildRight; }
		inline void                  ChildRight(_IN _REF CDictionaryNodeInt64* pChildRight) _SET { m_pChildRight = (CDictionaryNodeInt64*)pChildRight; }

		inline UINT Level()                 _GET { return m_uiLevel; }
		inline void Level(_IN UINT uiLevel) _SET { m_uiLevel = uiLevel; }

	public:
		inline bool IsRoot() { return (m_pParent == nullptr); }
		inline bool IsLeaf() { return ((m_pChildLeft == nullptr) && (m_pChildRight == nullptr)); }

	public:
		void Rebalance();
		void Remove();

		//Removes the node if it is the root & returns the new root..
		CDictionaryNodeInt64* RemoveRoot();

	protected:
		void Skew();
	    bool Split();

	public:
		CDictionaryNodeInt64* LeafSmallest();
		CDictionaryNodeInt64* LeafBiggest();

	private:
		UINT64 m_ui64Key;
		CObject* m_pObject;

	private:
		CDictionaryNodeInt64* m_pParent;

		CDictionaryNodeInt64* m_pChildLeft;
		CDictionaryNodeInt64* m_pChildRight;

		UINT m_uiLevel;
	};
};