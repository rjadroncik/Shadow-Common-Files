#pragma once

#ifdef PICTURE_EXPORTS
#define PICTURE_API __declspec(dllexport)
#else
#define PICTURE_API __declspec(dllimport)
#endif

#include "Constants.h"
#include <SCFObjectExtensions.h>

namespace SCFImaging
{
	class PICTURE_API CImage : public CObject
	{
	public:
		static CImage* Load(_IN CString& rFilePath,      _IN SCF::DWORD dwOptions);
		static CImage* Load(_IN CString& rFileExtension, _IN SCF::DWORD dwOptions, _INOUT IStreamRead& rStream);

		static CImage* Compose(_IN CString& rFileRGBPath, _IN SCF::DWORD dwRGBOptions, _IN CString& rFileAlphaPath, _IN SCF::DWORD dwAlphaOptions);

	public:
		SCF::ENUM ClassKey() _GET { return ClassImage; }

	public:
		CImage();
		virtual ~CImage();

	public:
		void*      Data()     _GET { return m_Data.Value(); }
		SCF::UINT  DataSize() _GET { return m_Data.Size(); }

	public:
		SCF::UINT Width()  _GET { return m_uiWidth; }
		SCF::UINT Height() _GET { return m_uiHeight; }

		SCF::UINT Stride() _GET { return m_uiChannels * m_uiWidth; }

		SCF::ENUM GLFormat() _GET { return m_eGLFormat; }
		SCF::UINT Channels() _GET { return m_uiChannels; }
		SCF::UINT Bits()     _GET { return m_uiBits; }

		SCF::ENUM Compression() _GET { return m_eCompression; }

	protected:
		SCF::UINT m_uiWidth;				
		SCF::UINT m_uiHeight;				

	protected:
		SCF::ENUM m_eCompression; //The compression (RLE, JPEG, none)
		SCF::ENUM m_eGLFormat;    //Color format stored as one of the OpenGL enumeration values		

	protected:
		SCF::UINT m_uiChannels;    //(1 = Alpha, 3 = RGB/BGR, 4 = RGBA/BGRA)
		SCF::UINT m_uiBits;        //(8, 16, 24, 32)
		SCF::UINT m_uiMipmapCount; //The number of mip-map images stored

	protected:
		//The image data storage, the size of this storage can be calculated as follows: (stride * height), for mip-mapped images this is the size of the image without the mip-map data
		CMemoryBlock m_Data;
	};
};

