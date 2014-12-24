#pragma once

#include "ListRaw.h"

namespace SCFBase
{
	template<class TValue> 
	class CList : public CListRaw
	{
		//friend class OBJECT_EXTENSIONS_API CEnumeratorList;

	//public:
	//	CEnumerator* EnumeratorNew() _GET;

	public:
		inline CList() {}
		inline virtual ~CList() {}

	public:
		inline TValue& At         (_IN SCF::UINT uiIndex) _GET { return (TValue&)CListRaw::At(uiIndex); }
		inline TValue& operator [](_IN SCF::UINT uiIndex) _GET { return (TValue&)CListRaw::operator [](uiIndex); }

		inline void AtPut(_IN SCF::UINT uiIndex, _IN _REF TValue& rObject) _SET { CListRaw::AtPut(uiIndex, rObject); }

	public:
		inline TValue& Last() _GET { return (TValue&)CListRaw::Last(); }

	public:
		inline void LastAdd(_IN _REF TValue& rObject) { CListRaw::LastAdd(rObject); }

	public:
		inline void FirstAdd(_IN _REF TValue& rObject) { CListRaw::FirstAdd(rObject); }

	public:
		inline void Insert(_IN SCF::UINT uiIndex, _IN _REF TValue& rObject) { CListRaw::Insert(uiIndex, rObject); }

	public:
		inline void Remove  (_IN TValue& rObject)      { CListRaw::Remove(rObject); }
		inline bool Contains(_IN TValue& rObject) _GET { return CListRaw::Contains(rObject); }
	};
};
