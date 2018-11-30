#pragma once

class CVertexBuffer
{
public:
	CVertexBuffer(void);
	virtual ~CVertexBuffer(void);
	
	// 创建顶点缓冲区 
	// nVertex 顶点的数量
	// cbVertex 每个顶点的字节数
	void CreateBuffer(int nVertex, int cbVertex, DWORD fvf);
	
	// 创建索引缓冲区
	// pIndex 输入索引数据
	// cbSize 索引数据总数（字节数）
	void CreateIndexBuffer(short *pIndex, int cbSize);
	
	virtual void Draw();
	
protected:
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuff;
	LPDIRECT3DINDEXBUFFER9 m_indexBuff;

private:
	DISALLOW_COPY_AND_ASSIGN(CVertexBuffer);
};
