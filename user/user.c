//
// Created by daou__jaejin on 2025-03-25.
//
#include <conio.h> // For _getch() on Windows
#include <windows.h> // For system("cls") on Windows

#include "user.h"

#include "../chatbot/chatbot.h"
#include "../file/file.h"
#include "../db/db.h"


char g_chatbot_name[20] = "키우Me";
User *g_user_data = NULL;

void user_print_ascii_art() {
    printf("██╗  ██╗ ██╗ ██╗    ██╗  ██████╗   ██████╗  ███╗   ███╗ ███████╗\n");
    printf("██║ ██╔╝ ██║ ██║    ██║ ██╔═══██╗ ██╔═══██╗ ████╗ ████║ ██╔════╝\n");
    printf("█████╔╝  ██║ ██║ █╗ ██║ ██║   ██║ ██║   ██║ ██╔████╔██║ █████╗  \n");
    printf("██╔═██╗  ██║ ██║███╗██║ ██║   ██║ ██║   ██║ ██║╚██╔╝██║ ██╔══╝  \n");
    printf("██║  ██╗ ██║ ╚███╔███╔╝ ╚██████╔╝ ╚██████╔╝ ██║ ╚═╝ ██║ ███████╗\n");
    printf("╚═╝  ╚═╝ ╚═╝  ╚══╝╚══╝   ╚═════╝   ╚═════╝  ╚═╝     ╚═╝ ╚══════╝\n");
}

int user_login_page() {
    char user_id[CHAR_LEN];

    user_print_ascii_art();
    // printf("\n============================================\n");
    // printf("🔐 DAOU 자산관리 시스템에 오신 것을 환영합니다.\n");
    // printf("============================================\n");

    printf("%s) ID를 입력해주세요!\n>> ", g_chatbot_name);
    while (1) {
        scanf("%s", user_id);
        g_user_data = db_getUser(user_id);
        if (g_user_data == NULL) {
            printf("%s) ID와 일치하는 회원 정보가 없습니다.\n", g_chatbot_name);
            printf("%s) ID를 다시 입력해주세요!\n>> ", g_chatbot_name);
        } else {
            break;
        }
    }

    int life = 3;
    int i = 0;
    char ch;
    while (life--) {
        char password[CHAR_LEN] = {0};
        printf("%s) 비밀번호를 입력해주세요!\n>> ", g_chatbot_name);
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
            printf("%s) 감사합니다! 이제 DAOU 자산관리 시스템 시작합니다!\n", g_chatbot_name);
            getchar();
            printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
            getchar();
            return LOGIN_SUCCESS;
        } else {
            printf("%s) 비밀번호가 틀렸습니다. (남은 기회: %d번)\n", g_chatbot_name, life);
        }
    }
    printf("%s) 비밀번호를 3회 이상 틀리셔서 접근이 차단되었습니다.\n", g_chatbot_name);
    return LOGIN_FAILED;
}

const char *menu[] = {
    "[1] 나의 자산 현황 확인",
    "[2] 자산 현황 파일로 저장",
    "[3] 자산 조정",
    "[4] 챗봇과 자산관리 채팅",
    "[5] 과거 ai 자산관리 가이드 파일로 저장",
    "[6] 종료"
};
int selected_option = 0; // Tracks the currently selected menu option
const int menu_size = sizeof(menu) / sizeof(menu[0]);

void print_menu(int selected) {
    system("cls"); // Clear the console screen
    printf("\n=================<MENU>===============\n");
    for (int i = 0; i < menu_size; i++) {
        if (i == selected) {
            printf(">> %s <<\n", menu[i]); // Highlight the selected option
        } else {
            printf("   %s\n", menu[i]);
        }
    }
    printf("======================================\n");
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
