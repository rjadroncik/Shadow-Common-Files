#include "StreamBuffered.h"
#include "Memory.h"

#include <SCFMathematicsTypes.h>

#include <malloc.h>

using namespace SCFBase;

CStreamBuffered::CStreamBuffered(_IN SCF::UINT uiBufferSize)
{
	m_bpBuffer     = NULL;
	m_uiBufferUsed = 0;

	this->BufferSize(uiBufferSize);
}

CStreamBuffered::~CStreamBuffered()
{
	if (m_bpBuffer) { free(m_bpBuffer); }
}

void CStreamBuffered::BufferSize(_IN SCF::UINT uiBufferSize)
{
	m_uiBufferSize = uiBufferSize;

	if (m_bpBuffer) { m_bpBuffer = (SCF::BYTE*)realloc(m_bpBuffer, m_uiBufferSize); }
	else            { m_bpBuffer = (SCF::BYTE*)malloc (            m_uiBufferSize); }
}

SCF::UINT CStreamBuffered::BufferedRead(_OUT void* vpBytes, _IN SCF::UINT uiCount)
{
	SCF::UINT uiBytesLeft = uiCount;
	SCF::UINT uiBytesRead = 0;

	//While we have something to read
	while (uiBytesLeft)
	{
		if (!m_uiBufferUsed)
		{
			//Fill the buffer by reading from the source - disk/device/etc..
			m_uiBufferUsed = this->BufferFill();

			if (!m_uiBufferUsed) { return uiBytesRead; }

			if (m_uiBufferUsed < m_uiBufferSize) { this->BufferSize(m_uiBufferUsed); }
		}

		//Copy contents of the buffer to the output buffer
		SCF::UINT uiBytesStored = __min(uiBytesLeft, m_uiBufferUsed);
		CMemory::Copy(&((SCF::BYTE*)vpBytes)[uiCount - uiBytesLeft], &m_bpBuffer[m_uiBufferSize - m_uiBufferUsed], uiBytesStored);

		//Reduce bytes left & bytes used _IN the buffer by the number of bytes read
		m_uiBufferUsed -= uiBytesStored;
		uiBytesLeft    -= uiBytesStored;
		uiBytesRead    += uiBytesStored;
	}

	return uiBytesRead;
}

SCF::UINT CStreamBuffered::BufferedWrite(_IN void* vpBytes, _IN SCF::UINT uiCount)
{
	SCF::UINT uiBytesLeft    = uiCount;
	SCF::UINT uiBytesWritten = 0;

	//While we have something to write
	while (uiBytesLeft)
	{
		//If the free space _IN the buffer isn't enough to store all the incoming data
		if ((uiBytesLeft + m_uiBufferUsed) > m_uiBufferSize)
		{
			//Commit the buffer to disk/device/etc..
			this->BufferCommit();

			//Fill the buffer with the next part of the incoming data
			m_uiBufferUsed = __min(uiBytesLeft, m_uiBufferSize);
			CMemory::Copy(m_bpBuffer, &((SCF::BYTE*)vpBytes)[uiCount - uiBytesLeft], m_uiBufferUsed);

			//Reduce bytes left by the number of bytes written
			uiBytesWritten += m_uiBufferUsed;
			uiBytesLeft    -= m_uiBufferUsed;
		}
		else
		{
			//Fill the buffer with the last/only part of the incoming data
			CMemory::Copy(&m_bpBuffer[m_uiBufferUsed], &((SCF::BYTE*)vpBytes)[uiCount - uiBytesLeft], uiBytesLeft);
			m_uiBufferUsed += uiBytesLeft;

			//Finished writing
			uiBytesWritten += uiBytesLeft;
			uiBytesLeft     = 0;
		}
	}

	return uiBytesWritten;
}