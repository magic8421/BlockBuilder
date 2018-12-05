#include "stdafx.h"
#include "Utility.h"

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

static int printX = 0;
static int printY = 0;

void PrintOnScreen(const wchar_t *format, ...)
{
    const size_t SIZE = 1024 * 2;
    wchar_t buffer[SIZE];
    va_list varg;
    va_start(varg, format);
    ::StringCbVPrintfW(buffer, SIZE, format, varg);
    va_end(varg);

    HRESULT hr = S_OK;
    // Rectangle where the text will be located
    RECT TextRect = { printX, printY, 0, 0 };
    // Calculate the rectangle the text will occupy
    hr = pDebugFont->DrawText(NULL, buffer, -1, &TextRect, DT_CALCRECT, 0xffcccccc);
    hr = pDebugFont->DrawText(NULL, buffer, -1, &TextRect, DT_LEFT, 0xffcccccc);
    printY += TextRect.bottom - TextRect.top + 3;
}

void RewindDebugPrint()
{
    printX = 0;
    printY = 0;
}

FpsCounter &FpsCounter::Instance()
{
    static FpsCounter inst;
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

TimeManager::TimeManager()
{
    m_prevTick = ::GetTickCount();
}

TimeManager & TimeManager::Instance()
{
    static TimeManager mgr;
    return mgr;
}

void TimeManager::MakeTick()
{
    DWORD now = ::GetTickCount();
    m_delta = (float)(now - m_prevTick) / 16.666666f;
    m_prevTick = now;
}

float TimeManager::GetTickDelta()
{
    return (float)m_delta;
}

TimeManager * TimeManager::m_inst = nullptr;
