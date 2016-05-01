#pragma once
#include "Stream.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamBuffered : public CStream
	{
	public:
		CStreamBuffered(_IN UINT uiBufferSize);
		virtual ~CStreamBuffered();

	public:
		UINT BufferSize() { return m_uiBufferSize; }
		void BufferSize(_IN UINT uiBufferSize);

	protected:
		UINT BufferedRead (_OUT void* vpBytes, _IN UINT uiCount);
		UINT BufferedWrite(_IN  void* vpBytes, _IN UINT uiCount);

		virtual UINT BufferFill()   = 0;
		virtual UINT BufferCommit() = 0;

	protected:
		UINT BufferUsed()                 { return m_uiBufferUsed; }
		void BufferUsed(_IN UINT uiBytes) { m_uiBufferUsed = uiBytes; }

		BYTE* Buffer() { return m_bpBuffer; }

	private:
		BYTE* m_bpBuffer;
		UINT  m_uiBufferSize;
		UINT  m_uiBufferUsed;
	};
};