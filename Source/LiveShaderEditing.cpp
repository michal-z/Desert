#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


bool
ShouldRecompileShaders()
{
    if ((GetAsyncKeyState('S') & 0x8000) && (GetAsyncKeyState(VK_CONTROL) & 0x8000))
    {
        Sleep(50);
        return true;
    }
    return false;
}

#else

bool
ShouldRecompileShaders()
{
    return false;
}

#endif
