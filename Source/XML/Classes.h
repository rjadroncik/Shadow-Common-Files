#pragma once

#ifdef XML_EXPORTS
#define XML_API __declspec(dllexport)
#else
#define XML_API __declspec(dllimport)
#endif

namespace SCFXML
{
	//Reserved class keys: 0x0101 - 0x0150

	enum Classes
	{
		//Basic values
		ClassXMLAttribute = 0x0101, //name=value (only in elements <item name=value>) values can be quoted ('value') or double quoted ("value")
		ClassXMLElement,            //<item [atributes]></item> or </item [atributes]>
		ClassXMLComment,            //<!-- my comment --> 
		ClassXMLNotation,           //<!NOTATION...> 
		ClassXMLDeclaration,        //<?xml version='1.0'?>
		ClassXMLCData,              //<![CDATA[my escaped text]]>

		ClassXMLEnumerator,
		ClassXMLDocument,
		ClassXMLDocumentFile,

		ClassXMLReader,
		ClassXMLWriter,

		ClassXMLStreamRead,
		ClassXMLStreamWrite,

		ClassXMLStreamReadObject,
		ClassXMLStreamWriteObject,

		ClassXMLPath,

		ClassEnumeration,
	};
};
