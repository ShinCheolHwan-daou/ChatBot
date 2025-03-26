//
// Created by daou__jaejin on 2025-03-25.
//

#include "asset.h"

const char* asset_type_strings[] = {
    "현금자산",
    "주식자산"
    //, "Crypto" // 추후
};


void asset_print_asset() {
    // [TODO] asset 데이터 정보들 db에서 받아오는 함수 넣기
    Asset* asset_data;
    asset_data = db_getUserAsset(g_user_data->user_id);

    printf("<<%s님의 자산 현황>>\n", g_user_data->name);
    printf("[현금자산] 총 %.2f원\n", asset_data[IDX_CASH].amount);
    printf("[주식자산] 총 %.2f원\n", asset_data[IDX_STOCK].amount);

    getchar();
    printf("%s) 아무 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

void asset_save_asset() {
    // [TODO] asset 데이터 정보들 db에서 받아오는 함수 넣기
    Asset asset_data[2];
    int choice;

    printf("%s) 저장하실 파일 확장자를 선택해주세요!\n", g_chatbot_name);
    printf("\t1.bin\n\t2.csv\n\t3.text\n>>");
    scanf("%d", &choice);
    // [TODO] 두 줄 지우기
    printf("아직 구현되지 않은 기능입니다.\n");
    // switch(choice) {
    //     case 1:
    //         save_binary(asset_data);
    //         break;
    //     case 2:
    //         save_csv(asset_data);
    //         break;
    //     case 3:
    //         save_text(asset_data);
    //         break;
    //     default:
    //         printf("%s) 유효한 선택이 아닙니다.\n", g_chatbot_name);
    // }
    getchar();
    printf("%s) 아무 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}

static void save_csv(Asset *asset) {
    // [TODO] 지현님 define에 따라서 코드 수정하기
    FILE* f = fopen("assets.csv", "w");
    fprintf(f, "자산유형,총액\n");
    for (int i = 0; i < 2; i++) {
        fprintf(f, "%s,%.2f\n",
            asset_type_strings[i],
            asset[i].amount
        );
    }
    fclose(f);
    printf("%s) csv 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_binary(Asset *asset) {
    // [TODO] 지현님 define에 따라서 코드 수정하기
    FILE* f = fopen("assets.dat", "wb");

    for (int i = 0; i < 2; i++) {
        fwrite(asset_type_strings[i], sizeof(asset_type_strings[i]), 1, f);
        fwrite(&asset[i].amount, sizeof(asset[i].amount), 1, f);
    }

    fclose(f);
    printf("%s) binary 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

static void save_text(Asset *asset) {
    // [TODO] 지현님 define에 따라서 코드 수정하기
    FILE* f = fopen("assets.txt", "w");
    fprintf(f, "자산유형,총액\n");
    for (int i = 0; i < 2; i++) {
        fprintf(f, "[%s] 총 %.2f원\n",
            asset_type_strings[i],
            asset[i].amount
        );
    }
    fclose(f);
    printf("%s) text 파일 저장을 완료했습니다!\n", g_chatbot_name);
}

// void asset_modify_asset_amount() {
//     printf("")
// }