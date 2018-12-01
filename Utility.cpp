#include "stdafx.h"
#include "Utility.h"
#include <cassert>

extern LPDIRECT3DDEVICE9 g_d3ddev;

static LPD3DXFONT pDebugFont = 0;

void InitDebugFont()
{
    if (pDebugFont) {
        assert(false);
        return;
    }
    HRESULT hr = S_OK;

    // Create the D3DX Font
    hr = D3DXCreateFont(g_d3ddev, 16, 8, 0, 0, 0, 0, 0, 0, 0, L"ËÎÌו", &pDebugFont);

    if (FAILED(hr)) {
        assert(false);
        return;
    }
}

void DrawDebugText(int x, int y, DWORD color, const wchar_t *str)
{
    if (!pDebugFont) {
        assert(false);
        return;
    }
    HRESULT hr = S_OK;
        // Rectangle where the text will be located
    RECT TextRect = { x, y, 0, 0 };

    // Calculate the rectangle the text will occupy
    pDebugFont->DrawText(NULL, str, -1, &TextRect, DT_CALCRECT, color);

    // Output the text, left aligned
    pDebugFont->DrawText(NULL, str, -1, &TextRect, DT_LEFT, color);
}

FpsCounter * FpsCounter::Instance()
{
    static FpsCounter* inst = nullptr;
    if (!inst) {
        inst = new FpsCounter;
    }
    return inst;
}

void FpsCounter::Feed()
{
    m_fps++;
    int time = (int)::GetTickCount() / 1000;
    if (time != m_prevTime) {
        m_prevTime = time;
        m_prevFps = m_fps;
        m_fps = 0;
    }
}

int FpsCounter::Get()
{
    return m_prevFps;
}
