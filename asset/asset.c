//
// Created by daou__jaejin on 2025-03-25.
//

#include "asset.h"

const char *asset_type_strings[] = {
    "현금자산",
    "주식자산"
    //, "Crypto" // 추후
};


void asset_print_asset() {
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);

    printf("<<%s님의 자산 현황>>\n", g_user_data->name);
    printf("[현금자산] 총 %.2f원\n", asset_data[IDX_CASH].amount);
    printf("[주식자산] 총 %.2f원\n", asset_data[IDX_STOCK].amount);

    free(asset_data);
    getchar();
    printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

void asset_save_asset() {
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    int choice;

    printf("%s) 저장하실 파일 확장자를 선택해주세요!\n", g_chatbot_name);
    printf("\t1.bin\n\t2.csv\n\t3.text\n>>");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            save_binary(asset_data);
            break;
        case 2:
            save_csv(asset_data);
            break;
        case 3:
            save_text(asset_data);
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
    }

    free(asset_data);
    getchar();
    printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

static void save_csv(Asset *asset) {
    FILE *f = fopen("assets.csv", "w");
    fprintf(f, "자산유형,총액\n");
    for (int i = 0; i < TOTAL_ASSET_NUM; i++) {
        fprintf(f, "%s,%.2f\n",
                asset_type_strings[i],
                asset[i].amount
        );
    }
    fclose(f);
    printf("%s) csv 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_binary(Asset *asset) {
    FILE *f = fopen("assets.dat", "wb");

    for (int i = 0; i < TOTAL_ASSET_NUM; i++) {
        fwrite(asset_type_strings[i], sizeof(asset_type_strings[i]), 1, f);
        fwrite(&asset[i].amount, sizeof(asset[i].amount), 1, f);
    }

    fclose(f);
    printf("%s) binary 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_text(Asset *asset) {
    FILE *f = fopen("assets.txt", "w");
    fprintf(f, "<<%s님의 자산 현황>>\n", g_user_data->name);
    for (int i = 0; i < TOTAL_ASSET_NUM; i++) {
        fprintf(f, "[%s] 총 %.2f원\n",
                asset_type_strings[i],
                asset[i].amount
        );
    }
    fclose(f);
    printf("%s) text 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

void asset_modify_asset_amount() {
    int asset_kind;

    printf("%s) 어떤 자산을 조정할까요?\n", g_chatbot_name);
    printf("\t1.현금자산\n\t2.주식자산\n>>");
    scanf("%d", &asset_kind);
    switch (asset_kind) {
        case 1:
            modify_cash();
            break;
        case 2:
            modify_stock();
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
    }

    getchar();
    printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

static void modify_cash() {
    int asset_method;
    double amount;

    printf("%s) 현금자산을 어떻게 조정할까요?\n", g_chatbot_name);
    printf("\t1.입금\n\t2.출금\n>>");
    scanf("%d", &asset_method);
    switch (asset_method) {
        case 1:
            asset_method = 1;
            printf("%s) 입금할 금액을 알려주세요!\n>>", g_chatbot_name);
            scanf("%lf", &amount);
            break;
        case 2:
            asset_method = -1;
            printf("%s) 출금할 금액을 알려주세요!\n>>", g_chatbot_name);
            scanf("%lf", &amount);
            Asset *asset_data = db_getUserAsset(g_user_data->user_id);
            if (asset_data[IDX_CASH].amount < amount) {
                printf("%s) 출금할 금액이 잔액보다 많아요!\n", g_chatbot_name);
                printf("%s) 남은 현금 잔액: %.2f원\n", g_chatbot_name, asset_data[IDX_CASH].amount);
                free(asset_data);
                return;
            }
            free(asset_data);
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            return;
    }
    // [TODO] 현금자산 DB에 동기화하는 함수 호출
    // db_updateAsset(g_user_data->user_id, IDX_CASH, amount * asset_method);
    printf("%s) 현금자산 조정이 완료되었습니다!\n", g_chatbot_name);
}

static void modify_stock() {
    int asset_method;
    char stock_name[255];
    int quantity;
    double price;

    printf("%s) 주식자산을 어떻게 조정할까요?\n", g_chatbot_name);
    printf("\t1.매수\n\t2.매도\n>>");
    scanf("%d", &asset_method);
    switch (asset_method) {
        case 1:
            asset_method = 1;
            printf("%s) 매수할 종목의 이름을 알려주세요!\n>>", g_chatbot_name);
            scanf("%s", stock_name);
        // [TODO] 해당 종목의 이름을 가진 정보가 STOCK 테이블에 있는지 확인
            // if (db_check_stock_name(stock_name) == false) {
            //     printf("%s) %s 주식의 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
            //            g_chatbot_name, stock_name);
            //     return;
            // }
            printf("%s) 매수할 종목의 개수를 알려주세요!\n>>", g_chatbot_name);
            scanf("%d", &quantity);
            printf("%s) 매수할 종목의 평단가를 알려주세요!\n>>", g_chatbot_name);
            scanf("%lf", &price);
            break;
        case 2:
            asset_method = -1;
            printf("%s) 매도할 종목의 이름을 알려주세요!\n>>", g_chatbot_name);
            scanf("%s", stock_name);
        // [TODO] 해당 종목의 이름을 가진 정보가 STOCK 테이블에 있는지 확인
            // if (db_check_stock_name(stock_name) == false) {
            //     printf("%s) %s 주식의 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
            //            g_chatbot_name, stock_name);
            //     return;
            // }
            printf("%s) 매도할 종목의 개수를 알려주세요!\n>>", g_chatbot_name);
            scanf("%d", &quantity);
            printf("%s) 매도할 종목의 평단가를 알려주세요!\n>>", g_chatbot_name);
            scanf("%lf", &price);
        // [TODO] 해당 종목의 User_Stock 정보 db에서 받아오기
            User_Stock *asset_data = NULL; // = db_getUserStock(g_user_data->name, stock_name);
            if (asset_data == NULL || asset_data[IDX_STOCK].quantity < quantity) {
                printf("%s) 매도할 보유주식이 적어요!\n", g_chatbot_name);
                printf("%s) 현재 %s 보유주식: %d개\n",
                       g_chatbot_name,
                       stock_name,
                       asset_data == NULL ? 0 : asset_data[IDX_STOCK].quantity);
                free(asset_data);
                return;
            }
            free(asset_data);
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            return;
    }
    // [TODO] ASSET 테이블 주식자산 DB에 동기화하는 함수 호출
    // db_updateAsset(g_user_data->user_id, IDX_STOCK, price * quantity * asset_method);
    // [TODO] USER_STOCK 테이블 해당 종목 보유정보 DB에 동기화하는 함수 호출
    // db_updateUserStock(g_user_data->user_id, stock_name, quantity, price);
    printf("%s) 주식자산 조정이 완료되었습니다!\n", g_chatbot_name);
}
