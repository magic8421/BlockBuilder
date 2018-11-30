#pragma once

#include "VertexBuffer.h"

struct WireVertex {
	float x, y, z;
	DWORD color;
};

#define WIRE_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class CSelectVertices : public CVertexBuffer
{
public:
	CSelectVertices(void);
	virtual ~CSelectVertices(void);

	void CreateBuffer();
	void CreateIndexBuffer();
	void GenerateVertices(D3DXVECTOR3 pos, int face, DWORD color);
	void Draw();
	
private:
	DISALLOW_COPY_AND_ASSIGN(CSelectVertices);
};
