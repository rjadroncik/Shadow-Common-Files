#pragma once
#include "Value.h"
#include "Float.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CVectorValue : public CValue
	{
	public:
		//Dynamic casting
		void* AsType(_IN ENUM eType) const; 
		ENUM ClassKey() _GET { return ClassVectorValue; }

	public:
		CVectorValue(_IN CFloat& rX, _IN CFloat& rY);
		CVectorValue(_IN CFloat& rX, _IN CFloat& rY, _IN CFloat& rZ);
		CVectorValue(_IN CFloat& rX, _IN CFloat& rY, _IN CFloat& rZ, _IN CFloat& rW);

		CVectorValue();
		virtual ~CVectorValue();

	public:
		CObject& At         (_IN UINT uiIndex) _GET throw(...);
		CObject& operator [](_IN UINT uiIndex) _GET throw(...);

		void AtPut(_IN UINT uiIndex, _IN _REF CValue& rValue) _SET throw(...);

	public:
		_PENDING void SortAscendingBubble();
		_PENDING void SortAscendingQuick();
		_PENDING void SortDescendingBubble();
		_PENDING void SortDescendingQuick();

	public:
		void AddLast(_IN _REF CValue& rValue);
		void RemoveLast() throw(...);

		void Remove(_IN UINT uiIndex) throw(...);
		void Delete(_IN UINT uiIndex) throw(...);

		void Remove(_IN CValue& rValue) throw(...);
		void Delete(_IN CValue& rValue) throw(...);

	public:
		//Find a given object in the  vector, the first performs an 
		//identity test, while the latter tests for equality of the objects
		CObject* Find     (_IN CValue& rValue) _GET throw(...);
		CObject* FindEqual(_IN CValue& rValue) _GET throw(...);

	public:
		void RemoveAll() throw(...);
		void DeleteAll() throw(...);

	public:
		inline UINT Size()    _GET { return m_uiCount; }
		inline bool IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
		
		void SerializeDependents  (_INOUT IStreamWriteObject& rStream) const;
		void DeserializeDependents(_INOUT IStreamReadObject&  rStream);

	protected:
		CValue** m_ppValues;
		UINT     m_uiCount;
	};
};
