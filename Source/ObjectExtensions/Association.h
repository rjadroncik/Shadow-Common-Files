#pragma once
#include "Association.h"

namespace SCFBase
{
	template<class TKey, class TValue>
	class CAssociation : public CAssociationRaw
	{
	public:
		CAssociation(_IN CAssociation<TKey, TValue>& rAssociation) : CAssociationRaw(rAssociation) {}
		CAssociation(_IN _REF TKey& rKey, _IN _REF TValue& rValue) : CAssociationRaw(rKey, rValue) {}

		virtual ~CAssociation() {}

	public:
		TKey& Key()     _GET { return (TKey&)CAssociationRaw::Key(); }
		TValue& Value() _GET { return (TValue&)CAssociationRaw::Value(); }
		
		void Key  (_IN _REF TKey&   rKey)   _SET { CAssociationRaw::Key(rKey); }
		void Value(_IN _REF TValue& rValue) _SET { CAssociationRaw::Value(rValue); }

	protected:
		CAssociation() {}
	};
};
