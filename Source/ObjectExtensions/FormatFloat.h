#pragma once
#include "Format.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFormatFloat : public CFormat
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassFormatFloat; }
		CString   ToString() _GET { return STRING("{FormatFloat}"); }

	public:
		CFormatFloat();
		CFormatFloat(_IN SCF::UINT uiFractionDigitsMin, _IN SCF::UINT uiFractionDigitsMax, _IN SCF::ENUM eDecimalSeparator);

		virtual ~CFormatFloat();

	public:
		void      FractionDigitsMin(_IN SCF::UINT uiMin) _SET { m_uiFractionDigitsMin = uiMin; }
		SCF::UINT FractionDigitsMin()                    _GET { return m_uiFractionDigitsMin; }
	
		void      FractionDigitsMax(_IN SCF::UINT uiMax) _SET { m_uiFractionDigitsMax = uiMax; }
		SCF::UINT FractionDigitsMax()                    _GET { return m_uiFractionDigitsMax; }

	public:
		void      DecimalSeparator(_IN SCF::ENUM eSeparator) _SET { m_usDecimalChar = (SCF::TCHAR)eSeparator; }
		SCF::ENUM DecimalSeparator()                         _GET { return (SCF::ENUM)m_usDecimalChar; }

	protected:
		SCF::UINT m_uiFractionDigitsMin;
		SCF::UINT m_uiFractionDigitsMax;

		SCF::TCHAR m_usDecimalChar;
	};
};
