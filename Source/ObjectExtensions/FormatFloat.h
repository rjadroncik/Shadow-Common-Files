#pragma once
#include "Format.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFormatFloat : public CFormat
	{
	public:
		CString ToString() _GET { return STRING("{FormatFloat}"); }

	public:
		CFormatFloat();
		CFormatFloat(_IN UINT uiFractionDigitsMin, _IN UINT uiFractionDigitsMax, _IN ENUM eDecimalSeparator);

		virtual ~CFormatFloat();

	public:
		void      FractionDigitsMin(_IN UINT uiMin) _SET { m_uiFractionDigitsMin = uiMin; }
		UINT FractionDigitsMin()                    _GET { return m_uiFractionDigitsMin; }
	
		void      FractionDigitsMax(_IN UINT uiMax) _SET { m_uiFractionDigitsMax = uiMax; }
		UINT FractionDigitsMax()                    _GET { return m_uiFractionDigitsMax; }

	public:
		void      DecimalSeparator(_IN ENUM eSeparator) _SET { m_usDecimalChar = (TCHAR)eSeparator; }
		ENUM DecimalSeparator()                         _GET { return (ENUM)m_usDecimalChar; }

	protected:
		UINT m_uiFractionDigitsMin;
		UINT m_uiFractionDigitsMax;

		TCHAR m_usDecimalChar;
	};
};
