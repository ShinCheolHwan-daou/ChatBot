//
// Created by daou__jaejin on 2025-03-25.
//
#include <conio.h> // For _getch() on Windows
#include <windows.h> // For system("cls") on Windows

#include "user.h"

#include "../chatbot/chatbot.h"
#include "../color/color.h"
#include "../file/file.h"
#include "../db/db.h"


char g_chatbot_name[20] = "키우Me";
User *g_user_data = NULL;

void user_print_ascii_art() {
    int text_colors[] = {
        WHITE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_BLUE,
        LIGHT_YELLOW,
        LIGHT_YELLOW
    };
    colorPrintf(text_colors[0], BLACK, "██╗"); colorPrintf(LIGHT_PURPLE,BLACK, "  ██╗ ");colorPrintf(text_colors[1], BLACK, "██╗ ");colorPrintf(text_colors[2], BLACK, "██╗    ██╗ ");colorPrintf(text_colors[3], BLACK, " ██████╗  ");colorPrintf(text_colors[4], BLACK, " ██████╗  ");colorPrintf(text_colors[5], BLACK, "███╗   ███╗ ");colorPrintf(text_colors[6], BLACK, "███████╗\n");
    colorPrintf(text_colors[0], BLACK, "██║"); colorPrintf(LIGHT_PURPLE,BLACK, " ██╔╝ ");colorPrintf(text_colors[1], BLACK, "██║ ");colorPrintf(text_colors[2], BLACK, "██║    ██║ ");colorPrintf(text_colors[3], BLACK, "██╔═══██╗ ");colorPrintf(text_colors[4], BLACK, "██╔═══██╗ ");colorPrintf(text_colors[5], BLACK, "████╗ ████║ ");colorPrintf(text_colors[6], BLACK, "██╔════╝\n");
    colorPrintf(text_colors[0], BLACK, "███"); colorPrintf(LIGHT_PURPLE,BLACK, "██╔╝  ");colorPrintf(text_colors[1], BLACK, "██║ ");colorPrintf(text_colors[2], BLACK, "██║ █╗ ██║ ");colorPrintf(text_colors[3], BLACK, "██║   ██║ ");colorPrintf(text_colors[4], BLACK, "██║   ██║ ");colorPrintf(text_colors[5], BLACK, "██╔████╔██║ ");colorPrintf(text_colors[6], BLACK, "█████╗  \n");
    colorPrintf(text_colors[0], BLACK, "██╔═██╗  ");colorPrintf(text_colors[1], BLACK, "██║ ");colorPrintf(text_colors[2], BLACK, "██║███╗██║ ");colorPrintf(text_colors[3], BLACK, "██║   ██║ ");colorPrintf(text_colors[4], BLACK, "██║   ██║ ");colorPrintf(text_colors[5], BLACK, "██║╚██╔╝██║ ");colorPrintf(text_colors[6], BLACK, "██╔══╝  \n");
    colorPrintf(text_colors[0], BLACK, "██║  ██╗ ");colorPrintf(text_colors[1], BLACK, "██║ ");colorPrintf(text_colors[2], BLACK, "╚███╔███╔╝ ");colorPrintf(text_colors[3], BLACK, "╚██████╔╝ ");colorPrintf(text_colors[4], BLACK, "╚██████╔╝ ");colorPrintf(text_colors[5], BLACK, "██║ ╚═╝ ██║ ");colorPrintf(text_colors[6], BLACK, "███████╗\n");
    colorPrintf(text_colors[0], BLACK, "╚═╝  ╚═╝ ");colorPrintf(text_colors[1], BLACK, "╚═╝ ");colorPrintf(text_colors[2], BLACK, " ╚══╝╚══╝  ");colorPrintf(text_colors[3], BLACK, " ╚═════╝  ");colorPrintf(text_colors[4], BLACK, " ╚═════╝  ");colorPrintf(text_colors[5], BLACK, "╚═╝     ╚═╝ ");colorPrintf(text_colors[6], BLACK, "╚══════╝\n");
}

