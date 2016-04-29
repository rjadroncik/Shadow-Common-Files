#pragma once 
#include "String.h"
#include "DictionaryInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CMemoryBlock : public CObject
	{
		friend class OBJECT_EXTENSIONS_API CStreamMemory;
		friend class OBJECT_EXTENSIONS_API CStreamMemoryRead;
 		friend class OBJECT_EXTENSIONS_API CStreamMemoryWrite;

	public:
		CString ToString() _GET { return STRING("{MemoryBlock}"); }

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

	protected:
		void* m_vpData;
		SCF::UINT m_uiSize;

	private:
		CMemoryBlock();
	};
};