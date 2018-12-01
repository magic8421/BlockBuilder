#pragma once

void InitDebugFont();
void DrawDebugText(int x, int y, DWORD color, const wchar_t *str);

class FpsCounter
{
protected:
    FpsCounter() {}
    ~FpsCounter() {}

public:
    static FpsCounter *Instance();

    void Feed();
    int Get();

private:
    int m_prevTime = 0;
    int m_fps = 0;
    int m_prevFps = 0;
    DISALLOW_COPY_AND_ASSIGN(FpsCounter);
};
