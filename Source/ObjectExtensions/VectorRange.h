#pragma once
#include "Vector.h"

namespace SCFBase
{
	_PENDING; //Implementing serialization
	class OBJECT_EXTENSIONS_API CVectorRange : public CVectorRaw
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassVectorRange; }

	public:
		CVectorRange(_IN CVectorRange& rRange);
		CVectorRange(_IN _REF CVectorRaw& rVector, _IN SCF::UINT uiStart);
		CVectorRange(_IN _REF CVectorRaw& rVector, _IN SCF::UINT uiStart, _IN SCF::UINT uiSize);
		virtual ~CVectorRange();

	public:
		bool IsRange() _GET { return TRUE; }

	public:
		//These are special purpose - speed oriented functions - use them with care & proper understanding
		void ChangeStart(_IN SCF::UINT uiStart) _SET;
		void ChangeSize (_IN SCF::UINT uiSize)  _SET;

	public:
		inline CVectorRaw& Parent() _GET { return *m_pParent; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);
	
	protected:
		CVectorRange() {}

	protected:
		CVectorRaw* m_pParent;
	};
};
