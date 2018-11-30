#pragma once

class CPlayer
{
public:
	CPlayer() {}
	~CPlayer() {}

	D3DXVECTOR3 GetHeadPosition() {return m_headPos;}
	D3DXVECTOR3 GetFeetPosition();

private:
	D3DXVECTOR3 m_headPos;

private:
	DISALLOW_COPY_AND_ASSIGN(CPlayer);
};