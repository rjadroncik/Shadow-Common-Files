#include "StdAfx.h"
#include "Tree.h"

using namespace SCFObjectExtensions;
using namespace SCFPrivate;

CTree::CTree()
{
	m_pRoot = new CTreeNode();
}

CTree::~CTree()
{
	delete m_pRoot;
}
