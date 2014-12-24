#include "StdAfx.h"
#include "VectorValue.h"

#include "StreamReadObject.h"
#include "StreamWriteObject.h"

using namespace SCFObjectExtensions;

CVectorValue::CVectorValue(_IN CFloat& rX, _IN CFloat& rY)
{
	m_uiCount = 2;
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * m_uiCount);

	m_ppValues[0] = (CValue*)&rX; rX.AddRef();
	m_ppValues[1] = (CValue*)&rY; rY.AddRef();
}

CVectorValue::CVectorValue(_IN CFloat& rX, _IN CFloat& rY, _IN CFloat& rZ)
{
	m_uiCount = 3;
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * m_uiCount);

	m_ppValues[0] = (CValue*)&rX; rX.AddRef();
	m_ppValues[1] = (CValue*)&rY; rY.AddRef();
	m_ppValues[2] = (CValue*)&rZ; rZ.AddRef();
}

CVectorValue::CVectorValue(_IN CFloat& rX, _IN CFloat& rY, _IN CFloat& rZ, _IN CFloat& rW)
{
	m_uiCount = 4;
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * m_uiCount);

	m_ppValues[0] = (CValue*)&rX; rX.AddRef();
	m_ppValues[1] = (CValue*)&rY; rY.AddRef();
	m_ppValues[2] = (CValue*)&rZ; rZ.AddRef();
	m_ppValues[3] = (CValue*)&rW; rW.AddRef();
}

CVectorValue::CVectorValue()
{
	m_ppValues = NULL;
	m_uiCount = 0;
}


CVectorValue::~CVectorValue()
{
	this->RemoveAll();
}

void* CVectorValue::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassVectorValue: { return (CVectorValue*)this; }
	}

	return NULL;
}

CValue& CVectorValue::At(_IN UINT uiIndex) _GET 
{
	if (uiIndex > m_uiCount) { throw SCFException(ExceptionIndexOverflow); }

	return *m_ppValues[uiIndex]; 
}

CValue& CVectorValue::operator [](_IN UINT uiIndex) _GET 
{
	if (uiIndex > m_uiCount) { throw SCFException(ExceptionIndexOverflow); }

	return *m_ppValues[uiIndex];
}

void CVectorValue::AtPut(_IN UINT uiIndex, _IN _REF CValue& rValue) _SET
{
	if (uiIndex > m_uiCount) { throw SCFException(ExceptionIndexOverflow); }

	//Release the previously stored object, so it can be deleted
	if (m_ppValues[uiIndex]) { m_ppValues[uiIndex]->Release(); }

	m_ppValues[uiIndex] = (CValue*)&rValue;
	rValue.AddRef();
}

void CVectorValue::AddLast(_IN _REF CValue& rValue)
{
	m_uiCount++;
	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);

	m_ppValues[m_uiCount - 1] = (CValue*)&rValue;
	rValue.AddRef();
}

void CVectorValue::RemoveLast()
{
	if (!m_uiCount) { throw SCFException(ExceptionIllegalOperation); }

	m_uiCount--;
	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::Remove(_IN UINT uiIndex)
{
	if (!m_uiCount)          { throw SCFException(ExceptionIllegalOperation); }
	if (uiIndex > m_uiCount) { throw SCFException(ExceptionIndexOverflow); }
	
	m_ppValues[uiIndex]->Release();

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppValues[i] = m_ppValues[i + 1]; }

	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::Delete(_IN UINT uiIndex)
{
	if (!m_uiCount)          { throw SCFException(ExceptionIllegalOperation); }
	if (uiIndex > m_uiCount) { throw SCFException(ExceptionIndexOverflow); }

	m_ppValues[uiIndex]->Release();
	delete m_ppValues[uiIndex];

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppValues[i] = m_ppValues[i + 1]; }

	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::Remove(_IN CValue& rValue)
{
	if (!m_uiCount) { throw SCFException(ExceptionIllegalOperation); }

	UINT uiIndex = (UINT)-1;
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppValues[i] == &rValue) { uiIndex = i; break; } }

	if (uiIndex == m_uiCount) { return; }

	m_ppValues[uiIndex]->Release();

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppValues[i] = m_ppValues[i + 1]; }

	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::Delete(_IN CValue& rValue)
{
	if (!m_uiCount) { throw SCFException(ExceptionIllegalOperation); }

	UINT uiIndex = (UINT)-1;
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppValues[i] == &rValue) { uiIndex = i; break; } }

	if (uiIndex == m_uiCount) { return; }

	m_ppValues[uiIndex]->Release();
	delete m_ppValues[uiIndex];

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppValues[i] = m_ppValues[i + 1]; }

	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiCount);
}

void CVectorValue::Deserialize(_INOUT IStreamRead& rStream)
{
	m_uiCount = rStream.GetInt();
	m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CValue*) * m_uiCount);
}

void CVectorValue::SerializeDependents(_INOUT IStreamWriteObject& rStream) const
{
	for (UINT i = 0; i < m_uiCount; i++)
	{
		rStream.Next((CObjectSerializable&)*m_ppValues[i]);
	}
}

void CVectorValue::DeserializeDependents(_INOUT IStreamReadObject& rStream)
{
	for (UINT i = 0; i < m_uiCount; i++)
	{
		m_ppValues[i] = rStream.Next();
		m_ppValues[i]->AddRef();	
	}
}

void CVectorValue::RemoveAll()
{
	for (UINT i = 0; i < m_uiCount; i++) { m_ppValues[i]->Release(); }

	m_uiCount = 0;
	if (m_ppValues) { free(m_ppValues); m_ppValues = NULL; }
}

void CVectorValue::DeleteAll()
{
	for (UINT i = 0; i < m_uiCount; i++) { m_ppValues[i]->Release(); delete m_ppValues[i]; }

	m_uiCount = 0;
	if (m_ppValues) { free(m_ppValues); m_ppValues = NULL; }
}

CValue* CVectorValue::Find(_IN CValue& rValue) _GET 
{
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppValues[i] == &rValue) { return m_ppValues[i]; } }

	return NULL;
}

CValue* CVectorValue::FindEqual(_IN CValue& rValue) _GET  
{
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppValues[i]->IsEqualTo(rValue) || rValue.IsEqualTo(*m_ppValues[i])) { return m_ppValues[i]; } }

	return NULL;
}

