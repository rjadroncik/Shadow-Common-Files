#pragma once
#include "Stream.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamBuffered : public CStream
	{
	public:
		CStreamBuffered(_IN SCF::UINT uiBufferSize);
		virtual ~CStreamBuffered();

	public:
		SCF::UINT BufferSize() { return m_uiBufferSize; }
		void BufferSize(_IN SCF::UINT uiBufferSize);

	protected:
		SCF::UINT BufferedRead (_OUT void* vpBytes, _IN SCF::UINT uiCount);
		SCF::UINT BufferedWrite(_IN  void* vpBytes, _IN SCF::UINT uiCount);

		virtual SCF::UINT BufferFill()   = 0;
		virtual SCF::UINT BufferCommit() = 0;

	protected:
		SCF::UINT BufferUsed()                 { return m_uiBufferUsed; }
		void BufferUsed(_IN SCF::UINT uiBytes) { m_uiBufferUsed = uiBytes; }

		SCF::BYTE* Buffer() { return m_bpBuffer; }

	private:
		SCF::BYTE* m_bpBuffer;
		SCF::UINT  m_uiBufferSize;
		SCF::UINT  m_uiBufferUsed;
	};
};