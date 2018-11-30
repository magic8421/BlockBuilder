#pragma once

#include "VertexBuffer.h"

class CBlock;
class CChunk;


// http://msdn.microsoft.com/zh-cn/library/bb172559(v=vs.85).aspx
struct MyVertex {
	float x, y, z;
	D3DVECTOR normal;
	DWORD color;
};
#define MY_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE)


class CColorVertices : public CVertexBuffer
{
public:
	CColorVertices();
	virtual ~CColorVertices(void);
	
	void CreateBuffer(int nBlocks);
	void CreateIndexBuffer();
	void GenerateVertices(CChunk &chunk);
	void RenderBlock(CBlock *pBlock, int x, int y, int z);
	void Draw();
	
	static const int CUBE_VERTICES = 24;
	
private:
	CColorVertices(const CColorVertices&);
	void operator=(const CColorVertices&);

	int m_nBlocks;
	int m_indexCount;
	int m_indexMax;
	
	// ��ǰ������λ�ã����������㻺�����С�
	int m_index;
	// ���㻺������ʼλ��
	MyVertex *m_pVertices;
};
