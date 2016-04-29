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

	class OBJECT_EXTENSIONS_API IStreamBase
	{
	public:
		virtual void Close() = 0;
		virtual bool IsOpen() _GET = 0;
	};

	class OBJECT_EXTENSIONS_API CStream : public CObject, public virtual IStreamBase 
	{
	public:
		CStream();
		virtual ~CStream();

	protected:
		BETAONLY(bool m_bLocked;)
 	};
};