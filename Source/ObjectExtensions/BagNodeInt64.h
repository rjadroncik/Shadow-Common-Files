#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	#define MAX_DEPTH_BAG_INT64 16

	class CBagNodeInt64
	{
	private:
		CBagNodeInt64();
		~CBagNodeInt64();

	public:
		//Used instead of constructors 
		static CBagNodeInt64* Create();

	public:
		//Recursive deletion, used to stop at level MAX_DEPTH_BAG_INT64
		static void Delete          (_IN CBagNodeInt64* pNode, _IN UINT uiLevel);
		static void DeleteWithObject(_IN CBagNodeInt64* pNode, _IN UINT uiLevel);

	public:
		inline CBagNodeInt64* SubNode(_IN UINT uiIndex)                              { return (CBagNodeInt64*)m_paSubNodes[uiIndex]; }
		inline void           SubNode(_IN UINT uiIndex, _IN CBagNodeInt64* pSubNode) { m_paSubNodes[uiIndex] = pSubNode; }

	private:
		 const CBagNodeInt64* m_paSubNodes[16]; 
	};
};