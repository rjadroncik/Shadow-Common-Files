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
		SCF::ENUM ClassKey() _GET { return ClassImageJPEG; }

	public:
		CImageJPEG(_INOUT IStreamRead& rReadStream, _IN SCF::DWORD dwOptions);
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
		static SCF::BYTE ReadByte(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static SCF::WORD ReadWord(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static SCF::BYTE ReadBit (_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData);
		static SCF::UINT ReadBits(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData, _IN SCF::UINT uiCount); 

	private:
		static SCF::WORD HuffmanCode(_INOUT IStreamRead& rStream, _INOUT SCFImagingPrivate::SDecodeData& rData, _IN SCFImagingPrivate::SHuffmanTable& rTable);
		
		//Convert bit code to int
		static int BitToInt(SCF::WORD bit, SCF::UINT i);

		//AA&N algorithm inverse DCT on a 8x8 pixel block
		static void InverseDCT(float *data);
	};
};