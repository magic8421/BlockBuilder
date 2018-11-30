#include "stdafx.h"
#include "Utility.h"
#include <cassert>

extern LPDIRECT3DDEVICE9 g_d3ddev;

void DrawTextString(int x, int y, DWORD color, const wchar_t *str)
{

    HRESULT r = 0;


    // Get a handle for the font to use
    LPD3DXFONT pFont = 0;

    // Create the D3DX Font
    r = D3DXCreateFont(g_d3ddev, 16, 16, 0, 0, 0, 0, 0, 0, 0, L"ËÎÌו", &pFont);

    if (FAILED(r)) {
        assert(false);
        return;
    }
        // Rectangle where the text will be located
    RECT TextRect = { x, y, 0, 0 };

    // Calculate the rectangle the text will occupy
    pFont->DrawText(NULL, str, -1, &TextRect, DT_CALCRECT, color);

    // Output the text, left aligned
    pFont->DrawText(NULL, str, -1, &TextRect, DT_LEFT, color);
}