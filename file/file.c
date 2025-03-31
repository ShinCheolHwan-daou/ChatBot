#include "file.h"
#include "../user/user.h"
#include "../db/db.h"
#include "../asset/asset.h"
#include "../chatbot/chatbot.h"

#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

#include "../print/print.h"

void file_export_json() {
    Asset *assets = db_getUserAsset(g_user_data->user_id);
    int chat_count = 0;
    Chat *chats = db_getUserChats(g_user_data->user_id, &chat_count);

    cJSON *json = cJSON_CreateObject();

    // user 데이터
    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddStringToObject(user_json, "id", g_user_data->user_id);
    cJSON_AddStringToObject(user_json, "name", g_user_data->name);
    cJSON_AddItemToObject(json, "user", user_json);

    // asset 데이터
    cJSON *assets_json = cJSON_CreateArray();
    for (int i = 0; i < TOTAL_ASSET_NUM; i++) {
        Asset cur_asset = assets[i];
        cJSON *asset_json = cJSON_CreateObject();
        switch (cur_asset.category) {
            case CASH:
                cJSON_AddNumberToObject(asset_json, "category", cur_asset.category);
                cJSON_AddNumberToObject(asset_json, "amount", cur_asset.amount);
                break;

            case STOCK:
                cJSON_AddNumberToObject(asset_json, "category", cur_asset.category);
                cJSON_AddNumberToObject(asset_json, "amount", cur_asset.amount);

                cJSON *user_stocks_json = cJSON_CreateArray();
                for (int j = 0; j < cur_asset.data.stock.stock_count; j++) {
                    User_Stock cur_stock = cur_asset.data.stock.user_stock[j];
                    cJSON *user_stock_json = cJSON_CreateObject();
                    cJSON_AddStringToObject(user_stock_json, "stock_name", cur_stock.stock_name);
                    cJSON_AddNumberToObject(user_stock_json, "quantity", cur_stock.quantity);
                    cJSON_AddNumberToObject(user_stock_json, "total_price", cur_stock.total_price);
                    cJSON_AddNumberToObject(user_stock_json, "current_price", cur_stock.current_price);
                    cJSON_AddItemToArray(user_stocks_json, user_stock_json);
                }
                cJSON_AddItemToObject(asset_json, "user_stocks", user_stocks_json);
                break;

            default:
                perror("unknown category.\n");
                break;
        }
        cJSON_AddItemToArray(assets_json, asset_json);
    }
    cJSON_AddItemToObject(json, "assets", assets_json);

    // chat 데이터
    cJSON *chats_json = cJSON_CreateArray();
    cJSON_AddItemToObject(json, "chats", chats_json);
    for (int i = 0; i < chat_count; i++) {
        Chat cur_chat = chats[i];
        cJSON *chat_json = cJSON_CreateObject();
        cJSON_AddNumberToObject(chat_json, "id", cur_chat.chat_id);
        cJSON_AddStringToObject(chat_json, "title", cur_chat.title);
        cJSON_AddStringToObject(chat_json, "summary", cur_chat.summary);
        cJSON *chat_content_json = cJSON_Parse(cur_chat.content);
        cJSON_AddItemToObject(chat_json, "content", chat_content_json);
        cJSON_AddStringToObject(chat_json, "created_at", cur_chat.created_at);
        cJSON_AddItemToArray(chats_json, chat_json);
    }

    char *json_str = cJSON_PrintUnformatted(json);
    FILE *fp = fopen("result.json", "w");
    fprintf(fp, "%s\n", json_str);
    fclose(fp);

    free(json_str);
    free_asset(assets);
    for (int i = 0; i < chat_count; i++) {
        free(chats[i].user_id);
        free(chats[i].title);
        free(chats[i].summary);
        free(chats[i].content);
        free(chats[i].created_at);
        free(chats[i].updated_at);
        free(chats[i].deleted_at);
    }
    free(chats);
    cJSON_free(json);

    printf("파일 저장에 성공했습니다.\n");
    print_color(BLUE, BLACK, "http://192.168.31.102:3000");
    printf("에서 결과보고서를 확인하세요!\n");
    printf("%s) Enter 키를 눌러 계속 진행하세요...\n", g_chatbot_name);
    getchar();
}
