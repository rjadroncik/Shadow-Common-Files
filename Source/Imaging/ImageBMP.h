#pragma once

#include "Image.h"

////////////////////////////////////////////////////////////////////////////////
//
//  CImageBMP - class for loading 24 & 32 bit (*.bmp) files
//
////////////////////////////////////////////////////////////////////////////////

namespace SCFImaging
{
	class PICTURE_API CImageBMP : public CImage
	{
	public:
		ENUM ClassKey() _GET { return ClassImageBMP; }

	public:
		CImageBMP(_INOUT IStreamRead& rReadStream, _IN DWORD dwOptions);
		virtual ~CImageBMP();
	};
};