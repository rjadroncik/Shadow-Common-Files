#pragma once
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStringRange : public CString
	{
	public:
		CStringRange(_IN CStringRange& rRange);
		CStringRange(_IN _REF CString& rString, _IN SCF::UINT uiStart);
		CStringRange(_IN _REF CString& rString, _IN SCF::UINT uiStart, _IN SCF::UINT uiLength);
		CStringRange(_IN _REF CString& rString, _IN CString& rWhitespaceCharacters = STRING(WHITESPACE_CHARACTERS), _IN bool bTrimAtBeginning = TRUE, _IN bool bTrimAtEnd = TRUE);
		virtual ~CStringRange();

	public:
		//Assignment operator
		void operator =(_IN CString&      rString);
		void operator =(_IN CStringRange& rRange);

	public:
		bool IsRange() _GET { return TRUE; }

	public:
		SCF::UINT Start() _GET { return m_szValue - m_pParent->m_szValue; }

	public:
		//These are special purpose - speed oriented functions - use them with care & proper understanding
		void ChangeStart (_IN SCF::UINT uiStart)  _SET;
		void ChangeLength(_IN SCF::UINT uiLength) _SET;
		
		void Rebind(_IN _REF CString& rString) _SET;

	public:
		inline CString& Parent() _GET { return *m_pParent; }

	protected:
		CStringRange() {}

	protected:
		CString* m_pParent;
	};
};
