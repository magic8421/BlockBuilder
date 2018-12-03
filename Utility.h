#pragma once

void InitDebugFont();
void DrawDebugText(int x, int y, DWORD color, const wchar_t *str);
void PrintOnScreen(const wchar_t *format, ...);
void RewindDebugPrint();


class FpsCounter
{
protected:
    FpsCounter() {}
    ~FpsCounter() {}

public:
    static FpsCounter *Instance();
    static void Free();

    void Feed();
    int Get();

private:
    static FpsCounter* m_inst;

    int m_prevTime = 0;
    int m_fps = 0;
    int m_prevFps = 0;
    DISALLOW_COPY_AND_ASSIGN(FpsCounter);
};

