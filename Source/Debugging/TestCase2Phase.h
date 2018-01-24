#pragma once

#include "TestCase.h"

namespace SCFDebugging
{
	//Description: A 'template'-like base-class for 2-phase 'standardized' testing.
	//
	//Usage: Derive(use as public base class), implement(override) at least one of these functions:
	//  Prepare{2}(), Run{2}(), Check{2}(), CleanUp{2}(). To run test create an instance of
	//  the derived class & call Perform().
	//
	//Notes: As of version 2.00, this class is a 'FULL' object (derives from CObject)
	//
	//Version: 2.000

	class DEBUGGING_API CTestCase2Phase : public CTestCase
	{
	public:
		CString   ToString() _GET { return STRING("{TestCase2Phase}"); }

	public:
		virtual ~CTestCase2Phase();

	public:
		//Executes the test case scenario & reports results (WHERE??? stream?, stderr?)
		virtual bool Perform();

	protected:
		//These are invoked in the order as they are declared here
		//Basic idea is:
		//Prepare() - constructs objects & puts them in a fully defined state
		//Run() - performs a set of operations; these are the ones we want to test
		//Check() - checks the state of the objects against known valid result states
		//Dispose() - deletes all created objects; could be called CleanUp()
		virtual bool Prepare2();
		virtual bool Run2();
		virtual bool Check2();
		virtual bool CleanUp2();

	protected:
		CTestCase2Phase(_IN CString& rName, _INOUT IStreamWriteText* pErrorStream = nullptr);
	};
};



