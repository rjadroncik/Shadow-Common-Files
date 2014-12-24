#pragma once
#include "Format.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CFormatInt : public CFormat
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassFormatInt; }
		CString   ToString() _GET { return STRING("{FormatInt}"); }

	public:
		CFormatInt();
		CFormatInt(_IN SCF::UINT uiDigitsMin, _IN SCF::BYTE ucBase = 10, _IN bool bBasePrefix = TRUE);

		virtual ~CFormatInt();

	public:
		void      DigitsMin(_IN SCF::UINT uiMin) _SET { m_uiDigitsMin = uiMin; }
		SCF::UINT DigitsMin()                    _GET { return m_uiDigitsMin; }

	public:
		void      Base(_IN SCF::BYTE ucBase) _SET { m_ucBase = ucBase; }
		SCF::BYTE Base()                     _GET { return m_ucBase; }

	public:
		void BasePrefix(_IN bool bBasePrefix) _SET { m_bBasePrefix = bBasePrefix; }
		bool BasePrefix()                     _GET { return m_bBasePrefix; }

	protected:
		SCF::UINT m_uiDigitsMin;
		SCF::BYTE m_ucBase;

		bool m_bBasePrefix;
	};
};
