//
// Created by daou__jaejin on 2025-03-25.
//

#include "asset.h"
#include "../db/db.h"
#include "../print/print.h"



const char *asset_type_strings[] = {
    "현금자산",
    "주식자산"
    //, "Crypto" // 추후
};

// 문자 폭 맞추기
int get_display_width(const char *str) {
    int width = 0;
    while (*str) {
        if ((*str & 0x80) != 0) {
            // 한글(UTF-8 기준)
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
void print_aligned_str(const char *str, int desired_width) {
    int current_width = get_display_width(str);
    printf("%s", str);
    for (int i = current_width; i < desired_width; i++) {
        printf(" ");
    }
}


void asset_print_asset() {
    print_clear();
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    printf("<<%s님의 자산 현황>>\n\n", g_user_data->name);
    printf("[💵 현금자산] 총 %.2f원\n", asset_data[IDX_CASH].amount);
    printf("[📈 주식자산] 총 %.2f원\n", asset_data[IDX_STOCK].amount);
    printf("------------------------------\n");
    printf("[📊 총 자산] 총 %.2f원\n", asset_data[IDX_CASH].amount + asset_data[IDX_STOCK].amount);

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
                print_aligned_str(stock_data[i].stock_name, 16); // 맞춤 폭 (16칸)
                printf("| %6d | %12.2f원 | %14.2f원 |\n",
                       stock_data[i].quantity,
                       avg_price,
                       stock_data[i].total_price);
            }
        }

        printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    }


    free_asset(asset_data);
    print_enter();
    getchar();
}

