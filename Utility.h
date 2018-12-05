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
    static FpsCounter &Instance();

    void Feed();
    int Get();

private:
    int m_prevTime = 0;
    int m_fps = 0;
    int m_prevFps = 0;
    DISALLOW_COPY_AND_ASSIGN(FpsCounter);
};

class TimeManager
{
protected:
    TimeManager();
    ~TimeManager() {}

public:
    static TimeManager &Instance();

    void MakeTick();
    float GetTickDelta();

private:
    static TimeManager *m_inst;
    DWORD m_prevTick = 0;
    float m_delta = 0;
};
