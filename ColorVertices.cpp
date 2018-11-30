#include "StdAfx.h"
#include "ColorVertices.h"
#include "Block.h"
#include "BlockBuilder.h"
#include "Chunk.h"

CColorVertices::CColorVertices()
{
	m_nBlocks = 0;
	m_indexCount = 0;
	m_indexMax = 0;
	m_index = 0;
	m_pVertices = NULL;
}

CColorVertices::~CColorVertices(void)
{
}

void CColorVertices::CreateBuffer(int nBlocks)
{
	m_nBlocks = nBlocks;
	CVertexBuffer::CreateBuffer(CUBE_VERTICES * nBlocks, sizeof(MyVertex), MY_FVF);
}

void CColorVertices::CreateIndexBuffer()
{
    // create the indices using an int array
    short indices[] =
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

    //g_d3ddev->CreateIndexBuffer(36*sizeof(short),
    //                          0,
    //                          D3DFMT_INDEX16,
    //                          D3DPOOL_MANAGED,
    //                          &m_indexBuff,
    //                          NULL);
	CVertexBuffer::CreateIndexBuffer(indices, sizeof(indices));
}

void CColorVertices::GenerateVertices(CChunk &chunk)
{
    VOID* pVoid;    // a void pointer

    // lock g_vertexBuff and load the vertices into it
    m_vertexBuff->Lock(0, 0, (void **)&pVoid, 0);
		m_index = 0;
		m_pVertices = static_cast<MyVertex *>(pVoid);

		for (int z = 0; z < CChunk::CHUNK_SIZE; z ++) {
			for (int y = 0; y < CChunk::CHUNK_SIZE; y ++) {
				for (int x = 0; x < CChunk::CHUNK_SIZE; x ++) {
					if (chunk.GetAt(x, y, z)->GetType() != CBlock::Air) {
						RenderBlock(chunk.GetAt(x, y, z), x, y , z);
					}
				}
			}
		}
		//chunk.TraverseBlocks(CBlockVertices::RenderBlock, this);
    m_vertexBuff->Unlock();
}

void CColorVertices::RenderBlock(CBlock *pBlock, int x, int y, int z)
{
// 	if (!pBlock)
// 		return;
		
	D3DXVECTOR3 pos;
	pos.x = (float)x;
	pos.y = (float)y;
	pos.z = (float)z;
	float size = 1.0f;
	DWORD color = pBlock->GetColor();
	MyVertex *pVert;
	
	if (m_index >= m_nBlocks)
	{
		_RPTF0(_CRT_ERROR, __FUNCTION__ "\r\n");
		return;
	}
		
	pVert = m_pVertices + m_index * CUBE_VERTICES;
	m_index ++;
	
	static const MyVertex cubePrototype[] =
    {
        { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, },    // side 1
        { 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
        { 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },
        { 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, },

        { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, },    // side 2
        { 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, },
        { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, },
        { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, },

        { 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },    // side 3
        { 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },
        { 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, },
        { 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, },

        { 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, },    // side 4
        { 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, },
        { 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, },
        { 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, },

        { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, },    // side 5
        { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, },
        { 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, },
        { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, },

        { 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, },    // side 6
        { 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
        { 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, },
        { 0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
    };
	for (int i = 0; i < CUBE_VERTICES; i ++)
	{
		pVert[i].x = cubePrototype[i].x * size + pos.x;
		pVert[i].y = cubePrototype[i].y * size + pos.y;
		pVert[i].z = cubePrototype[i].z * size + pos.z;
		pVert[i].normal = cubePrototype[i].normal;
		pVert[i].color = color;
	}
}

void CColorVertices::Draw()
{
	g_d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    g_d3ddev->SetFVF(MY_FVF);
    g_d3ddev->SetStreamSource(0, m_vertexBuff, 0, sizeof(MyVertex));
    g_d3ddev->SetIndices(m_indexBuff);

    // draw the cube
    for (int i = 0; i < m_nBlocks; i ++)
    {
		g_d3ddev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, i * CUBE_VERTICES, 0, CUBE_VERTICES, 0, 12); 
	}
}