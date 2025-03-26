//
// Created by daou__jaejin on 2025-03-25.
//

#ifndef ASSET_H
#define ASSET_H

#include "../user/user.h"

#define CHAR_LEN 255

typedef enum {
    CASH,
    STOCK
    //, CRYPTO // 추후
} Asset_Type;

typedef struct {
    int user_stock_id;
    int stock_id;
    char stock_name[CHAR_LEN];
    int quantity;
    int total_price; // 주식 별 총액
} User_Stock;

typedef union {
    struct {
        User_Stock *user_stock;
    } stock;

} Asset_Data;

typedef struct {
    int asset_id;
    char user_id[CHAR_LEN];
    double amount; // 자산 별 총액
    Asset_Type category;
    Asset_Data data;
} Asset;

// 1번 선택지
void asset_print_asset();
// 2번 선택지
void asset_save_asset();
// 3번 선택지
void asset_modify_asset_amount();

static void save_csv(Asset *asset);
static void save_binary(Asset *asset);
static void save_text(Asset *asset);

#endif //ASSET_H
