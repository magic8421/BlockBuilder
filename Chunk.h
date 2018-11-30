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

	// 生成一些测试方块
	void GenBlocks();

	// 获取需要渲染的块块数目 目前是"不是空气"的块块
	UINT GetRenderCount();

	// 在某一块旁边加上一块
	void AddBlock(int x, int y, int z, int face, DWORD color);

	// 删除一个方块(设置为空气)没有内存操作
	void RemoveBlock(int x, int y, int z);

	// 保存到文件 (整块写入)
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
