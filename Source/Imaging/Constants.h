#pragma once

//Reserved class keys: 0x0151 - 0x0200

//Contains all constants, that the user/client of the dll/object could need to fully exploit its capabilities, internal constants are not listed here

namespace SCFImaging
{
	enum Classes
	{
		ClassImageJPEG = 0x0151,
		ClassImageTGA,
		ClassImageBMP,
		ClassImageDDS,
		ClassImage,
	};

	enum CompressionTypes
	{
		CT_NONE		= 0,			
		CT_RLE,			
		CT_JPEG,	

		CT_DXT1,	
		CT_DXT3,	
		CT_DXT5,	
	};
};