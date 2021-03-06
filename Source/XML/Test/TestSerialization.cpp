#include "TestSerialization.h"

bool CTestSerialization::s_bRegistered = false;

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CTestSerialization::CTestSerialization(_INOUT IStreamWriteText& rErrorStream) : CTestCase2Phase(STRING("Test of XML serialization"), &rErrorStream)
{
	if (!s_bRegistered)
	{
		CLASS_XMLSERIALIZABLE_REGISTER("ClassA", TestNS::CClassA, GetModuleHandle(nullptr));
		CLASS_XMLSERIALIZABLE_REGISTER("ClassB", CClassB, GetModuleHandle(nullptr));

		CXMLEnumeration* pEnumeration = new CXMLEnumeration();
		{
			pEnumeration->ValueDefine(STRING("EnumValue01"), EnumValue01);
			pEnumeration->ValueDefine(STRING("EnumValue02"), EnumValue02);
			pEnumeration->ValueDefine(STRING("EnumValue03"), EnumValue03);
			pEnumeration->ValueDefine(STRING("EnumValue04"), EnumValue04);
		}
		CXMLEnumeration::Register(STRING("ClassBEnumeration"), *pEnumeration);

		s_bRegistered = true;
	}

	m_pDocument = nullptr;

	m_pStreamWrite = nullptr;
	m_pStreamWrite = nullptr;

	m_pObjectA  = nullptr;
	m_pObjectA2 = nullptr;
	m_pObjectB  = nullptr;
}

CTestSerialization::~CTestSerialization()
{

}

bool CTestSerialization::Prepare() 
{
	CFile* pFile = new CFile(STRING("Data\\XML\\Serialization.xml"));
	if (pFile->Exists())
	{
		pFile->Delete();
	}

	m_pDocument = new CXMLDocumentFile(*pFile);
	m_pStreamWrite = new CXMLStreamWriteObject(*m_pDocument);

	m_pObjectA  = new TestNS::CClassA(45);
	m_pObjectA2 = new TestNS::CClassA(80);
	m_pObjectB  = new CClassB();

	m_pObjectA->TestString(STRING("Testicek malicky zlatucky"));
	m_pObjectB->TestString(STRING("Testíček maličký zlatučký"));

/*	m_pObjectA->PropertySet(SCFScripting::CRegistry::IDOfProperty(STRING("TestString")), STRING("SomeText (a textík v slovenštine, ááách .. úžas!!!)"));
	m_pObjectA->PropertySet(SCFScripting::CRegistry::IDOfProperty(STRING("TestInt")),    CInt(88));
	m_pObjectA->PropertySet(SCFScripting::CRegistry::IDOfProperty(STRING("TestClassA")), *m_pObjectA2);

	//Make a chain cross-reference!!
	m_pObjectB->PropertySet (SCFScripting::CRegistry::IDOfProperty(STRING("TestClassA")),  *m_pObjectA2);
	m_pObjectB->PropertySet (SCFScripting::CRegistry::IDOfProperty(STRING("TestClassA2")), *m_pObjectA);
	m_pObjectA2->PropertySet(SCFScripting::CRegistry::IDOfProperty(STRING("TestClassA")),  *m_pObjectA);
*/
	m_ObjectsPersistent.AtPut(STRING("IdOfPersistent"), *m_pObjectA2);

	//Change a property of a sub-class
/*	CScriptableObject* pObject = (CScriptableObject*)m_pObjectA->PropertyGet(SCFScripting::CRegistry::IDOfProperty(STRING("TestClassBSub")));
	if (pObject)
	{ 
		pObject->PropertySet(SCFScripting::CRegistry::IDOfProperty(STRING("TestInt")), CInt(44)); 
	}
	else { return false; }
*/
	return true;
}
bool CTestSerialization::Run()    
{ 
	m_pStreamWrite->ObjectsWrittenSubmit(m_ObjectsPersistent);

	//Write both objects
	m_pStreamWrite->Next(m_pObjectA);
	m_pStreamWrite->Next(m_pObjectB);
	m_pStreamWrite->Next(m_pObjectA2);

	//Write document to file
	m_pDocument->Write(EncodingUTF8);

	return true;
}
bool CTestSerialization::Check()  
{
	return true;
}
bool CTestSerialization::CleanUp() 
{
	//Disposing breaks up the chain cross-reference we made for testing
	m_pObjectA->Dispose();
	m_pObjectA2->Dispose();
	m_pObjectB->Dispose();

	delete m_pObjectA;
	delete m_pObjectB;

	delete m_pStreamWrite;
	delete m_pDocument;

	return true;
}

bool CTestSerialization::Prepare2() 
{
	m_pDocument = new CXMLDocumentFile(*(new CFile(STRING("Data\\XML\\Serialization.xml"))));
	m_pStreamRead = new CXMLStreamReadObject(*m_pDocument);

	return true;
}
bool CTestSerialization::Run2() 
{ 
	m_pStreamRead->ObjectsReadSubmit(m_ObjectsPersistent); 

	while (m_pStreamRead->Next())
	{
		CError::Stream()->PutLine(m_pStreamRead->Current()->XmlName());

		m_Objects.LastAdd(*m_pStreamRead->Current());
	}

	return true;
}
bool CTestSerialization::Check2()  
{
	//Change a property of a sub-class
	//CObject* pObject = (CScriptableObject*)((CScriptableObject*)(&m_Objects[0]))->PropertyGet(SCFScripting::CRegistry::IDOfProperty(STRING("TestClassBSub")));
	//if (pObject)
	{
		//Check whether the values from de-serialization are correct
		//CInt* pInt = (CInt*)pObject->PropertyGet(SCFScripting::CRegistry::IDOfProperty(STRING("TestInt"))); 
		//if (!pInt || (pInt->Value() != 44)) { return false; }
		
		//delete pInt;
	}
	//else { return false; }

//	CString* pString = (CString*)((CScriptableObject*)(&m_Objects[0]))->PropertyGet(SCFScripting::CRegistry::IDOfProperty(STRING("TestString")));
//	if (!pString) { return false; }

//	CError::Stream()->PutLine(*pString);
//	delete pString;

	return true;
}
bool CTestSerialization::CleanUp2() 
{
	delete m_pStreamRead;
	delete m_pDocument;

	m_ObjectsPersistent.AllRemove();

	//The persistent object is among the returned objects when reading
	m_Objects.AllDispose();
	m_Objects.AllDelete();

	return true;
}



