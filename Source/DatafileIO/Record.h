#pragma once
#include <SCFObjectExtensions.h>
#include "Classes.h"

using namespace SCFBase;

namespace SCFDatafileIOPrivate
{
	class CRecord : public CObjectSerializable
	{
	public:
		CRecord();
		virtual ~CRecord();
		
	public:
		bool Encrypted()                    _GET;
		bool Encrypted(_IN bool bEncrypted) _SET;

	protected:
		//When setting attributes, only this variable is modified & the appropriate changes
		//are done during the next save operation
		BYTE m_ucAttributes;
	};
};