int user_login_page() {
    char user_id[CHAR_LEN];

    user_print_ascii_art();
    // todo:: [재진] 사용설명
    // printf("\n============================================\n");
    // printf("🔐 DAOU 자산관리 시스템에 오신 것을 환영합니다.\n");
    // printf("============================================\n");

    printf("\n%s)\t😊 안녕하세요! 저는 ai 챗봇 키우ME 입니다!\n", g_chatbot_name);
    printf("\t✅ 저는 고객님의 자산관리를 도와주며,\n");
    printf("\t✅ 다양한 주제에 대해 대답해 드리는 역할을 합니다.\n", g_chatbot_name);
    printf("\n(Enter를 눌러 계속...)");
    getchar();
    system("cls"); // Clear the console screen

    printf("%s)\t 고객님의 ID를 입력해주세요!\n>> ", g_chatbot_name);
    while (1) {
        scanf("%s", user_id);
        g_user_data = db_getUser(user_id);
        if (g_user_data == NULL) {
            printf("\n%s)\t❌ ID와 일치하는 회원 정보가 없습니다.\n", g_chatbot_name);
            printf("%s)\tID를 다시 입력해주세요!\n>> ", g_chatbot_name);
        } else {
            break;
        }
    }
    printf("\n");

    int life = 3;
    int i = 0;
    char ch;
    while (life--) {
        char password[CHAR_LEN] = {0};
        printf("%s)\t 비밀번호를 입력해주세요!\n>> ", g_chatbot_name);
        while (1) {
            ch = getch(); // 키 입력 받기 (화면에 표시되지 않음)
            if (ch == '\r') {
                // Enter 키를 누르면 종료
                password[i] = '\0'; // 문자열 끝에 NULL 추가
                break;
            } else if (ch == '\b' && i > 0) {
                // Backspace 처리
                printf("\b \b"); // 화면에서 지우기
                i--;
            } else if (ch != '\b') {
                // 일반 문자 처리
                password[i++] = ch;
                printf("*"); // 화면에 '*' 출력
            }
        }
        printf("\n");

        if (strcmp(g_user_data->password, password) == 0) {
            getchar();
            printf("\n%s)\t✨ 환영합니다 %s 고객님!\n", g_chatbot_name, g_user_data->name);
            printf("\t키우ME 서비스를 시작합니다~\n");
            printf("\n(Enter를 눌러 계속...)\n");
            getchar();
            return LOGIN_SUCCESS;
        } else {
            printf("\n%s)\t❌ 비밀번호가 틀렸습니다. (남은 기회: %d번)\n", g_chatbot_name, life);
        }
    }
    printf("%s)\t😭 비밀번호를 3회 이상 틀리셔서 접근이 차단되었습니다.\n", g_chatbot_name);
    getchar();
    printf("\n(Enter를 눌러 나가기...)\n");
    getchar();
    return LOGIN_FAILED;
}

// todo:: [지현] 워딩정리
const char *menu[] = {
    "📊 내 자산 한눈에 보기",
    "💾 내 자산 저장하기",
    "✏️ 내 자산 수정하기",
    "💬 AI 챗봇과 대화하기",
    "📚 AI 추천 가이드 저장하기",
    "🚪 종료하기"
};

int selected_option = 0; // Tracks the currently selected menu option
const int menu_size = sizeof(menu) / sizeof(menu[0]);

void print_menu(int selected) {
    system("cls"); // Clear the console screen
    printf("\n=================<MENU>===============\n\n");
    for (int i = 0; i < menu_size; i++) {
        if (i == selected) {
            printf(">> %s <<\n", menu[i]); // Highlight the selected option
        } else {
            printf("   %s\n", menu[i]);
        }
    }
    printf("\n======================================\n");
}

int user_run_main_page() {
    int key;
    while (1) {
        print_menu(selected_option);
        printf("\n%s) 위/아래 방향키로 선택하고 Enter 키를 눌러 확인하세요.\n", g_chatbot_name);

        key = _getch(); // Get keyboard input without requiring Enter

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
            // Enter key
            switch (selected_option) {
                case 0:
                    system("cls");
                    asset_print_asset();
                    break;
                case 1:
                    system("cls");
                    asset_save_asset();
                    break;
                case 2:
                    system("cls");
                    asset_modify_asset_amount();
                    break;
                case 3:
                    system("cls");
                    chatbot_chat();
                    break;
                case 4:
                    system("cls");
                    file_export_json();
                    break;
                case 5:
                    return MAIN_FINISHED;
            }
        }
    }
    return MAIN_WORKING;
}
