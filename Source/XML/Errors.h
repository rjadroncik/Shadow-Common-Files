#pragma once

#ifdef XML_EXPORTS
#define XML_API __declspec(dllexport)
#else
#define XML_API __declspec(dllimport)
#endif

namespace SCFXML
{
	enum Errors
	{
		//Basic values
		ErrorXMLStreamFailedWrite = 0x0101,
		ErrorXMLStreamFailedRead,
	};
};
