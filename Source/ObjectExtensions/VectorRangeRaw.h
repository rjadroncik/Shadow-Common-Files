#pragma once
#include "Vector.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CVectorRangeRaw : public CVectorRaw
	{
	public:
		CVectorRangeRaw(_IN CVectorRangeRaw& rRange);
		CVectorRangeRaw(_IN _REF CVectorRaw& rVector, _IN SCF::UINT uiStart);
		CVectorRangeRaw(_IN _REF CVectorRaw& rVector, _IN SCF::UINT uiStart, _IN SCF::UINT uiSize);
		virtual ~CVectorRangeRaw();

	public:
		bool IsRange() _GET { return TRUE; }

	public:
		//These are special purpose - speed oriented functions - use them with care & proper understanding
		void ChangeStart(_IN SCF::UINT uiStart) _SET;
		void ChangeSize (_IN SCF::UINT uiSize)  _SET;

	public:
		inline CVectorRaw& Parent() _GET { return *m_pParent; }

	protected:
		CVectorRangeRaw() {}

	protected:
		CVectorRaw* m_pParent;
	};
};
