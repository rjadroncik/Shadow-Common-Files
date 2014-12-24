#pragma once
#include "Classes.h"

namespace SCFGraphics
{
	//Color types, data alignment is most-to-least significant byte, from iX-to-iWidth (e.i.: when composing a [ColorRGBA] value the A (Alpha) component is not shifted)
	typedef SCF::DWORD ColorRGB;		
	typedef SCF::DWORD ColorRGBA;
	typedef SCF::DWORD ColorARGB;

	typedef SCF::DWORD ColorWinRGB; //Important!!! - ColorWinRGB relates to standard Windows RGB format which has a different memory layout - BGR instead of RGB

	//Color composition macros
	#define SCF_RGB(RedByte, GreenByte, BlueByte)             ((SCFGraphics::ColorRGB)  (((SCF::BYTE)(BlueByte)  | ((SCF::WORD)((SCF::BYTE)(GreenByte)) << 8)) | (((SCF::DWORD)(SCF::BYTE)(RedByte))   << 16))) 
	#define SCF_RGBA(RedByte, GreenByte, BlueByte, AlphaByte) ((SCFGraphics::ColorRGBA)((((SCF::BYTE)(AlphaByte) | ((SCF::WORD)((SCF::BYTE)(BlueByte))  << 8)) | (((SCF::DWORD)(SCF::BYTE)(GreenByte)) << 16))) | (((SCF::DWORD)(SCF::BYTE)(RedByte))   << 24)) 
	#define SCF_ARGB(AlphaByte, RedByte, GreenByte, BlueByte) ((SCFGraphics::ColorRGBA)((((SCF::BYTE)(BlueByte)  | ((SCF::WORD)((SCF::BYTE)(GreenByte)) << 8)) | (((SCF::DWORD)(SCF::BYTE)(RedByte))   << 16))) | (((SCF::DWORD)(SCF::BYTE)(AlphaByte)) << 24)) 
	#define SCF_WINRGB(RedByte, GreenByte, BlueByte)          ((SCFGraphics::ColorRGB)  (((SCF::BYTE)(RedByte)   | ((SCF::WORD)((SCF::BYTE)(GreenByte)) << 8)) | (((SCF::DWORD)(SCF::BYTE)(BlueByte))  << 16))) 

	//Color decomposition macros
	#define SCF_RGBA_A(dwRGBA)     ((SCF::BYTE)(dwRGBA))
	#define SCF_RGBA_B(dwRGBA)     ((SCF::BYTE)(((SCF::WORD)(dwRGBA)) >> 8))
	#define SCF_RGBA_G(dwRGBA)     ((SCF::BYTE)((dwRGBA) >> 16))
	#define SCF_RGBA_R(dwRGBA)     ((SCF::BYTE)((dwRGBA) >> 24))

	#define SCF_ARGB_B(dwARGB)     ((SCF::BYTE)(dwARGB))
	#define SCF_ARGB_G(dwARGB)     ((SCF::BYTE)(((SCF::WORD)(dwARGB)) >> 8))
	#define SCF_ARGB_R(dwARGB)     ((SCF::BYTE)((dwARGB) >> 16))
	#define SCF_ARGB_A(dwARGB)     ((SCF::BYTE)((dwARGB) >> 24))

	#define SCF_RGB_R(dwRGB)       ((SCF::BYTE)((dwRGB) >> 16))
	#define SCF_RGB_G(dwRGB)       ((SCF::BYTE)(((SCF::WORD)(dwRGB)) >> 8))
	#define SCF_RGB_B(dwRGB)       ((SCF::BYTE)(dwRGB))

	#define SCF_WINRGB_R(dwWINRGB) ((SCF::BYTE)(dwWINRGB))
	#define SCF_WINRGB_G(dwWINRGB) ((SCF::BYTE)(((SCF::WORD)(dwWINRGB)) >> 8))
	#define SCF_WINRGB_B(dwWINRGB) ((SCF::BYTE)((dwWINRGB) >> 16))

	//Color conversion macros
	#define SCF_RGBA_TO_ARGB(dwRGBA)     ((SCFGraphics::ColorARGB)(((SCF::BYTE)(dwRGBA) << 24) | ((SCF::DWORD)(dwRGBA) >> 8)))
	#define SCF_ARGB_TO_RGBA(dwARGB)     ((SCFGraphics::ColorARGB)((SCF::BYTE)((dwARGB) >> 24) | ((SCF::DWORD)(dwARGB) << 8)))

	#define SCF_RGB_TO_ARGB(dwRGB)       ((SCFGraphics::ColorARGB)((SCF::DWORD)((SCF::BYTE)(255) << 24) | (SCF::DWORD)(dwRGB)))
	#define SCF_RGB_TO_RGBA(dwRGB)       ((SCFGraphics::ColorARGB)((SCF::DWORD)(dwRGB << 8) | (SCF::DWORD)(255)))

	#define SCF_WINRGB_TO_ARGB(dwWINRGB) ((SCFGraphics::ColorARGB)((SCF::DWORD)((SCF::BYTE)(255) << 24) | (SCF::DWORD)(((SCF_WINRGB_R(dwWINRGB) << 16) | (SCF_WINRGB_G(dwWINRGB) << 8)) | SCF_WINRGB_B(dwWINRGB))))
	#define SCF_WINRGB_TO_RGBA(dwWINRGB) ((SCFGraphics::ColorARGB)(((SCF::DWORD)(((SCF_WINRGB_R(dwWINRGB) << 16) | (SCF_WINRGB_G(dwWINRGB) << 8)) | SCF_WINRGB_B(dwWINRGB)) << 8) | (SCF::DWORD)(255)))


	class SCFGRAPHICS_API CColor : public CValue
	{
		friend class CValue;
	
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static SCFGraphics::ColorRGBA Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed = NULL);
		static CString                Print(_IN  SCFGraphics::ColorRGBA& rValue);

	public:
		SCF::ENUM ClassKey() _GET { return ClassColor; }
		CString   ToString() _GET;

	public:
		CColor(_IN SCFGraphics::ColorRGBA& rValue);
		CColor(_IN SCF::BYTE ucR, _IN SCF::BYTE ucG, _IN SCF::BYTE ucB, _IN SCF::BYTE ucA);
		CColor(_IN CString& rString);
		CColor(_IN CColor& rValue);

		virtual ~CColor();

	public:
		SCF::BYTE At         (_IN SCF::UINT uiIndex) _GET;
		SCF::BYTE operator [](_IN SCF::UINT uiIndex) _GET;

	public:
		SCF::BYTE R()                  _GET { return SCF_RGBA_R(m_Value); }
		void      R(_IN SCF::BYTE ucR) _SET { m_Value &= 0x00ffffff; m_Value |= (ucR << 24); }

		SCF::BYTE G()                  _GET { return SCF_RGBA_G(m_Value); }
		void      G(_IN SCF::BYTE ucG) _SET { m_Value &= 0xff00ffff; m_Value |= (ucG << 16); }

		SCF::BYTE B()                  _GET { return SCF_RGBA_B(m_Value); }
		void      B(_IN SCF::BYTE ucB) _SET { m_Value &= 0xffff00ff; m_Value |= (ucB << 8); }

		SCF::BYTE A()                  _GET { return SCF_RGBA_A(m_Value); }
		void      A(_IN SCF::BYTE ucA) _SET { m_Value &= 0xffffff00; m_Value |= ucA; }

	public:
		void AtPut(_IN SCF::UINT uiIndex, _IN SCF::BYTE ucValue) _SET;

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
