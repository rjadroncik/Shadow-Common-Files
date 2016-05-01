#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFPrivate
{
	#define MAX_DEPTH_BAG_INT16 4

	class CBagNodeInt16
	{
	private:
		CBagNodeInt16();
		~CBagNodeInt16();

	public:
		//Used instead of constructors 
		static CBagNodeInt16* Create();

	public:
		//Recursive deletion, used to stop at level MAX_DEPTH_BAG_INT16
		static void Delete(_IN CBagNodeInt16* pNode, _IN UINT uiLevel);

	public:
		inline CBagNodeInt16* SubNode(_IN USHORT usIndex)                              { return (CBagNodeInt16*)m_paSubNodes[usIndex]; }
		inline void           SubNode(_IN USHORT usIndex, _IN CBagNodeInt16* pSubNode) { m_paSubNodes[usIndex] = pSubNode; }

	private:
		 const CBagNodeInt16* m_paSubNodes[16]; 
	};
};