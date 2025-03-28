#include "color.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#include <windows.h>

void setColor(int text, int background) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), background * 16 + text);
}

#else
void setColor(int text, int background) {
    printf("\033[%d;%dm", text + 30, background + 40);
}
#endif

void colorPrintf(int textColor, int bgColor, const char *format, ...) {
    setColor(textColor, bgColor);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

#ifdef _WIN32
    setColor(WHITE, BLACK); // 기본 색상 복구
#else
    printf("\033[0m"); // Linux/macOS 기본 색상 복구
#endif
}
