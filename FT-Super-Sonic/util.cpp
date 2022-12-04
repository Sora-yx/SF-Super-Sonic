#include "pch.h"

HANDLE stdoutHandle = nullptr;

void PrintInfo(const char* text, ...)
{
    va_list ap;
    va_start(ap, text);

    if (!stdoutHandle)
        stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    char buffer[512];
    _vsprintf_p(buffer, 512, text, ap);
    printf("%s%s\n", "[Super Sonic]: ", buffer);
    va_end(ap);
}