#include <SCFScripting.h> 

#include "Preprocessor.h"

using namespace SCF;
using namespace SCFBase;
using namespace SCFScripting;
using namespace SCFLibrarian;

// bool AnalyzeLine(Line)
// {
// 
// 
// }

bool ParseFile(_IN CFile& rFile)
{
	CStreamFileRead StreamRead(rFile);
	CStreamReadTextGeneric StreamReadText(StreamRead);

	CString Text;
	StreamReadText.GetString(Text, 0);

	CPreprocessor Preprocessor;
	Preprocessor.Preprocess(Text);

// 	CString Line;
// 
// 	while (StreamReadText.GetLine(Line))
// 	{
// 		//AnalyzeLine(Line);
// 	}

	return TRUE;
}

int __cdecl main(int argc, char* argv[])
{
	SCF_UNREFERENCED_PARAMETER(argc);
	SCF_UNREFERENCED_PARAMETER(argv);

	ParseFile(STRING("D:\\Roman\\Programming\\Shadow Common Files\\Source\\ObjectExtensions\\Classes.h"));

	return 0;
}

