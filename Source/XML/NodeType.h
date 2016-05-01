#pragma once

#ifdef XML_EXPORTS
#define XML_API __declspec(dllexport)
#else
#define XML_API __declspec(dllimport)
#endif

namespace SCFXML
{
	//Reserved class keys: 0x0101 - 0x0150

	enum NodeType
	{
		//Basic values
		XmlAttribute = 0x0101, //name=value (only in elements <item name=value>) values can be quoted ('value') or double quoted ("value")
		XmlElement,            //<item [atributes]></item> or </item [atributes]>
		XmlComment,            //<!-- my comment --> 
		XmlNotation,           //<!NOTATION...> 
		XmlDeclaration,        //<?xml version='1.0'?>
		XmlCData,              //<![CDATA[my escaped text]]>

		XmlDocument,
	};
};
