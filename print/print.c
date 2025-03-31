#include "print.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>


static void print_menu(int selected, const char *menu[], int menu_size);
static void user_print_ascii_art();

void setColor(int text, int background) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), background * 16 + text);
}

void print_color(int textColor, int bgColor, const char *format, ...) {
    setColor(textColor, bgColor);

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    setColor(WHITE, BLACK); // 기본 색상 복구
}

int select_menu(const char *start_str, const char *menu[], int menu_size) {
    int selected_option = 0;
    int key;

    while (true) {
        print_clear();
        if (start_str != NULL) {
            printf("%s\n\n", start_str);
        }
        print_menu(selected_option, menu, menu_size);
        printf("\n위/아래 방향키로 선택하고 Enter 키를 눌러 확인하세요.\n");
        key = _getch();

        if (key == 224) {
            // Arrow keys are preceded by 224 on Windows
            key = _getch(); // Get the actual key code
            if (key == 72) {
                // Up arrow key
                selected_option = (selected_option - 1 + menu_size) % menu_size;
            } else if (key == 80) {
                // Down arrow key
                selected_option = (selected_option + 1) % menu_size;
            }
        } else if (key == 13) {
            return selected_option;
        }
    }
}

static void print_menu(int selected, const char *menu[], int menu_size) {
    for (int i = 0; i < menu_size; i++) {
        if (i == selected) {
            printf(">> %s <<\n", menu[i]); // Highlight the selected option
        } else {
            printf("   %s\n", menu[i]);
        }
    }
}

void print_clear() {
    system("cls");
    user_print_ascii_art();
}

static void user_print_ascii_art() {
    int text_colors[] = {
        WHITE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_YELLOW,
        LIGHT_YELLOW
    };
    print_color(text_colors[0], BLACK, "██╗");
    print_color(LIGHT_PURPLE,BLACK, "  ██╗ ");
    print_color(text_colors[1], BLACK, "██╗ ");
    print_color(text_colors[2], BLACK, "██╗    ██╗ ");
    print_color(text_colors[3], BLACK, " ██████╗  ");
    print_color(text_colors[4], BLACK, " ██████╗  ");
    print_color(text_colors[5], BLACK, "███╗   ███╗ ");
    print_color(text_colors[6], BLACK, "███████╗\n");
    print_color(text_colors[0], BLACK, "██║");
    print_color(LIGHT_PURPLE,BLACK, " ██╔╝ ");
    print_color(text_colors[1], BLACK, "██║ ");
    print_color(text_colors[2], BLACK, "██║    ██║ ");
    print_color(text_colors[3], BLACK, "██╔═══██╗ ");
    print_color(text_colors[4], BLACK, "██╔═══██╗ ");
    print_color(text_colors[5], BLACK, "████╗ ████║ ");
    print_color(text_colors[6], BLACK, "██╔════╝\n");
    print_color(text_colors[0], BLACK, "███");
    print_color(LIGHT_PURPLE,BLACK, "██╔╝  ");
    print_color(text_colors[1], BLACK, "██║ ");
    print_color(text_colors[2], BLACK, "██║ █╗ ██║ ");
    print_color(text_colors[3], BLACK, "██║   ██║ ");
    print_color(text_colors[4], BLACK, "██║   ██║ ");
    print_color(text_colors[5], BLACK, "██╔████╔██║ ");
    print_color(text_colors[6], BLACK, "█████╗  \n");
    print_color(text_colors[0], BLACK, "██╔═██╗  ");
    print_color(text_colors[1], BLACK, "██║ ");
    print_color(text_colors[2], BLACK, "██║███╗██║ ");
    print_color(text_colors[3], BLACK, "██║   ██║ ");
    print_color(text_colors[4], BLACK, "██║   ██║ ");
    print_color(text_colors[5], BLACK, "██║╚██╔╝██║ ");
    print_color(text_colors[6], BLACK, "██╔══╝  \n");
    print_color(text_colors[0], BLACK, "██║  ██╗ ");
    print_color(text_colors[1], BLACK, "██║ ");
    print_color(text_colors[2], BLACK, "╚███╔███╔╝ ");
    print_color(text_colors[3], BLACK, "╚██████╔╝ ");
    print_color(text_colors[4], BLACK, "╚██████╔╝ ");
    print_color(text_colors[5], BLACK, "██║ ╚═╝ ██║ ");
    print_color(text_colors[6], BLACK, "███████╗\n");
    print_color(text_colors[0], BLACK, "╚═╝  ╚═╝ ");
    print_color(text_colors[1], BLACK, "╚═╝ ");
    print_color(text_colors[2], BLACK, " ╚══╝╚══╝  ");
    print_color(text_colors[3], BLACK, " ╚═════╝  ");
    print_color(text_colors[4], BLACK, " ╚═════╝  ");
    print_color(text_colors[5], BLACK, "╚═╝     ╚═╝ ");
    print_color(text_colors[6], BLACK, "╚══════╝\n\n");
}


void clear_input_buffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}