#pragma once


class CBlock
{
public:
	CBlock(void);
	~CBlock(void);
	
	DWORD GetColor() {return m_color;}
	void SetColor(DWORD color);

	UINT GetType() {return m_type;}
	void SetType (UINT t) {m_type = t;}
	bool IsAir() {return m_type == Air;}

	enum {Air, Colored};

private:
	DWORD m_color;
	UINT m_type;
};
