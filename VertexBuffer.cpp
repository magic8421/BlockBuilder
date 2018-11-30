#include "StdAfx.h"
#include "BlockBuilder.h"
#include "VertexBuffer.h"

CVertexBuffer::CVertexBuffer(void)
{
	
	m_vertexBuff = NULL;
	m_indexBuff = NULL;
}

CVertexBuffer::~CVertexBuffer(void)
{
	if (m_vertexBuff)
		m_vertexBuff->Release();
	if (m_indexBuff)
		m_indexBuff->Release();
}

void CVertexBuffer::CreateBuffer(int nVertex, int cbVertex, DWORD fvf)
{
	if (m_vertexBuff)
	{	
		m_vertexBuff->Release();
		m_vertexBuff = NULL;
	}
    HRESULT hr = g_d3ddev->CreateVertexBuffer(nVertex * cbVertex,
		0, fvf, D3DPOOL_MANAGED, &m_vertexBuff, NULL);
	if (FAILED(hr))
	{
		_RPTF0(_CRT_ERROR, __FUNCTION__ "\r\n");
	}
}

void CVertexBuffer::CreateIndexBuffer(short *pIndex, int cbSize)
{
	if (m_indexBuff)
	{
		m_indexBuff->Release();
		m_indexBuff = NULL;
	}
    // create an index buffer interface called g_indexBuff
    HRESULT hr = g_d3ddev->CreateIndexBuffer(cbSize,
                              0,
                              D3DFMT_INDEX16,
                              D3DPOOL_MANAGED,
                              &m_indexBuff,
                              NULL);
	if (FAILED(hr))
	{
		_RPTF0(_CRT_ERROR, __FUNCTION__ "\r\n");
	}
	void **pVoid;
    // lock g_indexBuff and load the indices into it
    m_indexBuff->Lock(0, 0, (void**)&pVoid, 0);
    memcpy(pVoid, pIndex, cbSize);
    m_indexBuff->Unlock();
}

void CVertexBuffer::Draw()
{
	_RPTF0(_CRT_ERROR, __FUNCTION__ "\r\n");
}
