#pragma once

#include "WindowAbstract.h"

#include <SCFXML.h>
#include <SCFObjectExtensions.h>

using namespace SCFXML;
using namespace SCFBase;

namespace SCFUI
{
	class SCFUI_API CModule
	{
	public:
		CModule();
		~CModule();

		 bool BuildFrom(_IN LPTSTR szFilePath);

	private:
		
		void RepportAppend(_IN LPTSTR szText);

		bool ConstructDialog     (_IN CXMLElement& rElement);
		bool ConstructButtonClick(_IN CXMLElement& rElement, _INOUT CWindowAbstract* pParent);

		CXMLDocumentFile* m_pSource;

		#pragma warning(disable:4251)
		CDictionaryString<CWindowAbstract> m_Dictionary;
		#pragma warning(default:4251)

		LPTSTR m_szReport;
	};
};