#pragma once

#include "PathStep.h"

namespace SCFXMLPrivate
{
	class CXMLPathStepNamed : public CXMLPathStep
	{
	public:
		inline void          QName(_IN CString& rQName) _SET { m_QName = rQName; }
		inline const CString QName()                    _GET { return STRINGREF(m_QName); }

	protected:
		CString m_QName;
	};
};