void asset_save_asset() {
    Asset *asset_data = db_getUserAsset(g_user_data->user_id);


    const char *menu[] = {
        ".dat",
        ".csv",
        ".txt",
        "취소"
    };
    int selected = 0; // Tracks the currently selected menu option
    const int menu_size = sizeof(menu) / sizeof(menu[0]);
    char start_string[100];
    sprintf(start_string, "%s) 저장하실 파일 확장자를 선택해주세요!", g_chatbot_name);
    selected = select_menu(start_string, menu, menu_size);

    switch (selected) {
        case 0:
            save_binary(asset_data);
            break;
        case 1:
            save_csv(asset_data);
            break;
        case 2:
            save_text(asset_data);
            break;
        case 3:
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
    }

    free_asset(asset_data);
    if (selected != 3) {
        print_enter();
        getchar();
    }
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
    printf("\n%s) 💾csv 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_binary(Asset *asset) {
    FILE *f = fopen("assets.dat", "wb");

    for (int i = 0; i < TOTAL_ASSET_NUM; i++) {
        fwrite(asset_type_strings[i], sizeof(asset_type_strings[i]), 1, f);
        fwrite(&asset[i].amount, sizeof(asset[i].amount), 1, f);
    }

    fclose(f);
    printf("\n%s) 💾binary 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_text(Asset *asset) {
    FILE *f = fopen("assets.txt", "w");

    Asset *asset_data = db_getUserAsset(g_user_data->user_id);
    fprintf(f, "<<%s님의 자산 현황>>\n\n", g_user_data->name);
    fprintf(f, "[💵 현금자산] 총 %.2f원\n", asset_data[IDX_CASH].amount);
    fprintf(f, "[📈 주식자산] 총 %.2f원\n", asset_data[IDX_STOCK].amount);
    fprintf(f, "------------------------------\n");
    fprintf(f, "[📊 총 자산] 총 %.2f원\n", asset_data[IDX_CASH].amount + asset_data[IDX_STOCK].amount);

    User_Stock *stock_data = asset_data[IDX_STOCK].data.stock.user_stock;

    if (stock_data != NULL) {
        int total_stocks = asset_data[IDX_STOCK].data.stock.stock_count;

        fprintf(f, "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        fprintf(f, "📑 보유 종목 정보 (총 %d건)\n", total_stocks);
        fprintf(f, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
        fprintf(f, "|     종목명      | 보유량 |     평단가     |       총액       |\n");
        fprintf(f, "|-----------------|--------|----------------|------------------|\n");

        for (int i = 0; i < total_stocks; i++) {
            if (stock_data[i].quantity > 0) {
                double avg_price = stock_data[i].total_price / stock_data[i].quantity;

                fprintf(f, "| ");
                // print_aligned_str(stock_data[i].stock_name, 16); // 맞춤 폭 (16칸)
                int current_width = get_display_width(stock_data[i].stock_name);
                fprintf(f, "%s", stock_data[i].stock_name);
                for (int i = current_width; i < 16; i++) {
                    fprintf(f, " ");
                }
                fprintf(f, "| %6d | %12.2f원 | %14.2f원 |\n",
                       stock_data[i].quantity,
                       avg_price,
                       stock_data[i].total_price);
            }
        }
        fprintf(f, "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");

        fclose(f);
        printf("\n%s) 💾text 파일 저장을 완료했습니다!\n", g_chatbot_name);
    }
}

void asset_modify_asset_amount() {
    const char *menu[] = {
        "💵 현금 자산",
        "📈 주식 자산",
        "취소"
    };
    int selected = 0;
    const int menu_size = sizeof(menu) / sizeof(menu[0]);
    char start_string[100];
    sprintf(start_string, "어떤 자산을 기록할까요?");
    selected = select_menu(start_string, menu, menu_size);
    switch (selected) {
        case 0:
            modify_cash();
            break;
        case 1:
            modify_stock();
            break;
        case 2:
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
    }
}

static void modify_cash() {
    const char *menu[] = {
        "현금이 늘었어요 (증가)",
        "현금이 줄었어요 (감소)",
        "취소"
    };
    int selected = 0;
    const int menu_size = sizeof(menu) / sizeof(menu[0]);
    char start_string[100];
    sprintf(start_string, "%s) 현금자산을 어떻게 기록할까요?", g_chatbot_name);
    selected = select_menu(start_string, menu, menu_size);
    double amount;
    print_clear();
    switch (selected) {
        case 0:
            printf("%s) 현금이 얼마나 늘었나요?\n>>", g_chatbot_name);
            scanf("%lf", &amount);
            break;
        case 1:
            Asset *asset_data = db_getUserAsset(g_user_data->user_id);
            printf("\n%s) 현금이 얼마나 줄었나요? (현재 잔액: %.2f원)\n>>", g_chatbot_name, asset_data[IDX_CASH].amount);
            scanf("%lf", &amount);
            if (asset_data[IDX_CASH].amount < amount) {
                printf("\n%s) 출금할 금액이 잔액보다 많아요!\n", g_chatbot_name);
                printf("%s) 남은 현금 잔액: %.2f원\n", g_chatbot_name, asset_data[IDX_CASH].amount);
                print_enter();
                clear_input_buffer();
                getchar();
                free_asset(asset_data);
                return;
            }
            free_asset(asset_data);
            amount *= -1;
            break;
        case 2:
            break;
        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            print_enter();
            clear_input_buffer();
            getchar();
            return;
    }

    if (selected != 2) {
        clear_input_buffer();
        Asset *asset_data = db_getUserAsset(g_user_data->user_id);
        int cash_id = asset_data[IDX_CASH].asset_id;
        free_asset(asset_data);
        db_updateAsset(cash_id, amount);
        printf("\n%s) ✨현금자산 기록이 완료되었습니다!\n", g_chatbot_name);
        print_enter();
        getchar();
    }
}

static void modify_stock() {
    char stock_name[255];
    int quantity;
    double price;
    const char *menu[] = {
        "주식을 샀어요!",
        "주식을 팔았어요!",
        "취소"
    };
    int selected = 0;
    const int menu_size = sizeof(menu) / sizeof(menu[0]);
    char start_string[100];
    sprintf(start_string, "%s) 주식자산을 어떻게 기록할까요?", g_chatbot_name);
    selected = select_menu(start_string, menu, menu_size);

    print_clear();
    switch (selected) {
        case 0:
            printf("%s) 어떤 주식을 매수하셨나요? (종목명 입력)\n>>", g_chatbot_name);
            scanf("%s", stock_name);
            if (db_checkStockName(stock_name) == false) {
                printf("\n%s) %s 주식 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
                       g_chatbot_name, stock_name);
                print_enter();
                clear_input_buffer();
                getchar();
                return;
            }
            printf("\n%s) %s를 몇 주 매수하셨나요?\n>>", g_chatbot_name, stock_name);
            scanf("%d", &quantity);
            if (quantity == 0) {
                printf("\n%s) ✨주식자산 기록이 완료되었습니다!\n", g_chatbot_name);
                print_enter();
                clear_input_buffer();
                getchar();
                return;
            }
            printf("\n%s) 매수 가격은 얼마였나요?\n>>", g_chatbot_name);
            scanf("%lf", &price);
            break;
        case 1:
            Asset *asset_datas = db_getUserAsset(g_user_data->user_id);
            User_Stock *stock_data = asset_datas[IDX_STOCK].data.stock.user_stock;
            printf("%s) 현재 보유 주식은 다음과 같아요!\n", g_chatbot_name);
            if (stock_data != NULL) {
                int total_stocks = asset_datas[IDX_STOCK].data.stock.stock_count;

                printf("\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
                printf("📑 보유 종목 정보 (총 %d건)\n", total_stocks);
                printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
                printf("|     종목명      | 보유량 |     평단가     |       총액       |\n");
                printf("|-----------------|--------|----------------|------------------|\n");

                for (int i = 0; i < total_stocks; i++) {
                    if (stock_data[i].quantity > 0) {
                        double avg_price = stock_data[i].total_price / stock_data[i].quantity;

                        printf("| ");
                        print_aligned_str(stock_data[i].stock_name, 16); // 맞춤 폭 (16칸)
                        printf("| %6d | %12.2f원 | %14.2f원 |\n",
                               stock_data[i].quantity,
                               avg_price,
                               stock_data[i].total_price);
                    }
                }

                printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
            }
            free_asset(asset_datas);

            printf("\n%s) 매도하신 종목의 이름을 입력해주세요!\n>>", g_chatbot_name);
            scanf("%s", stock_name);
            if (db_checkStockName(stock_name) == false) {
                printf("%s) %s 주식 정보를 찾을 수 없습니다. 오타가 없는지 확인해주세요!\n",
                       g_chatbot_name, stock_name);
                print_enter();
                clear_input_buffer();
                getchar();
                return;
            }
            User_Stock *asset_data = db_getUserStock(g_user_data->user_id, stock_name);
            printf("\n%s) %s를 몇 주 매도하셨나요? (현재 보유수: %d개)\n>>",
                   g_chatbot_name,
                   stock_name,
                   asset_data == NULL ? 0 : asset_data->quantity);
            scanf("%d", &quantity);
            if (quantity == 0) {
                free(asset_data);
                printf("\n%s) ✨주식자산 기록이 완료되었습니다!\n", g_chatbot_name);
                print_enter();
                clear_input_buffer();
                getchar();
                return;
            }
            if (asset_data == NULL || asset_data->quantity < quantity) {
                printf("\n%s) 매도할 보유주식이 적어요!\n", g_chatbot_name);
                printf("%s) 현재 %s 보유주식: %d주\n",
                       g_chatbot_name,
                       stock_name,
                       asset_data == NULL ? 0 : asset_data->quantity);
                free(asset_data);
                return;
            }
            price = asset_data->total_price / asset_data->quantity;
            quantity *= -1;
            free(asset_data);
            break;
        case 2:
            break;

        default:
            printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
            print_enter();
            clear_input_buffer();
            getchar();
            return;
    }

    if (selected != 2) {
        clear_input_buffer();
        Asset *asset_data = db_getUserAsset(g_user_data->user_id);
        int stock_id = asset_data[IDX_STOCK].asset_id;
        free_asset(asset_data);
        db_updateAsset(stock_id, price * quantity);
        db_updateUserStock(g_user_data->user_id, stock_name, quantity, quantity * price);
        printf("\n%s) ✨주식자산 기록이 완료되었습니다!\n", g_chatbot_name);
        print_enter();
        getchar();
    }
}

void free_asset(Asset *asset) {
    free(asset[IDX_STOCK].data.stock.user_stock);
    free(asset);
}
