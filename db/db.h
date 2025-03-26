#ifndef DB_H
#define DB_H
// #include "user.h"
// #include "asset.h"
#define TOTAL_ASSET_NUM 2
#define IDX_CASH 255
#define IDX_STOCK 255


// 이따 지우기~~~~~~~~~
#define CHAR_LEN 255

typedef struct {
    char user_id[CHAR_LEN];
    char name[CHAR_LEN];
    char password[CHAR_LEN];
} User;

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



User* db_getUser(const char* user_id);
Asset* db_getUserAsset(const char* user_id);
User_Stock* db_getUserStockList(const char *user_id, int *outCount);


#endif // DB_H
