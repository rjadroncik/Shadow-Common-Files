#pragma once
#include "VectorRaw.h"

namespace SCFBase
{
	template<class TValue> 
	class CVector : public CVectorRaw
	{
	public:
		inline CVector() {}
		inline virtual ~CVector() {}

	public:
		inline TValue& At         (_IN SCF::UINT uiIndex) _GET { return (TValue&)CVectorRaw::At(uiIndex); }
		inline TValue& operator [](_IN SCF::UINT uiIndex) _GET { return (TValue&)CVectorRaw::operator[](uiIndex); }

		inline void AtPut(_IN SCF::UINT uiIndex, _IN _REF TValue& rObject) _SET { CVectorRaw::Atput(uiIndex, rObject); }

	public:
		inline TValue& Last() _GET { return (TValue&)CVectorRaw::Last(); }

	public:
		inline void LastAdd(_IN _REF TValue& rObject) { CVectorRaw::LastAdd(rObject); }

		inline void Remove(_IN TValue& rObject) { CVectorRaw::Remove(rObject); }
		inline void Delete(_IN TValue& rObject) { CVectorRaw::Delete(rObject); }

	public:
		inline void Insert(_IN SCF::UINT uiIndex, _IN TValue& rObject) { CVectorRaw::Insert(uiIndex, rObject); }

	public:
		//Find a given object in the vector based on an identity test
		inline int Find(_IN TValue& rObject) _GET { return CVectorRaw::Find(rObject); }

		//Find a given object in the  vector based on a custom equality test
		inline int FindEqual(_IN TValue& rObject, _IN CComparer& rComparer) _GET { return CVectorRaw::FindEqual(rObject, rComparer); }
	};
};
