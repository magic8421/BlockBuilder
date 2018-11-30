#include "StdAfx.h"
#include "Chunk.h"
#include "block.h"

CChunk::CChunk(void)
{
// 	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i ++)
// 	{
// 		m_blocks[i].
// 	}
}

CChunk::~CChunk(void)
{
}

void CChunk::GenBlocks()
{
	// 生成一个平面 作为基础
	for (int z = 0; z < 32; z ++)
	{
		for (int x = 0; x < 32; x ++)
		{
			GetAt(x, 0, z)->SetColor(D3DCOLOR_XRGB(127,127,127));
			GetAt(x, 0, z)->SetType(CBlock::Colored);
		}
	}
}

UINT CChunk::GetRenderCount()
{
	UINT count = 0;

	for (int z = 0; z < CHUNK_SIZE; z ++)
	{
		for (int y = 0; y < CHUNK_SIZE; y ++)
		{
			for (int x = 0; x < CHUNK_SIZE; x ++)
			{
				if (GetAt(x, y, z)->GetType() != CBlock::Air)
				{
					count ++;
				}
			}
		}
	}
	return count;
}

CBlock * CChunk::GetAt( int x, int y, int z )
{
	//return &m_blocks[z * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + x]; // 这个坐标系是怎么样的还有待商讨
	if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE)
	{
		return NULL;
	}
	else
	{
		return &m_blocks[(z << 14) + (y << 7) + x];
	}
}

void CChunk::AddBlock( int x, int y, int z, int face, DWORD color )
{
	switch (face)
	{
	case 0:
		x --;
		break;
	case 1:
		y --;
		break;
	case 2:
		z --;
		break;
	case 3:
		x ++;
		break;
	case 4:
		y ++;
		break;
	case 5:
		z ++;
		break;
	}

	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
		return;
	GetAt(x, y, z)->SetType(CBlock::Colored);
	GetAt(x, y, z)->SetColor(color);
}

void CChunk::RemoveBlock( int x, int y, int z )
{
	GetAt(x, y, z)->SetType(CBlock::Air);
}

bool CChunk::SaveToFile( LPCTSTR pFileName )
{
	FILE *fp = NULL;
	bool bRet = false;
	_wfopen_s(&fp, pFileName, L"wb");
	if (fp)
	{
		size_t c = fwrite(&m_blocks, 1, sizeof(m_blocks), fp);
		if (c == sizeof(m_blocks))
		{
			bRet = true;
		}
		fclose(fp);
	}
	return bRet;
}

bool CChunk::LoadFromFile( LPCTSTR pFileName )
{
	FILE *fp = NULL;
	bool bRet = false;
	_wfopen_s(&fp, pFileName, L"rb");
	if (fp)
	{
		int c = fread(&m_blocks, 1, sizeof(m_blocks), fp);
		if (c == sizeof(m_blocks))
		{
			bRet = true;
		}
		fclose(fp);
	}
	return bRet;
}

bool CChunk::CollisionTest( D3DXVECTOR3 pos, D3DXVECTOR3& speed, int id )
{
	// 前面的想法都是错的 应该用包围盒做这个东西
	// 这个函数可能要调用8次 对8个顶点都求碰撞
	// 2013-12-14 1315 还是有问题 为什么脚上1个方块会被"跨过去"
	int x = int(pos.x);
	int y = int(pos.y);
	int z = int(pos.z);

	CBlock *block = NULL;
// 	block = GetAt(x, y, z);
// 	if (block && block->GetType() != CBlock::Air)
// 	{
// 		return false;
// 	}
	if (speed.x > 0.0f && (id & 1))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.x = 0.0f;
			}
		}
	}
	if (speed.x < 0.0f && !(id & 1))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.x = 0.0f;
			}
		}
	}

	if (speed.y > 0.0f && (id & 2))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.y = 0.0f;
			}
		}
	}
	if (speed.y < 0.0f && !(id & 2))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.y = 0.0f;
			}
		}
	}

	if (speed.z > 0.0f && (id & 4))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.z = 0.0f;
			}
		}
	}
	if (speed.z < 0.0f && !(id & 4))
	{
		block = GetAt(x, y, z);
		if (block)
		{
			if (!block->IsAir())
			{
				speed.z = 0.0f;
			}
		}
	}
	return true;
}

