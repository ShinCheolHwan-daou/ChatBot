#include <stdio.h>
#include "asset/asset.h"
#include "chatbot/chatbot.h"
#include "db/db.h"
#include "file/file.h"
#include "user/user.h"


int main() {
    if (user_login_page() == LOGIN_FAILED)
        return 1;
    while (1)
        if (user_run_main_page() == MAIN_FINISHED)
            break;

    return 0;
}
