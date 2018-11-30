#include "StdAfx.h"
#include "BlockBuilder.h"
#include "SelectVertices.h"
#include "VertexBuffer.h"

CSelectVertices::CSelectVertices(void)
{
}

CSelectVertices::~CSelectVertices(void)
{
}

void CSelectVertices::CreateBuffer()
{
	CVertexBuffer::CreateBuffer(4, sizeof(WireVertex), WIRE_FVF);
}

void CSelectVertices::CreateIndexBuffer()
{
    short indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};
	CVertexBuffer::CreateIndexBuffer(indices, sizeof(indices));
}

void CSelectVertices::GenerateVertices( D3DXVECTOR3 pos, int face, DWORD color )
{
	static const WireVertex wirePrototype[6][4] = {
		{
			{0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)}
		},
		{
			{0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)}
		},
		{
			{0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)}
		},
		{
			{1.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)}
		},
		{
			{0.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)}
		},
		{
			{0.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 0.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{1.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)},
			{0.0f, 1.0f, 1.0f, D3DCOLOR_XRGB(255, 255, 255)}
		}
	};

	WireVertex *pVert;
	m_vertexBuff->Lock(0, 0, (void **)&pVert, 0);
	for (int i = 0; i < 4; i ++) 
	{
		pVert->x = pos.x + wirePrototype[face][i].x;
		pVert->y = pos.y + wirePrototype[face][i].y;
		pVert->z = pos.z + wirePrototype[face][i].z;
		pVert->color = color; // 这个注释掉之后居然是红色的线 现在问题就是线是黑的，想弄成别的颜色。
		pVert ++;
	}
	m_vertexBuff->Unlock();
}

void CSelectVertices::Draw()
{
	g_d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_d3ddev->SetFVF(WIRE_FVF);
    g_d3ddev->SetStreamSource(0, m_vertexBuff, 0, sizeof(WireVertex));
    g_d3ddev->SetIndices(m_indexBuff);
	g_d3ddev->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, 4, 0, 4); 
}
