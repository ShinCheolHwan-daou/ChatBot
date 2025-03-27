#include "file.h"
#include "../user/user.h"
#include "../asset/asset.h"

#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

void file_export_json() {
    Asset *assets = db_getUserAsset(g_user_data->user_id);
    int user_stock_count = 0;
    User_Stock *user_stocks = db_getUserStockList(g_user_data->user_id, &user_stock_count);
    cJSON *json = cJSON_CreateObject();
    cJSON *user_json = cJSON_CreateObject();
    cJSON_AddStringToObject(user_json, "id", g_user_data->user_id);
    cJSON_AddStringToObject(user_json, "name", g_user_data->name);
    cJSON_AddItemToObject(json, "user", user_json);

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
                for (int j = 0; j < user_stock_count; j++) {
                    User_Stock cur_stock = user_stocks[j];
                    cJSON *user_stock_json = cJSON_CreateObject();
                    cJSON_AddStringToObject(user_stock_json, "stock_name", cur_stock.stock_name);
                    cJSON_AddNumberToObject(user_stock_json, "quantity", cur_stock.quantity);
                    cJSON_AddNumberToObject(user_stock_json, "total_price", cur_stock.total_price);
                    // todo:: cur_price 추가
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

    // todo:: chat 처리
    cJSON *chats_json = cJSON_CreateArray();
    cJSON_AddItemToObject(json, "chats", chats_json);

    char *json_str = cJSON_PrintUnformatted(json);
    FILE *fp = fopen("result.json", "w");
    fprintf(fp, "%s\n", json_str);
    fclose(fp);
    free(json_str);
    free(assets);
    free(user_stocks);
    cJSON_free(json);
}
