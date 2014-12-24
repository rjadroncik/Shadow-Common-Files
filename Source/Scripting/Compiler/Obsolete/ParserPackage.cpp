#include "ParserPackage.h"

#include "Scanner/TokenWord.h"
#include "Scanner/TokenOperator.h"

using namespace SCF;
using namespace SCFCompiler;

bool CParserPackage::Parse(_INOUT CEnumeratorList& rTokens)
{
	if (rTokens.Current()->ClassKey() == ClassTokenWord)
	{
		CTokenWord* pTokenWord = (CTokenWord*)rTokens.Current();

		if (pTokenWord->IsKeyword() && (pTokenWord->Keyword() == KeywordPackage))
		{
			rTokens.Next();
			if (rTokens.Current()->ClassKey() == ClassTokenWord)
			{
				CTokenWord* pTokenWordPackage = (CTokenWord*)rTokens.Current();

				if (!pTokenWordPackage->IsKeyword())
				{
					///CREATE A PACKGE WITH THE GIVEN NAME!!!
 
					rTokens.Next();
					if (rTokens.Current()->ClassKey() == ClassTokenOperator)
					{
						CTokenOperator* pTokenOperator = (CTokenOperator*)rTokens.Current();

						if (pTokenOperator->Operator() == OperatorEnd)
						{
							rTokens.Next();
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}