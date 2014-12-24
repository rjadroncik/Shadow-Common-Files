#include "ComparerBoundValue.h"

using namespace SCFBase;
using namespace SCF;
using namespace SCFXMLPrivate;

CXMLComparerBoundValue::CXMLComparerBoundValue()
{
}

CXMLComparerBoundValue::~CXMLComparerBoundValue()
{
}

bool CXMLComparerBoundValue::LessOrEqual(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	if (r1st.ClassKey() == r2nd.ClassKey())
	{
		switch (r1st.ClassKey())
		{
		case ClassBool:   { return   (CBool&)r1st <=   (CBool&)r2nd; }
		case ClassInt:    { return    (CInt&)r1st <=    (CInt&)r2nd; }
		case ClassFloat:  { return  (CFloat&)r1st <=  (CFloat&)r2nd; }
		case ClassString: { return (CString&)r1st <= (CString&)r2nd; }
		}
	}
	else
	{
		if (r1st.ClassKey() == ClassString)
		{
			UINT uiCharsParsed = 0;
			CValue* p1stValue = &CValue::Parse(r2nd.ClassKey(), (const CString&)r1st, &uiCharsParsed);
			bool bLessOrEqual = FALSE;

			switch (r2nd.ClassKey())
			{
			case ClassBool:   { bLessOrEqual =   *(CBool*)p1stValue <=   (CBool&)r2nd; break; }
			case ClassInt:    { bLessOrEqual =    *(CInt*)p1stValue <=    (CInt&)r2nd; break; }
			case ClassFloat:  { bLessOrEqual =  *(CFloat*)p1stValue <=  (CFloat&)r2nd; break; }
			case ClassString: { bLessOrEqual = *(CString*)p1stValue <= (CString&)r2nd; break; }
			}

			delete p1stValue;
			return bLessOrEqual;
		}
		else if (r2nd.ClassKey() == ClassString)
		{
			UINT uiCharsParsed = 0;
			CValue* p2ndValue = &CValue::Parse(r1st.ClassKey(), (const CString&)r2nd, &uiCharsParsed);
			bool bLessOrEqual = FALSE;

			switch (r1st.ClassKey())
			{
			case ClassBool:   { bLessOrEqual =   (CBool&)r1st <=   *(CBool*)p2ndValue; break; }
			case ClassInt:    { bLessOrEqual =    (CInt&)r1st <=    *(CInt*)p2ndValue; break; }
			case ClassFloat:  { bLessOrEqual =  (CFloat&)r1st <=  *(CFloat*)p2ndValue; break; }
			case ClassString: { bLessOrEqual = (CString&)r1st <= *(CString*)p2ndValue; break; }
			}

			delete p2ndValue;
			return bLessOrEqual;
		}
	}

	return FALSE;
}

bool CXMLComparerBoundValue::Equal(_IN CObject& r1st, _IN CObject& r2nd) _GET
{
	if (r1st.ClassKey() == r2nd.ClassKey())
	{
		switch (r1st.ClassKey())
		{
		case ClassBool:   { return   (CBool&)r1st ==   (CBool&)r2nd; }
		case ClassInt:    { return    (CInt&)r1st ==    (CInt&)r2nd; }
		case ClassFloat:  { return  (CFloat&)r1st ==  (CFloat&)r2nd; }
		case ClassString: { return (CString&)r1st == (CString&)r2nd; }
		}
	}
	else
	{
		if (r1st.ClassKey() == ClassString)
		{
			UINT uiCharsParsed = 0;
			CValue* p1stValue = &CValue::Parse(r2nd.ClassKey(), (const CString&)r1st, &uiCharsParsed);
			bool bEqual = FALSE;

			switch (r2nd.ClassKey())
			{
			case ClassBool:   { bEqual =   *(CBool*)p1stValue ==   (CBool&)r2nd; break; }
			case ClassInt:    { bEqual =    *(CInt*)p1stValue ==    (CInt&)r2nd; break; }
			case ClassFloat:  { bEqual =  *(CFloat*)p1stValue ==  (CFloat&)r2nd; break; }
			case ClassString: { bEqual = *(CString*)p1stValue == (CString&)r2nd; break; }
			}

			delete p1stValue;
			return bEqual;
		}
		else if (r2nd.ClassKey() == ClassString)
		{
			UINT uiCharsParsed = 0;
			CValue* p2ndValue = &CValue::Parse(r1st.ClassKey(), (const CString&)r2nd, &uiCharsParsed);
			bool bEqual = FALSE;

			switch (r1st.ClassKey())
			{
			case ClassBool:   { bEqual =   (CBool&)r1st ==   *(CBool*)p2ndValue; break; }
			case ClassInt:    { bEqual =    (CInt&)r1st ==    *(CInt*)p2ndValue; break; }
			case ClassFloat:  { bEqual =  (CFloat&)r1st ==  *(CFloat*)p2ndValue; break; }
			case ClassString: { bEqual = (CString&)r1st == *(CString*)p2ndValue; break; }
			}

			delete p2ndValue;
			return bEqual;
		}
	}

	return FALSE;
}