#pragma once

#include "Errors.h"

#include <SCFObjectExtensions.h>

#ifdef DEBUGGING_EXPORTS
#define DEBUGGING_API __declspec(dllexport)
#else
#define DEBUGGING_API __declspec(dllimport)
#endif

namespace SCFDebugging
{
	//Description: A 'template'-like base-class for 'standardized' testing.
	//
	//Usage: Derive(use as public base class), implement(override) at least one of these functions:
	//  Prepare(), Run(), Check(), CleanUp(). To run test create an instance of
	//  the derived class & call Perform().
	//
	//Notes: As of version 2.00, this class is a 'FULL' object (derives from CObject)
	//
	//Version: 2.000

	class DEBUGGING_API CTestCase : public SCFBase::CObject
	{
	public:
		CString   ToString() _GET { return STRING("{TestCase}"); }

	public:
		virtual ~CTestCase();

#ifdef _BETA
	public:
		//Enables/disables the tracing of object creation & deletion during testing, tracing is off default 
		bool Tracing()                  _GET { return m_bTracing; }
		void Tracing(_IN bool bTracing) _SET { m_bTracing = bTracing; }
#endif

	public:
		//Executes the test case scenario & reports results (WHERE??? stream?, stderr?)
		virtual bool Perform();

	protected:
		//These are invoked in the order as they are declared here
		//Basic idea is:
		//Prepare() - constructs objects & puts them in a fully defined state
		//Run() - performs a set of operations; these are the ones we want to test
		//Check() - checks the state of the objects against known valid result states
		//CleanUp() - deletes all created objects; could be called CleanUp()
		virtual bool Prepare();
		virtual bool Run();
		virtual bool Check();
		virtual bool CleanUp();

	protected:
		//Executes the test case scenario without detecting memory leaks
		//NOTICE: This is used by the 2phase test case to detect memory leaks in the whole test case & not just per phase
		bool PerformInternal();

	protected:
		void ErrorReport(_IN ENUM eError, _IN CString& rStage);

	protected:
		CTestCase(_IN CString& rName, _INOUT IStreamWriteText* pErrorStream = nullptr);

	protected:
		CString           m_csName;
		IStreamWriteText* m_pErrorStream;

#ifdef _BETA
	protected:
		bool m_bTracing;
		UINT m_uiGlobalObjectCount;
#endif
	};
};



