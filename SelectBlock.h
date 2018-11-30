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
	
	// �õ���ǰѡ�� ���û��ѡ���򷵻�NULL
	CBlock *GetSelect() {return m_pFound;}
	
	// �õ�ѡ�е��� 0-5
	int GetFace() {return m_face;}
	
	// �õ�ѡ�񽹵���������ľ���
	float GetDistance() {return m_tMin;}

	// �õ�ѡ�з�����±�
	D3DXVECTOR3 GetPos();

	void GetPos(int &x, int &y, int &z);
	
	// ȡ��ѡ��
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
