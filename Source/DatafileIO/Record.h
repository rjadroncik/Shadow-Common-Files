#pragma once
#include <SCFObjectExtensions.h>
#include <SCFXML.h>
#include "Errors.h"

using namespace SCFBase;

namespace SCFDatafileIOPrivate
{
	enum Attributes
	{
		AttributeCompressed = 0x01,
		AttributeEncrypted = 0x02
	};

	enum RecordType
	{
		RecordFile = 1,
		RecordDirectory
	};

	class CRecord : public SCFXML::CXMLObjectSerializable
	{
	public:
		CRecord(RecordType type);
		virtual ~CRecord();
		
	public:
		bool Encrypted()                    _GET;
		bool Encrypted(_IN bool bEncrypted) _SET;

		RecordType Type() _GET { return m_eType; }

	protected:
		//When setting attributes, only this variable is modified & the appropriate changes
		//are done during the next save operation
		BYTE m_ucAttributes;

		RecordType m_eType;
	};
};
