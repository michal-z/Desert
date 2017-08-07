#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


int ShouldRecompileShaders(void)
{
    if ((GetAsyncKeyState('S') & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
    {
        Sleep(50);
        return 1;
    }
    return 0;
}

#else

int ShouldRecompileShaders(void)
{
    return 0;
}

#endif
