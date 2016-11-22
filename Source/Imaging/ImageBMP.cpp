#include <SCFStandard.h>

#include <SCFWinAPI.h>

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN		
#include <windows.h>

#include "ImageBMP.h"

using namespace SCFImaging;

CImageBMP::CImageBMP(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions)
{
	SCF_UNREFERENCED_PARAMETER(dwOptions);

	BITMAPFILEHEADER BmpFileHeader;
	rReadStream.GetBytes(&BmpFileHeader, sizeof(BmpFileHeader));

	//Check the type field to make sure we have a (*.bmp) file
	if (memcmp(&BmpFileHeader.bfType, "BM", 2)) { return; }

	BITMAPINFOHEADER BmpInfoheader;
	rReadStream.GetBytes(&BmpInfoheader, sizeof(BmpInfoheader));

	//We only support 24-bit and 32-bit files
	if ((BmpInfoheader.biBitCount != 24) && (BmpInfoheader.biBitCount != 32)) { return; }

	m_uiHeight = BmpInfoheader.biHeight;
	m_uiWidth  = BmpInfoheader.biWidth;

	m_uiBits     = BmpInfoheader.biBitCount;
	m_uiChannels = BmpInfoheader.biBitCount / 8;

	if (m_uiChannels == 3 ) { m_eFormat = IF_RGB; }
	if (m_uiChannels == 4 ) { m_eFormat = IF_RGBA; }

	//Allocate memory
	m_Data.Size(this->Stride() * m_uiHeight);

	//Find pixel data
	//rReadStream.SkipBytes(BmpFileHeader.bfOffBits - (UINT)rReadStream.BytesRead()); 

	UINT uiStride  = this->Stride();
	UINT uiPadding = (uiStride % 4) ? (4 - (uiStride % 4)) : (0);

	for (UINT i = 0; i < m_uiHeight; i++)
	{
		//Read one line at a time & skip padding
		rReadStream.GetBytes(&((BYTE*)m_Data.Value())[uiStride * i], uiStride);
		rReadStream.SkipBytes(uiPadding);
	}

	if (m_uiChannels == 1 ) { m_eFormat = IF_ALPHA; }
	if (m_uiChannels == 3 ) { m_eFormat = IF_BGR; }
	if (m_uiChannels == 4 ) { m_eFormat = IF_BGRA; }
}

CImageBMP::~CImageBMP() 
{
}