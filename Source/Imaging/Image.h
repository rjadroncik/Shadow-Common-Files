#pragma once

#ifdef IMAGING_EXPORTS
#define IMAGING_API __declspec(dllexport)
#else
#define IMAGING_API __declspec(dllimport)
#endif

#include "Constants.h"
#include <SCFObjectExtensions.h>

namespace SCFImaging
{
	class IMAGING_API CImage : public CObject
	{
	public:
		static CImage* Load(_IN CString& rFilePath,      _IN DWORD dwOptions);
		static CImage* Load(_IN CString& rFileExtension, _IN DWORD dwOptions, _INOUT IStreamRead& rStream);

		static CImage* Compose(_IN CString& rFileRGBPath, _IN DWORD dwRGBOptions, _IN CString& rFileAlphaPath, _IN DWORD dwAlphaOptions);

	public:
		CImage();
		virtual ~CImage();

	public:
		void* Data()     _GET { return m_Data.Value(); }
		UINT  DataSize() _GET { return m_Data.Size(); }

	public:
		UINT Width()  _GET { return m_uiWidth; }
		UINT Height() _GET { return m_uiHeight; }

		UINT Stride() _GET { return m_uiChannels * m_uiWidth; }

		ENUM Format()   _GET { return m_eFormat; }
		UINT Channels() _GET { return m_uiChannels; }
		UINT Bits()     _GET { return m_uiBits; }

		ENUM Compression() _GET { return m_eCompression; }

	protected:
		UINT m_uiWidth;				
		UINT m_uiHeight;				

	protected:
		ENUM m_eCompression; //The compression (DXT1, none)
		ENUM m_eFormat;      //Color format stored as one of the OpenGL enumeration values		

	protected:
		UINT m_uiChannels;    //(1 = Alpha, 3 = RGB/BGR, 4 = RGBA/BGRA)
		UINT m_uiBits;        //(8, 16, 24, 32)
		UINT m_uiMipmapCount; //The number of mip-map images stored

	protected:
		//The image data storage, the size of this storage can be calculated as follows: (stride * height), for mip-mapped images this is the size of the image without the mip-map data
		CMemoryBlock m_Data;
	};
};

