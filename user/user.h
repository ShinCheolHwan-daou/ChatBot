//
// Created by daou__jaejin on 2025-03-25.
//

#ifndef USER_H
#define USER_H

#include <stdio.h>
#include <string.h>
#include "../asset/asset.h"


#define CHAR_LEN        255

#define MAIN_WORKING    0
#define MAIN_FINISHED   1

#define LOGIN_SUCCESS   0
#define LOGIN_FAILED    -1

typedef struct User {
    char user_id[CHAR_LEN];
    char name[CHAR_LEN];
    char password[CHAR_LEN];
} User;

extern char g_chatbot_name[20];
extern User* g_user_data;

void user_print_ascii_art();
int user_login_page();
int user_run_main_page();

#endif //USER_H
