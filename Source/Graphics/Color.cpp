#include "Color.h"

using namespace SCFBase;
using namespace SCFGraphics;

ColorRGBA CColor::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiCharsParsed = 0;
	register SCF::TCHAR* szString = rString.Value();
	
	for ( ; uiCharsParsed < rString.Length(); uiCharsParsed++)
	{
		if (szString[uiCharsParsed] == '#') 
		{
			uiCharsParsed++;
			ColorRGBA Color = 0;

			for (SCF::UINT j = 0; j < 7; j++, uiCharsParsed++)
			{
				if ((szString[uiCharsParsed] >= '0') && (szString[uiCharsParsed] <= '9')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - '0')      & 0x0f; Color <<= 4; continue; }
				if ((szString[uiCharsParsed] >= 'a') && (szString[uiCharsParsed] <= 'f')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - 'a' + 10) & 0x0f; Color <<= 4; continue; }
				if ((szString[uiCharsParsed] >= 'A') && (szString[uiCharsParsed] <= 'F')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - 'A' + 10) & 0x0f; Color <<= 4; continue; }

				//If we got here, the color is not well formed
				if (uipOutCharsParsed) { *uipOutCharsParsed = 0; }
				return 0; 
			}

			if ((szString[uiCharsParsed] >= '0') && (szString[uiCharsParsed] <= '9')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - '0')      & 0x0f; if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsed + 1; } return Color; }
			if ((szString[uiCharsParsed] >= 'a') && (szString[uiCharsParsed] <= 'f')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - 'a' + 10) & 0x0f; if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsed + 1; } return Color; }
			if ((szString[uiCharsParsed] >= 'A') && (szString[uiCharsParsed] <= 'F')) { Color |= (SCF::BYTE)(szString[uiCharsParsed] - 'A' + 10) & 0x0f; if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsed + 1; } return Color; }

			//If we got here, the color is not well formed
			if (uipOutCharsParsed) { *uipOutCharsParsed = 0; }
			return 0; 
		}
	}

	//If we got here, the color is not well formed
	if (uipOutCharsParsed) { *uipOutCharsParsed = 0; }
	return 0;
}

CString CColor::Print(_IN ColorRGBA& rValue)
{
	ColorRGBA Color = rValue;

	CString RetVal;
	RetVal.Resize(9);
	RetVal.AtPut(0, '#');

	for (SCF::UINT i = 8; i > 0; i--)
	{
		if ((Color & 0x0f) > 9)
		{
			RetVal.AtPut(i, 'A' + (SCF::BYTE)(Color & 0x0f) - 10);
		}
		else
		{
			RetVal.AtPut(i, '0' + (SCF::BYTE)(Color & 0x0f));
		}

		Color >>= 4;
	}

	return RetVal;
}

CColor::CColor(_IN SCF::BYTE ucR, _IN SCF::BYTE ucG, _IN SCF::BYTE ucB, _IN SCF::BYTE ucA) { m_Value = SCF_RGBA(ucR, ucG, ucB, ucA); }

CColor::CColor(_IN ColorRGBA& rValue)  { m_Value = rValue; }
CColor::CColor(_IN CColor&    rValue)  { m_Value = rValue.m_Value; }
CColor::CColor(_IN CString&   rString) { m_Value = CColor::Parse(rString, NULL); }

CColor::~CColor() {}

CString CColor::ToString() _GET { return CColor::Print(m_Value); }

void CColor::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_Value);
}

void CColor::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Value = rStream.GetInt();
}

#define SCF_RGBA_A(dwRGBA)     ((SCF::BYTE)(dwRGBA))
#define SCF_RGBA_B(dwRGBA)     ((SCF::BYTE)(((SCF::WORD)(dwRGBA)) >> 8))
#define SCF_RGBA_G(dwRGBA)     ((SCF::BYTE)((dwRGBA) >> 16))
#define SCF_RGBA_R(dwRGBA)     ((SCF::BYTE)((dwRGBA) >> 24))

SCF::BYTE CColor::At(_IN SCF::UINT uiIndex) _GET 
{ 
	_ASSERTE(uiIndex < 4);
	return (SCF::BYTE)((m_Value) >> (24 - (uiIndex * 8)));
}
SCF::BYTE CColor::operator[](_IN SCF::UINT uiIndex) _GET
{
	_ASSERTE(uiIndex < 4);
	return (SCF::BYTE)((m_Value) >> (24 - (uiIndex * 8)));
}
void CColor::AtPut(_IN SCF::UINT uiIndex, _IN SCF::BYTE ucValue) _SET
{
	ColorRGBA color = ucValue;
	color <<= (24 - (uiIndex * 8));

	ColorRGBA mask = 0xff;
	mask <<= (24 - (uiIndex * 8));

	m_Value |= mask;
	m_Value ^= mask;
	m_Value |= color;
}