#include "pch.h"
#include "xinput.h"

#pragma comment(lib, "XInput.lib") /// Library containing necessary 360

extern int inputDelay;
static const uint8_t delay = 15;

bool isInputPressed(const int input)
{
    DWORD dwResult;
    for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
    {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState(i, &state);

        if (dwResult == ERROR_SUCCESS)
        {
            // Controller is connected
            if (state.Gamepad.wButtons & input)
            {
                inputDelay = delay;
                return true;
            }
        }
    }

    return false;
}


bool isKeyPressed(char key)
{
    if (GetKeyState(key) & 0x8000)
    {
        inputDelay = delay;
        return true;
    }
}