#pragma once
#include "TreeNode.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CTree : public CObject
	{
	public:
		CTree();
		virtual ~CTree();

	public:
		CObject* ObjectAtRoot() _GET { return m_pRoot->Value(); }

	protected:
		SCFPrivate::CTreeNode* m_pRoot;
	};
};