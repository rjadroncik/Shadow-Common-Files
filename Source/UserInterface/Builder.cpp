#include "Builder.h"

#include "Window.h"
#include "ButtonClick.h"

using namespace SCFXML;
using namespace SCFBase;
using namespace SCFUI;

CModule::CModule()
{
	m_pSource  = NULL;
	m_szReport = NULL;
}

CModule::~CModule()
{
	if (m_pSource)  { delete m_pSource; }
	if (m_szReport) { free(m_szReport); }
}

void CModule::RepportAppend(_IN LPTSTR szText)
{
	//m_szReport = (LPTSTR)realloc(m_szReport, sizeof(TCHAR) * (crlStrLen(m_szReport) + crlStrLen(szText) + 1));

	//crlStrCat(m_szReport, szText);
} 

bool CModule::ConstructButtonClick(_IN CXMLElement& rElement, _INOUT CWindowAbstract* pParent)
{
	CButtonClick* pButton = new CButtonClick(pParent);
	
	m_Dictionary.AtPut(rElement.AttributeFirst()->Value()->ToString(), *pButton);

    //TODO: needs descendands or just children
	CXMLNode* pRectangle = rElement.DescendantsNamed(TEXT("rectangle")); 
	CXMLNode* pLabel     = rElement.DescendantsNamed(TEXT("label"));

	if (pRectangle) { pButton->RectWindow(SCFGraphics::CRectInt(pRectangle->Value()->ToString()).Value()); }
	if (pLabel)     { pButton->Label(pLabel->Value()->ToString().Value()); }

	pButton->Visible(TRUE);
	return TRUE;
}

bool CModule::ConstructDialog(_IN CXMLElement& rElement)
{
	CWindow* pDialog = new CWindow();
	
	m_Dictionary.AtPut(rElement.AttributeFirst()->Value()->ToString(), *pDialog);

	CXMLEnumerator Enumerator(rElement);

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->Name() == TEXT("buttonClick"))
		{
			if (!ConstructButtonClick((CXMLElement&)*Enumerator.Current(), pDialog)) { return FALSE; }
		}
	}

	pDialog->Visible(TRUE);
	return TRUE;
}

bool CModule::BuildFrom(_IN LPTSTR szFilePath)
{
	if (m_pSource) { delete m_pSource; }

	m_pSource = new CXMLDocumentFile(CFile(szFilePath));

	CXMLEnumerator Enumerator(*m_pSource);

	while (Enumerator.Next())
	{
		if (Enumerator.Current()->Name() == TEXT("dialog"))
		{
			ConstructDialog((CXMLElement&)*Enumerator.Current());

		/*	
			//Skip unlabeled nodes
			if (!Enumerator.Current()->AttributeFirst()) { continue; }

			//Find nodes containing relevant data
			CNode* pFile    = Enumerator.Current()->Find(TEXT("file"));
			CNode* pTarget  = Enumerator.Current()->Find(TEXT("target"));
			CNode* pMipMaps = Enumerator.Current()->Find(TEXT("mipMaps"));

			//Check whether required nodes were found
			if (!pFile || !pMipMaps) { continue; }

			UINT eTarget = GL_TEXTURE_2D;

			if (pTarget)
			{
				if (crlStrStr(pTarget->Value()->String(), TEXT("1D")))      { eTarget = GL_TEXTURE_1D; }
				if (crlStrStr(pTarget->Value()->String(), TEXT("2D")))      { eTarget = GL_TEXTURE_2D; }
				if (crlStrStr(pTarget->Value()->String(), TEXT("3D")))      { eTarget = GL_TEXTURE_3D; }
				if (crlStrStr(pTarget->Value()->String(), TEXT("cubeMap"))) { eTarget = GL_TEXTURE_CUBE_MAP; }
			}

			//Create the new texture & register it in texture cache
			CTexture* pTexture = new CTexture(eTarget, pMipMaps->Value()->ParseBOOL(), pFile->Value()->String());
			m_Textures.Add(*pTexture, Enumerator.Current()->AttributeFind(TEXT("id"))->Value()->String());
		*/
		}
	}

	/*m_Dictionary.EnumRestart();

	while (m_Dictionary.EnumNext())
	{
		MessageBox(NULL, ((CWindowAbstract*)m_Dictionary.EnumCurrent())->Label(), m_Dictionary.ObjectToName(*m_Dictionary.EnumCurrent()), MB_OK);
	}*/

	return TRUE;
}