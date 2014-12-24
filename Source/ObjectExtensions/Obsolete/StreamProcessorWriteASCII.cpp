#include "StreamProcessorWriteASCII.h"

using namespace SCFObjectExtensions;

CStreamProcessorWriteASCII::CStreamProcessorWriteASCII()  {}
CStreamProcessorWriteASCII::~CStreamProcessorWriteASCII() {}

void CStreamProcessorWriteASCII::Process(_IN void* vpData, _IN SCF::UINT uiBytes, _OUT IStreamWrite& rStream)
{
	rStream.Processor(NULL);

	for (SCF::UINT i = 0; i < (uiBytes / 2); i++)
	{
		rStream.PutByte((SCF::BYTE)(((SCF::TCHAR*)vpData)[i]));
	}

	rStream.Processor(this);
}
