#pragma once
#include "Format.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFormatInt : public CFormat
	{
	public:
		CString ToString() _GET { return STRING("{FormatInt}"); }

	public:
		CFormatInt();
		CFormatInt(_IN UINT uiDigitsMin, _IN BYTE ucBase = 10, _IN bool bBasePrefix = TRUE);

		virtual ~CFormatInt();

	public:
		void      DigitsMin(_IN UINT uiMin) _SET { m_uiDigitsMin = uiMin; }
		UINT DigitsMin()                    _GET { return m_uiDigitsMin; }

	public:
		void      Base(_IN BYTE ucBase) _SET { m_ucBase = ucBase; }
		BYTE Base()                     _GET { return m_ucBase; }

	public:
		void BasePrefix(_IN bool bBasePrefix) _SET { m_bBasePrefix = bBasePrefix; }
		bool BasePrefix()                     _GET { return m_bBasePrefix; }

	protected:
		UINT m_uiDigitsMin;
		BYTE m_ucBase;

		bool m_bBasePrefix;
	};
};
