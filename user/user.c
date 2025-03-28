//
// Created by daou__jaejin on 2025-03-25.
//

#include "user.h"

#include "../chatbot/chatbot.h"
#include "../file/file.h"
#include "../db/db.h"


char g_chatbot_name[20] = "???";
User* g_user_data = NULL;

void user_print_ascii_art() {
    printf("     ／＞　 フ    $\n");
    printf("    | 　_　_|      $$\n");
    printf("  ／` ミ＿xノ      $\n");
    printf(" /　 づ $$ ⊃          \n");
    printf("/　  ｜　  ﾉ        \n");
    printf("│　  ｜｜｜           ___________________________\n");
    printf("／￣|　 ｜｜｜        |####=================####|\n");
    printf("(￣ヽ＿_ヽ_)__)      |#(4)****DAOU FITS****(4)#|\n");
    printf("＼二)                |*# {G}   < ^_^ >      #*|\n");
    printf("                    |##=====ONE DOLLAR======##|\n");
    printf("                    ---------------------------\n");

}

int user_login_page() {
    char user_id[CHAR_LEN];
    char password[CHAR_LEN];

    // user_print_ascii_art();
    printf("\n============================================\n");
    printf("DAOU 자산관리 시스템에 오신 것을 환영합니다.\n");
    printf("============================================\n");

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
    while (life--) {
        printf("%s) 비밀번호를 입력해주세요!\n>> ", g_chatbot_name);
        scanf("%s", password);
        if (strcmp(g_user_data->password, password) == 0) {
            printf("%s) %s님, 로그인이 완료되었습니다!\n", g_chatbot_name, g_user_data->name);
            printf("%s) 앗 참, 제 이름은 뭐로 할까요?\n>> ", g_chatbot_name);
            scanf("%s", g_chatbot_name);
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

int user_run_main_page() {
    int choice;
    printf("\n=================<MENU>===============\n");
    printf("[1] 나의 자산 현황\n");
    printf("[2] 자산 현황 파일로 저장\n");
    printf("[3] 자산 조정\n");
    printf("[4] 챗봇과 자산관리 채팅\n");
    printf("[5] 과거 ai 자산관리 가이드 출력\n");
    printf("[6] 종료\n");
    printf("======================================\n\n");
    printf("%s) 원하시는 메뉴를 선택해주세요!\n>> ", g_chatbot_name);
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            asset_print_asset();
            break;
        case 2:
            asset_save_asset();
            break;
        case 3:
            asset_modify_asset_amount();
            break;
        case 4:
            chatbot_chat();
            break;
        case 5:
            file_export_json();
            break;
        case 6:
            return MAIN_FINISHED;
        default:
            printf("%s) 유효하지 않은 선택지입니다.\n", g_chatbot_name);
            getchar();
            printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
            getchar();
    }
    return MAIN_WORKING;
}
