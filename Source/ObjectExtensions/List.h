#pragma once

#include "ListRaw.h"
#include "EnumeratorList.h"

namespace SCFBase
{
	template<class TValue> 
	class CList : public CListRaw, public IContainer<TValue>
	{
		//friend class OBJECT_EXTENSIONS_API CEnumeratorList;

	public:
		inline CList() {}
		inline virtual ~CList() {}
		
	public:
		UINT Size()    _GET { return m_uiCount; }
		bool IsEmpty() _GET { return (m_uiCount == 0); }

		inline IEnumerator<TValue>& NewEnumerator() _GET { return *(new CEnumeratorList<TValue>(*this)); }

	public:
		inline TValue& At         (_IN UINT uiIndex) _GET { return (TValue&)CListRaw::At(uiIndex); }
		inline TValue& operator [](_IN UINT uiIndex) _GET { return (TValue&)CListRaw::operator [](uiIndex); }

		inline void AtPut(_IN UINT uiIndex, _IN _REF TValue& rObject) _SET { CListRaw::AtPut(uiIndex, rObject); }

	public:
		inline TValue& Last() _GET { return (TValue&)CListRaw::Last(); }

	public:
		inline void LastAdd(_IN _REF TValue& rObject) { CListRaw::LastAdd(rObject); }

	public:
		inline void FirstAdd(_IN _REF TValue& rObject) { CListRaw::FirstAdd(rObject); }

	public:
		inline void Insert(_IN UINT uiIndex, _IN _REF TValue& rObject) { CListRaw::Insert(uiIndex, rObject); }

	public:
		inline void Remove  (_IN TValue& rObject)      { CListRaw::Remove(rObject); }
		inline bool Contains(_IN TValue& rObject) _GET { return CListRaw::Contains(rObject); }
	};
};
