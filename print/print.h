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

void print_color(int textColor, int bgColor, const char *format, ...);

int select_menu(const char *start_string, const char *menu[], int menu_size);

void print_clear();

void clear_input_buffer();

void print_enter();

#endif // COLOR_H
