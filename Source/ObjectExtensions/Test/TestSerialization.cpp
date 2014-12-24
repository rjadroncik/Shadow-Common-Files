#include "stdafx.h"
#include "TestSerialization.h"

CTestSerialization::CTestSerialization(_INOUT IStreamWriteText& rErrorStream) : CTestCase2Phase(STRING("Test of serialization"), &rErrorStream)
{
	m_pArrayInt = NULL;
	m_pArrayFloat = NULL; 

	m_pAssociation = NULL;
	m_pVector = NULL;
	m_pList = NULL;

	m_pDictionaryString = NULL;
	//m_pDictionaryInt64 = NULL;

	m_pStreamWrite = NULL;
	m_pStreamWriteObject = NULL;

	m_pStreamRead = NULL;
	m_pStreamReadObject = NULL;
}

CTestSerialization::~CTestSerialization()
{

}

bool CTestSerialization::Prepare() 
{
	m_pArrayInt = new CArrayInt(4);
	m_pArrayInt->AtPut(3, CInt(99));
	m_pArrayInt->AtPut(2, CInt(88));
	m_pArrayInt->AtPut(1, CInt(77));
	m_pArrayInt->AtPut(0, CInt(66));

	m_pArrayFloat = new CArrayFloat(3);
	m_pArrayFloat->AtPut(0, CFloat(66.66f));
	m_pArrayFloat->AtPut(1, CFloat(77.77f));
	m_pArrayFloat->AtPut(2, CFloat(88.88f));

	m_pAssociation = new CAssociation(*(new CString(STRING_CREATE_ARGS("key"))), *(new CInt(5)));

	m_pVector = new CVector<CObject>();
	m_pVector->LastAdd(*(new STRING("String")));
	m_pVector->LastAdd(*(new CInt(2)));
	m_pVector->LastAdd(*(new CBool(TRUE)));
	m_pVector->LastAdd(*(new CFloat(1.5f)));

	m_pList = new CList<CObject>();
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new CInt(2)));
	m_pList->LastAdd(*(new CBool(TRUE)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new CInt(2)));
	m_pList->LastAdd(*(new CBool(TRUE)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new CInt(2)));
	m_pList->LastAdd(*(new CBool(TRUE)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new CInt(2)));
	m_pList->LastAdd(*(new CBool(TRUE)));
	m_pList->LastAdd(*(new CFloat(1.5f)));
	m_pList->LastAdd(*(new STRING("String")));
	m_pList->LastAdd(*(new CInt(2)));
	m_pList->LastAdd(*(new CBool(TRUE)));
	m_pList->LastAdd(*(new CFloat(1.5f)));

	m_pList->LastAdd(*(new CVectorRange(*m_pVector, 1, 1)));

	m_pList->LastAdd(*(new STRING("12345678901234567890")));
	m_pList->LastAdd(*(new CStringRange((CString&)m_pList->Last(), 8, 3)));

	m_pDictionaryString = new CDictionaryString<CFloat>();
	m_pDictionaryString->AtPut(STRING("Test"),      *(new CFloat(15)));
	m_pDictionaryString->AtPut(STRING("TestDruhy"), *(new CFloat(16)));
	m_pDictionaryString->AtPut(STRING("Test3"),     *(new CFloat(17))); 

	//m_pDictionaryInt64 = new CDictionaryInt64();
	//m_pDictionaryInt64->AtPut(888, *(new CFloat(15)));
	//m_pDictionaryInt64->AtPut(889, *(new CFloat(16)));
	//m_pDictionaryInt64->AtPut(890, *(new CFloat(17))); 

	m_pStreamWrite = new CStreamFileWrite(CFile(STRING("Data\\ObjectExtensions\\Tmp.bin")), false);
	m_pStreamWriteObject = new CStreamWriteObject(*m_pStreamWrite);

	return TRUE;
}
bool CTestSerialization::Run()    
{ 
	m_pStreamWriteObject->Next(*m_pArrayInt);
	m_pStreamWriteObject->Next(*m_pArrayFloat);

	m_pStreamWriteObject->Next(*m_pAssociation);

	//Swapped order on purpose - for testing [VectorRange] serialization
	m_pStreamWriteObject->Next(*m_pList);
	m_pStreamWriteObject->Next(*m_pVector);

	m_pStreamWriteObject->Next(*m_pDictionaryString);
	//m_pStreamWriteObject->Next(*m_pDictionaryInt64);

	m_pStreamWriteObject->Close();

	return TRUE;
}
bool CTestSerialization::Check()  
{
	return TRUE; 
}
bool CTestSerialization::CleanUp() 
{
	delete m_pStreamWriteObject;
	delete m_pStreamWrite;

	return TRUE;
}

bool CTestSerialization::Prepare2() 
{
	m_pStreamRead = new CStreamFileRead(CFile(STRING("Data\\ObjectExtensions\\Tmp.bin")));
	m_pStreamReadObject = new CStreamReadObject(*m_pStreamRead);

	return TRUE;
}
bool CTestSerialization::Run2()    
{ 
	while (m_pStreamReadObject->Next())
	{
		CError::Stream()->PutLine(m_pStreamReadObject->Current()->ToString());
		m_ReadObjects.LastAdd(*m_pStreamReadObject->Current());
	}

	m_pStreamReadObject->Close();

	return TRUE;
}
bool CTestSerialization::Check2()  
{
	return TRUE; 
}
bool CTestSerialization::CleanUp2() 
{
	delete m_pArrayInt;
	delete m_pArrayFloat;

	m_pAssociation->DeleteWithObjects();

	//Have to delete the StringRange before the String it depends on
	m_pList->LastDelete();
	m_pList->AllDelete();
	delete m_pList;

	//Swapped order on purpose - for testing [VectorRange] serialization
	m_pVector->AllDelete();
	delete m_pVector;

	m_pDictionaryString->AllDelete();
	//m_pDictionaryInt64->AllDelete();

	delete m_pDictionaryString;
	//delete m_pDictionaryInt64;

	delete m_pStreamReadObject;
	delete m_pStreamRead;

	while (m_ReadObjects.Size())
	{
		switch(m_ReadObjects[0].ClassKey())
		{
		case ClassAssociation:
			{
				CAssociation* pAssociation = &(CAssociation&)m_ReadObjects[0];
				m_ReadObjects.RemoveAt(0);
				pAssociation->DeleteWithObjects();
				break;
			}
		case ClassVector:
			{
				((CVector<CObject>&)m_ReadObjects[0]).AllDelete();
				m_ReadObjects.DeleteAt(0); 
				break;
			}
		case ClassList:
			{
				//Have to delete the StringRange before the String it depends on
				((CList<CObject>&)m_ReadObjects[0]).LastDelete();
				((CList<CObject>&)m_ReadObjects[0]).AllDelete();
				m_ReadObjects.DeleteAt(0); 
				break;
			}
		case ClassDictionaryString:
			{
				((CDictionaryString<CFloat>&)m_ReadObjects[0]).AllDelete();
				m_ReadObjects.DeleteAt(0); 
				break;
			}
		default: 
			{ 
				m_ReadObjects.DeleteAt(0); 
				break; 
			}
		}
	}

	return TRUE;
}

