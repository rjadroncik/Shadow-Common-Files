#pragma once 
#include "String.h"
#include "DictionaryInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CMemoryBlock : public CObjectSerializable
	{
		friend class OBJECT_EXTENSIONS_API CStreamMemory;
		friend class OBJECT_EXTENSIONS_API CStreamMemoryRead;
 		friend class OBJECT_EXTENSIONS_API CStreamMemoryWrite;

	public:
		SCF::ENUM ClassKey() _GET { return ClassMemoryBlock; }
		CString   ToString() _GET { return STRING("{MemoryBlock}"); }

	public:
		CMemoryBlock(_IN SCF::UINT uiBytes);
		virtual ~CMemoryBlock();

	public:
		static SCF::UINT   BlockCount();
		static SCF::UINT64 AllocatedBytes();

	public:
		void* operator [](_IN SCF::UINT uiByteOffset) _GET { return (SCF::BYTE*)m_vpData + uiByteOffset; }

	public:
		void Size(_IN SCF::UINT uiBytes) _SET;
		SCF::UINT Size()                 _GET { return m_uiSize; }
	
	public:
		void* Value() _GET { return m_vpData; }

		void Value(_IN CMemoryBlock& rMemoryBlock) _SET;
		void Value(_IN void* pMemory, _IN SCF::UINT uiBytes) _SET;

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
		
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const;
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream);

	protected:
		void* m_vpData;
		SCF::UINT m_uiSize;

	private:
		CMemoryBlock();
	};
};