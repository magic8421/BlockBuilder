#pragma once

#include "atlbase.h"
#include "Block.h"

typedef void (*TraverseBlockFn)(CBlock *block, int index, int level, void *udata);

class CChunk
{
public:
	CChunk(void);
	~CChunk(void);
	
	CBlock *GetAt(int x, int y, int z);

	// ����һЩ���Է���
	void GenBlocks();

	// ��ȡ��Ҫ��Ⱦ�Ŀ����Ŀ Ŀǰ��"���ǿ���"�Ŀ��
	UINT GetRenderCount();

	// ��ĳһ���Ա߼���һ��
	void AddBlock(int x, int y, int z, int face, DWORD color);

	// ɾ��һ������(����Ϊ����)û���ڴ����
	void RemoveBlock(int x, int y, int z);

	// ���浽�ļ� (����д��)
	bool SaveToFile(LPCTSTR pFileName);

	//
	bool LoadFromFile(LPCTSTR pFileName);
	bool CollisionTest(D3DXVECTOR3 pos, D3DXVECTOR3& speed, int id);

public:
	// 
	enum {CHUNK_SIZE = 128};

private:
	CBlock m_blocks[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
};
