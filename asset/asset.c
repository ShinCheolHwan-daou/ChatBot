//
// Created by daou__jaejin on 2025-03-25.
//

#include "asset.h"

#include "../db/db.h"

const char *asset_type_strings[] = {
    "현금자산",
    "주식자산"
    //, "Crypto" // 추후
};

// 문자 폭 맞추기
int get_display_width(const char* str) {
    int width = 0;
    while (*str) {
        if ((*str & 0x80) != 0) { // 한글(UTF-8 기준)
            width += 2;
            str += 3; // UTF-8 한글 3바이트
        } else {
            width += 1;
            str += 1;
        }
    }
    return width;
}

// 너비에 맞춰 오른쪽 공백 추가
void print_aligned_str(const char* str, int desired_width) {
    int current_width = get_display_width(str);
    printf("%s", str);
    for (int i = current_width; i < desired_width; i++) {
        printf(" ");
    }
}


void asset_print_asset() {
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    printf("<<%s님의 자산 현황>>\n\n", g_user_data->name);
    printf("[💵 현금자산] 총 %.2f원\n", asset_data[IDX_CASH].amount);
    printf("[📈 주식자산] 총 %.2f원\n", asset_data[IDX_STOCK].amount);
    printf("------------------------------\n");
    printf("[📊 총 자산] 총 %.2f원\n", asset_data[IDX_CASH].amount+asset_data[IDX_STOCK].amount);

    User_Stock *stock_data = asset_data[IDX_STOCK].data.stock.user_stock;

    if (stock_data != NULL) {
        int total_stocks = asset_data[IDX_STOCK].data.stock.stock_count;

        printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("📑 보유 종목 정보 (총 %d건)\n", total_stocks);
        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        printf("|     종목명      | 보유량 |     평단가     |       총액       |\n");
        printf("|-----------------|--------|----------------|------------------|\n");

        for (int i = 0; i < total_stocks; i++) {
            if (stock_data[i].quantity > 0) {
                double avg_price = stock_data[i].total_price / stock_data[i].quantity;

                printf("| ");
                print_aligned_str(stock_data[i].stock_name, 16);  // 맞춤 폭 (16칸)
                printf("| %6d | %12.2f원 | %14.2f원 |\n",
                    stock_data[i].quantity,
                    avg_price,
                    stock_data[i].total_price);
            }
        }

        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }


    free_asset(asset_data);
    getchar();
    printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

void asset_save_asset() {
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    int choice;

    printf("%s) 저장하실 파일 확장자를 선택해주세요!\n", g_chatbot_name);
    printf("\t1.bin\n\t2.csv\n\t3.text\n\t4.취소\n>>");
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

    free_asset(asset_data);
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

    User_Stock *stock_data = asset[IDX_STOCK].data.stock.user_stock;
    if (stock_data != NULL) {
        for (int i = 0; i < asset[IDX_STOCK].data.stock.stock_count; i++) {
            printf("- 종목명: %s, 보유량: %d, 평단가: %.2f, 총액: %.2f\n",
                stock_data[i].stock_name,
                stock_data[i].quantity,
                stock_data[i].current_price / stock_data[i].quantity,
                stock_data[i].total_price);
        }
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
            Asset *asset_data = db_getUserAsset(g_user_data->user_id);
            printf("%s) 출금할 금액을 알려주세요! (현재 잔액: %.2f원)\n>>", g_chatbot_name, asset_data[IDX_CASH].amount);
            scanf("%lf", &amount);
            if (asset_data[IDX_CASH].amount < amount) {
                printf("%s) 출금할 금액이 잔액보다 많아요!\n", g_chatbot_name);
                printf("%s) 남은 현금 잔액: %.2f원\n", g_chatbot_name, asset_data[IDX_CASH].amount);
                free_asset(asset_data);
                return;
            }
            free_asset(asset_data);
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            return;
    }
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    int cash_id = asset_data[IDX_CASH].asset_id;
    free_asset(asset_data);
    db_updateAsset(cash_id, amount * asset_method);
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
            if (db_checkStockName(stock_name) == false) {
                printf("%s) %s 주식의 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
                       g_chatbot_name, stock_name);
                return;
            }
            printf("%s) 매수할 종목의 개수를 알려주세요!\n>>", g_chatbot_name);
            scanf("%d", &quantity);
            if (quantity == 0) {
                printf("%s) 주식자산 조정이 완료되었습니다!\n", g_chatbot_name);
                return;
            }
            printf("%s) 매수할 종목의 평단가를 알려주세요!\n>>", g_chatbot_name);
            scanf("%lf", &price);
            break;
        case 2:
            asset_method = -1;

        //================================================
            Asset *asset_datas = db_getUserAsset(g_user_data->user_id);
            User_Stock *stock_data = asset_datas[IDX_STOCK].data.stock.user_stock;
            printf("%s) 현재 보유 주식은 다음과 같아요!\n", g_chatbot_name);
            if (stock_data != NULL) {
                for (int i = 0; i < asset_datas[IDX_STOCK].data.stock.stock_count; i++) {
                    if (stock_data[i].quantity > 0)
                    printf("\t%d) 종목명: %s, 보유량: %d, 평단가: %.2f, 총액: %.2f\n",
                        i + 1,
                        stock_data[i].stock_name,
                        stock_data[i].quantity,
                        stock_data[i].total_price / stock_data[i].quantity,
                        stock_data[i].total_price);
                }
            }
            free_asset(asset_datas);
        //================================================
            printf("\n%s) 매도할 종목의 이름을 알려주세요!\n>>", g_chatbot_name);
            scanf("%s", stock_name);
            if (db_checkStockName(stock_name) == false) {
                printf("%s) %s 주식의 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
                       g_chatbot_name, stock_name);
                return;
            }
            User_Stock *asset_data = db_getUserStock(g_user_data->user_id, stock_name);
            printf("%s) 매도할 종목의 개수를 알려주세요! (현재 보유수: %d개)\n>>",
                g_chatbot_name,
                asset_data == NULL ? 0 : asset_data->quantity);
            scanf("%d", &quantity);
            if (quantity == 0) {
                free(asset_data);
                printf("%s) 주식자산 조정이 완료되었습니다!\n", g_chatbot_name);
                return;
            }
            if (asset_data == NULL || asset_data->quantity < quantity) {
                printf("%s) 매도할 보유주식이 적어요!\n", g_chatbot_name);
                printf("%s) 현재 %s 보유주식: %d개\n",
                       g_chatbot_name,
                       stock_name,
                       asset_data == NULL ? 0 : asset_data->quantity);
                free(asset_data);
                return;
            }
            price = asset_data->total_price / asset_data->quantity;
            free(asset_data);
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            return;
    }
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    int stock_id = asset_data[IDX_STOCK].asset_id;
    free_asset(asset_data);
    db_updateAsset(stock_id, price * quantity * asset_method);
    db_updateUserStock(g_user_data->user_id, stock_name, asset_method * quantity, quantity * asset_method * price);
    printf("%s) 주식자산 조정이 완료되었습니다!\n", g_chatbot_name);
}

void free_asset(Asset *asset) {
    free(asset[IDX_STOCK].data.stock.user_stock);
    free(asset);
}