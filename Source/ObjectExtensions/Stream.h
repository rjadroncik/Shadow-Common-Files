#pragma once

#include "Object.h"

namespace SCFBase
{
	enum TextEncodings
	{
		EncodingUCS2,
		EncodingASCII,
		EncodingUTF8,
	};

	class OBJECT_EXTENSIONS_API IStream
	{
	public:
		virtual void Close() = 0;
		virtual bool IsOpen() _GET = 0;

	protected:
		virtual ~IStream() {}
	};

	class OBJECT_EXTENSIONS_API CStream : public CObject, public virtual IStream 
	{
	public:
		CStream();
		virtual ~CStream();

	protected:
		BETAONLY(bool m_bLocked;)
 	};
};