#pragma once

//Contains all constants, that the user/client of the dll/object could need to fully exploit its capabilities, internal constants are not listed here

namespace SCFImaging
{
	enum CompressionTypes
	{
		CT_NONE = 0,			

		CT_DXT1 = 1,	
		CT_DXT3 = 3,	
		CT_DXT5 = 5,	
	};

	enum ImageFormat
	{
		IF_ALPHA = 1,
		
		IF_RGB = 31,
		IF_BGR = 32,

		IF_RGBA = 41,
		IF_BGRA = 42,

		IF_DXT1_RGBA = 141,
		IF_DXT3_RGBA = 341,
		IF_DXT5_RGBA = 541,
	};
};