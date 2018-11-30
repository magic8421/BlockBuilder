#pragma once

class CVertexBuffer
{
public:
	CVertexBuffer(void);
	virtual ~CVertexBuffer(void);
	
	// �������㻺���� 
	// nVertex ���������
	// cbVertex ÿ��������ֽ���
	void CreateBuffer(int nVertex, int cbVertex, DWORD fvf);
	
	// ��������������
	// pIndex ������������
	// cbSize ���������������ֽ�����
	void CreateIndexBuffer(short *pIndex, int cbSize);
	
	virtual void Draw();
	
protected:
	LPDIRECT3DVERTEXBUFFER9 m_vertexBuff;
	LPDIRECT3DINDEXBUFFER9 m_indexBuff;

private:
	DISALLOW_COPY_AND_ASSIGN(CVertexBuffer);
};
