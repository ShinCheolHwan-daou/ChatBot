#ifndef COLOR_H
#define COLOR_H

// 🎨 콘솔 색상 코드 (Windows 기준)
#define BLACK        0
#define BLUE         1
#define GREEN        2
#define AQUA         3
#define RED          4
#define PURPLE       5
#define YELLOW       6
#define WHITE        7
#define GRAY         8
#define LIGHT_BLUE   9
#define LIGHT_GREEN  10
#define LIGHT_AQUA   11
#define LIGHT_RED    12
#define LIGHT_PURPLE 13
#define LIGHT_YELLOW 14
#define BRIGHT_WHITE 15

// 색상을 지정하여 출력하는 함수 선언
void colorPrintf(int textColor, int bgColor, const char *format, ...);

#endif // COLOR_H
