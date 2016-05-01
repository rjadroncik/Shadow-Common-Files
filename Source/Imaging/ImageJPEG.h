#pragma once

#include "Image.h"

////////////////////////////////////////////////////////////////////////////////
//
//  CImageJPEG - class for loading 24 bit baseline compressed (*.jpg) files
//
////////////////////////////////////////////////////////////////////////////////

namespace SCFImagingPrivate
{
	struct SDecodeData;
	struct SHuffmanTable;
};

namespace SCFImaging
{
	class PICTURE_API CImageJPEG : public CImage
	{
	public:
		ENUM ClassKey() _GET { return ClassImageJPEG; }

	public:
		CImageJPEG(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		~CImageJPEG();

	private:
		int  ReadMarkers(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);

		//Decompress jpeg file (Baseline algorithm)
		void Decompress (_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);

	private:
		//Convert YCbCr image to RGB
		void YCbCrToRGB();
		//Convert gray-scale image to RGB
		void GrayToRGB(); 

	private:
		static BYTE ReadByte(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static WORD ReadWord(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static BYTE ReadBit (_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static UINT ReadBits(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData, _IN UINT uiCount); 

	private:
		static WORD HuffmanCode(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData, _IN SCFImagingPrivate::SHuffmanTable& rTable);
		
		//Convert bit code to int
		static int BitToInt(WORD bit, UINT i);

		//AA&N algorithm inverse DCT on a 8x8 pixel block
		static void InverseDCT(float *data);
	};
};