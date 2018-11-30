#include "StdAfx.h"
#include "Block.h"


CBlock::CBlock()
{
	m_color = 0xffcccccc;
	m_type = Air;
}

CBlock::~CBlock(void)
{
}

void CBlock::SetColor(DWORD color)
{
	m_color = color;
}
