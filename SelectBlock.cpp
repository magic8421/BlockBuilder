#include "StdAfx.h"
#include <limits>
#include "SelectBlock.h"
#include "camera.h"
#include "block.h"
#include "Chunk.h"

CSelectBlock::CSelectBlock(CCamera *camera)
{
	m_pCamera = camera;
	Reset();
}

CSelectBlock::~CSelectBlock(void)
{
}

void CSelectBlock::Reset() {
	m_pFound = NULL;
	m_tMin = std::numeric_limits<float>::infinity();
	m_face = -1;

	m_x = 0;
	m_y = 0;
	m_z = 0;
}

float operator *(const D3DXVECTOR3& lhs, const D3DXVECTOR3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// 从鼠标准星选取方框
bool CSelectBlock::TestBlockIntersection(CBlock *block, int x, int y, int z)
{
	//_RPT3(_CRT_WARN, "TestBlockIntersection %d,%d,%d\r\n", x, y, z);

	if (block->GetType() == CBlock::Air)
		return false;
	CCamera *pCamera = m_pCamera;
	D3DXVECTOR3 n, p1, p0, u, p;
	float size = 1.0f;
	bool bFound = false;

	for (int i = 0; i < 6; i ++)
	{
		u = *pCamera->GetViewVector() - *pCamera->GetCameraVector();
		p0 = *pCamera->GetCameraVector();
		
		p1.x = (float) x;
		p1.y = (float) y;
		p1.z = (float) z;
		if (i / 3 == 0)
		{
			//p1 = block->GetPos(); // 这个点在3个面上
		}
		else
		{
			//p1 = block->GetPos();
			p1.x += size;
			p1.y += size;
			p1.z += size;
		}
		switch (i % 3)
		{
		case 0:
			n = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
			break;
		case 1:
			n = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			break;
		case 2:
			n = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		}
		
		float t = ((n * p1) - (n * p0)) / (n * u);
		p = p0 + t * u;
		
		// 判断p是否在方块的区域内
		//float size = block->GetSize();
		switch (i)
		{
		case 0:
			bFound = p.z > p1.z && p.y > p1.y && p.z < p1.z + size && p.y < p1.y + size;
			break;
		case 1:
			bFound = p.z > p1.z && p.x > p1.x && p.z < p1.z + size && p.x < p1.x + size;
			break;
		case 2:
			bFound = p.y > p1.y && p.x > p1.x && p.y < p1.y + size && p.x < p1.x + size;
			break;
		case 3:
			bFound = p.z > p1.z - size && p.y > p1.y - size && p.z < p1.z && p.y < p1.y;
			break;
		case 4:
			bFound = p.z > p1.z - size && p.x > p1.x - size && p.z < p1.z && p.x < p1.x;
			break;
		case 5:
			bFound = p.y > p1.y - size && p.x > p1.x - size && p.y < p1.y && p.x < p1.x;
			break;
		}
		
		if (bFound)
		{
			//_RPT3 (_CRT_WARN, __FUNCTION__ " t: %f m_face:%d id:%d\r\n", t, i, block->GetID());
			if (t < m_tMin && t > 0.0f)
			{
				//_RPT2(_CRT_WARN, "t:%f t old:%f\r\n", t, m_tMin);
				m_tMin = t;
				m_pFound = block;
				m_face = i;

				m_x = x;
				m_y = y;
				m_z = z;
			}
		}
	} // for i
	return bFound;
}

void CSelectBlock::SelectBlockFromView(CChunk &chunk)
{
	Reset();

	// 先取得摄像机的位置 然后减去x, 扫描2x范围 (x取10左右吧) 其他算法不变
	const int SEARCH_RADIUS = 8;
	const D3DXVECTOR3 *pcv = m_pCamera->GetCameraVector();
	int x, y, z;
	int x2, y2, z2;
	x = (int)pcv->x - SEARCH_RADIUS;
	y = (int)pcv->y - SEARCH_RADIUS;
	z = (int)pcv->z - SEARCH_RADIUS;

	x2 = (int)pcv->x + SEARCH_RADIUS;
	y2 = (int)pcv->y + SEARCH_RADIUS;
	z2 = (int)pcv->z + SEARCH_RADIUS;

	x = max(0, x);
	x = min(CChunk::CHUNK_SIZE - 1, x);
	y = max(0, y);
	y = min(CChunk::CHUNK_SIZE - 1, y);
	z = max(0, z);
	z = min(CChunk::CHUNK_SIZE - 1, z);

	x2 = max(0, x2);
	x2 = min(CChunk::CHUNK_SIZE - 1, x2);
	y2 = max(0, y2);
	y2 = min(CChunk::CHUNK_SIZE - 1, y2);
	z2 = max(0, z2);
	z2 = min(CChunk::CHUNK_SIZE - 1, z2);

	int x3, y3, z3;
	for (z3 = z; z3 < z2; z3 ++)
	{
		for (y3 = y; y3 < y2; y3 ++)
		{
			for (x3 = x; x3 < x2; x3 ++)
			{
				TestBlockIntersection(chunk.GetAt(x3, y3, z3), x3, y3, z3);
			}
		}
	}
}

void CSelectBlock::GetPos( int &x, int &y, int &z )
{
	x = m_x;
	y = m_y;
	z = m_z;
}

D3DXVECTOR3 CSelectBlock::GetPos()
{
	D3DXVECTOR3 pos;
	pos.x = (float)m_x;
	pos.y = (float)m_y;
	pos.z = (float)m_z;
	return pos;
}
