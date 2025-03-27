#ifndef DB_H
#define DB_H
#define TOTAL_ASSET_NUM 2
#define IDX_CASH 0
#define IDX_STOCK 1

typedef struct User User;
typedef struct Asset Asset;
typedef struct User_Stock User_Stock;



User* db_getUser(const char* user_id);
Asset* db_getUserAsset(const char* user_id);
User_Stock* db_getUserStockList(const char *user_id, int *outCount);
User_Stock* db_getUserStock(const char *user_id, const char *stock_name);


#endif // DB_H
