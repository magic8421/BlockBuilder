#pragma once

class CToolBox
{
public:
	CToolBox () {};
	~CToolBox() {};

	void ChooseColor();
	void StrawColor();
	void Save();
	void Load();
	void RemoveBlock();
	void AddBlock();

private:
	DISALLOW_COPY_AND_ASSIGN(CToolBox);
};