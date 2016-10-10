#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	#define MAX_DEPTH_BAG_INT64 16

	class CSetNodeInt64
	{
	private:
		CSetNodeInt64();
		~CSetNodeInt64();

	public:
		//Used instead of constructors 
		static CSetNodeInt64* Create();

	public:
		//Recursive deletion, used to stop at level MAX_DEPTH_BAG_INT64
		static void Delete          (_IN CSetNodeInt64* pNode, _IN UINT uiLevel);
		static void DeleteWithObject(_IN CSetNodeInt64* pNode, _IN UINT uiLevel);

	public:
		inline CSetNodeInt64* SubNode(_IN UINT uiIndex)                              { return (CSetNodeInt64*)m_paSubNodes[uiIndex]; }
		inline void           SubNode(_IN UINT uiIndex, _IN CSetNodeInt64* pSubNode) { m_paSubNodes[uiIndex] = pSubNode; }

	private:
		 const CSetNodeInt64* m_paSubNodes[16]; 
	};
};