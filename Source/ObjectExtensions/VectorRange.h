#pragma once
#include "VectorRangeRaw.h"

namespace SCFBase
{
	template<class TValue> 
	class CVectorRange : public CVectorRangeRaw
	{
	public:
		inline CVectorRange(_IN CVectorRange<TValue>& rRange) : CVectorRangeRaw(rRange) {}
		inline CVectorRange(_IN _REF CVector<TValue>& rVector, _IN SCF::UINT uiStart) : CVectorRangeRaw(rVector, uiStart) {}
		inline CVectorRange(_IN _REF CVector<TValue>& rVector, _IN SCF::UINT uiStart, _IN SCF::UINT uiSize) : CVectorRangeRaw(rVector, uiStart, uiSize) {}
		inline virtual ~CVectorRange() {}

	public:
		//These are special purpose - speed oriented functions - use them with care & proper understanding
		inline void ChangeStart(_IN SCF::UINT uiStart) _SET { CVectorRangeRaw::ChangeStart(uiStart); };
		inline void ChangeSize (_IN SCF::UINT uiSize)  _SET { CVectorRangeRaw::ChangeSize(uiSize); };

	public:
		inline CVector<TValue>& Parent() _GET { return (CVector<TValue>&)CVectorRangeRaw::Parent(); }

		//********************************* CVector ***********************************
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
