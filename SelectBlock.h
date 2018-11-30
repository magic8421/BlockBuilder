#pragma once

class CBlock;
class CCamera;
class CChunk;

class CSelectBlock
{
public:
	CSelectBlock(CCamera *);
	~CSelectBlock(void);

	void SelectBlockFromView(CChunk &chunk);
	
	// 得到当前选择 如果没有选中则返回NULL
	CBlock *GetSelect() {return m_pFound;}
	
	// 得到选中的面 0-5
	int GetFace() {return m_face;}
	
	// 得到选择焦点与摄像机的距离
	float GetDistance() {return m_tMin;}

	// 得到选中方块的下标
	D3DXVECTOR3 GetPos();

	void GetPos(int &x, int &y, int &z);
	
	// 取消选择
	void Reset();

private:
	bool TestBlockIntersection(CBlock *block, int x, int y, int z);
	
private:
	CBlock *m_pFound;
	float m_tMin;
	int m_face;
	int m_x, m_y, m_z;
	CCamera *m_pCamera;
};
