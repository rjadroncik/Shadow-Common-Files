#pragma once
#include "Classes.h"

namespace SCFGraphics
{
	//Color types, data alignment is most-to-least significant byte, from iX-to-iWidth (e.i.: when composing a [ColorRGBA] value the A (Alpha) component is not shifted)
	typedef DWORD ColorRGB;		
	typedef DWORD ColorRGBA;
	typedef DWORD ColorARGB;

	typedef DWORD ColorWinRGB; //Important!!! - ColorWinRGB relates to standard Windows RGB format which has a different memory layout - BGR instead of RGB

	//Color composition macros
	#define SCF_RGB(RedByte, GreenByte, BlueByte)             ((SCFGraphics::ColorRGB)  (((BYTE)(BlueByte)  | ((WORD)((BYTE)(GreenByte)) << 8)) | (((DWORD)(BYTE)(RedByte))   << 16))) 
	#define SCF_RGBA(RedByte, GreenByte, BlueByte, AlphaByte) ((SCFGraphics::ColorRGBA)((((BYTE)(AlphaByte) | ((WORD)((BYTE)(BlueByte))  << 8)) | (((DWORD)(BYTE)(GreenByte)) << 16))) | (((DWORD)(BYTE)(RedByte))   << 24)) 
	#define SCF_ARGB(AlphaByte, RedByte, GreenByte, BlueByte) ((SCFGraphics::ColorRGBA)((((BYTE)(BlueByte)  | ((WORD)((BYTE)(GreenByte)) << 8)) | (((DWORD)(BYTE)(RedByte))   << 16))) | (((DWORD)(BYTE)(AlphaByte)) << 24)) 
	#define SCF_WINRGB(RedByte, GreenByte, BlueByte)          ((SCFGraphics::ColorRGB)  (((BYTE)(RedByte)   | ((WORD)((BYTE)(GreenByte)) << 8)) | (((DWORD)(BYTE)(BlueByte))  << 16))) 

	//Color decomposition macros
	#define SCF_RGBA_A(dwRGBA)     ((BYTE)(dwRGBA))
	#define SCF_RGBA_B(dwRGBA)     ((BYTE)(((WORD)(dwRGBA)) >> 8))
	#define SCF_RGBA_G(dwRGBA)     ((BYTE)((dwRGBA) >> 16))
	#define SCF_RGBA_R(dwRGBA)     ((BYTE)((dwRGBA) >> 24))

	#define SCF_ARGB_B(dwARGB)     ((BYTE)(dwARGB))
	#define SCF_ARGB_G(dwARGB)     ((BYTE)(((WORD)(dwARGB)) >> 8))
	#define SCF_ARGB_R(dwARGB)     ((BYTE)((dwARGB) >> 16))
	#define SCF_ARGB_A(dwARGB)     ((BYTE)((dwARGB) >> 24))

	#define SCF_RGB_R(dwRGB)       ((BYTE)((dwRGB) >> 16))
	#define SCF_RGB_G(dwRGB)       ((BYTE)(((WORD)(dwRGB)) >> 8))
	#define SCF_RGB_B(dwRGB)       ((BYTE)(dwRGB))

	#define SCF_WINRGB_R(dwWINRGB) ((BYTE)(dwWINRGB))
	#define SCF_WINRGB_G(dwWINRGB) ((BYTE)(((WORD)(dwWINRGB)) >> 8))
	#define SCF_WINRGB_B(dwWINRGB) ((BYTE)((dwWINRGB) >> 16))

	//Color conversion macros
	#define SCF_RGBA_TO_ARGB(dwRGBA)     ((SCFGraphics::ColorARGB)(((BYTE)(dwRGBA) << 24) | ((DWORD)(dwRGBA) >> 8)))
	#define SCF_ARGB_TO_RGBA(dwARGB)     ((SCFGraphics::ColorARGB)((BYTE)((dwARGB) >> 24) | ((DWORD)(dwARGB) << 8)))

	#define SCF_RGB_TO_ARGB(dwRGB)       ((SCFGraphics::ColorARGB)((DWORD)((BYTE)(255) << 24) | (DWORD)(dwRGB)))
	#define SCF_RGB_TO_RGBA(dwRGB)       ((SCFGraphics::ColorARGB)((DWORD)(dwRGB << 8) | (DWORD)(255)))

	#define SCF_WINRGB_TO_ARGB(dwWINRGB) ((SCFGraphics::ColorARGB)((DWORD)((BYTE)(255) << 24) | (DWORD)(((SCF_WINRGB_R(dwWINRGB) << 16) | (SCF_WINRGB_G(dwWINRGB) << 8)) | SCF_WINRGB_B(dwWINRGB))))
	#define SCF_WINRGB_TO_RGBA(dwWINRGB) ((SCFGraphics::ColorARGB)(((DWORD)(((SCF_WINRGB_R(dwWINRGB) << 16) | (SCF_WINRGB_G(dwWINRGB) << 8)) | SCF_WINRGB_B(dwWINRGB)) << 8) | (DWORD)(255)))


	class SCFGRAPHICS_API CColor : public CValue
	{
		friend class CValue;
	
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static SCFGraphics::ColorRGBA Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed = nullptr);
		static CString                Print(_IN  SCFGraphics::ColorRGBA& rValue);

	public:
		CString ToString() _GET;

	public:
		CColor(_IN SCFGraphics::ColorRGBA& rValue);
		CColor(_IN BYTE ucR, _IN BYTE ucG, _IN BYTE ucB, _IN BYTE ucA);
		CColor(_IN CString& rString);
		CColor(_IN CColor& rValue);

		virtual ~CColor();

	public:
		BYTE At         (_IN UINT uiIndex) _GET;
		BYTE operator [](_IN UINT uiIndex) _GET;

	public:
		BYTE R()                  _GET { return SCF_RGBA_R(m_Value); }
		void      R(_IN BYTE ucR) _SET { m_Value &= 0x00ffffff; m_Value |= (ucR << 24); }

		BYTE G()                  _GET { return SCF_RGBA_G(m_Value); }
		void      G(_IN BYTE ucG) _SET { m_Value &= 0xff00ffff; m_Value |= (ucG << 16); }

		BYTE B()                  _GET { return SCF_RGBA_B(m_Value); }
		void      B(_IN BYTE ucB) _SET { m_Value &= 0xffff00ff; m_Value |= (ucB << 8); }

		BYTE A()                  _GET { return SCF_RGBA_A(m_Value); }
		void      A(_IN BYTE ucA) _SET { m_Value &= 0xffffff00; m_Value |= ucA; }

	public:
		void AtPut(_IN UINT uiIndex, _IN BYTE ucValue) _SET;

	public:
		inline const SCFGraphics::ColorRGBA Value() _GET { return m_Value; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		SCFGraphics::ColorRGBA m_Value;

	private:
		CColor() { m_Value = 0; }
	};
};
