#pragma once
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStringRange : public CString
	{
	public:
		CStringRange(_IN CStringRange& rRange);
		CStringRange(_IN _REF CString& rString, _IN UINT uiStart);
		CStringRange(_IN _REF CString& rString, _IN UINT uiStart, _IN UINT uiLength);
		CStringRange(_IN _REF CString& rString, _IN CString& rWhitespaceCharacters = STRING(WHITESPACE_CHARACTERS), _IN bool bTrimAtBeginning = true, _IN bool bTrimAtEnd = true);
		virtual ~CStringRange();

	public:
		//Assignment operator
		void operator =(_IN CString&      rString);
		void operator =(_IN CStringRange& rRange);

	public:
		bool IsRange() _GET { return true; }

	public:
		UINT Start() _GET { return m_szValue - m_pParent->m_szValue; }

	public:
		//These are special purpose - speed oriented functions - use them with care & proper understanding
		void ChangeStart (_IN UINT uiStart)  _SET;
		void ChangeLength(_IN UINT uiLength) _SET;
		
		void Rebind(_IN _REF CString& rString) _SET;

	public:
		inline CString& Parent() _GET { return *m_pParent; }

	protected:
		CStringRange() {}

	protected:
		CString* m_pParent;
	};
};
