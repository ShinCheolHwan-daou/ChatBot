#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci.h>

#include "db.h"
#include "../user/user.h"
#include "../asset/asset.h"


char* username = "C##C_CHATBOT";
char* password = "1234";
char* dbname = "192.168.31.102";
static OCIEnv* envhp;
static OCIError* errhp;
static OCISvcCtx* svchp;
static OCISession* usrhp;
static OCIServer* srvhp;


void check_error(OCIError* errhp) {
    text errbuf[512];
    sb4 errcode = 0;
    OCIErrorGet((dvoid*)errhp, (ub4)1, (text*)NULL, &errcode, errbuf, (ub4)sizeof(errbuf),
        OCI_HTYPE_ERROR);
    // printf("Error: %p\n", errbuf);
}


static int db_connect() {
    putenv("NLS_LANG=.AL32UTF8");
    // 환경 핸들 생성
    if (OCIEnvCreate(&envhp, OCI_DEFAULT, NULL, NULL, NULL, NULL, 0, NULL) !=
        OCI_SUCCESS) {
        printf("OCIEnvCreate failed\n");
        return -1;
        }
    // 오류 핸들 생성
    if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&errhp, OCI_HTYPE_ERROR, (size_t)0,
        (dvoid**)NULL) != OCI_SUCCESS) {
        printf("OCIHandleAlloc failed for error handle\n");
        return -1;
        }
    // 서버 핸들 생성
    if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&srvhp, OCI_HTYPE_SERVER, (size_t)0,
        (dvoid**)NULL) != OCI_SUCCESS) {
        printf("OCIHandleAlloc failed for server handle\n");
        return -1;
        }
    // 서버 연결
    if (OCIServerAttach(srvhp, errhp, (text*)dbname, strlen(dbname), OCI_DEFAULT) !=
        OCI_SUCCESS) {
        check_error(errhp);
        return -1;
        }
    // 서비스 컨텍스트 생성
    if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&svchp, OCI_HTYPE_SVCCTX, (size_t)0,
        (dvoid**)NULL) != OCI_SUCCESS) {
        check_error(errhp);
        return -1;
        }
    // 세션 핸들 생성 및 연결
    if (OCIHandleAlloc((dvoid*)envhp, (dvoid**)&usrhp, OCI_HTYPE_SESSION, (size_t)0,
        (dvoid**)NULL) != OCI_SUCCESS) {
        check_error(errhp);
        return -1;
        }
    if (OCILogon2(envhp, errhp, &svchp,
        (OraText*)username, (ub4)strlen(username),
        (OraText*)password, (ub4)strlen(password),
        (OraText*)dbname, (ub4)strlen(dbname),
        OCI_DEFAULT /* 마지막 인수 추가 */
    ) != OCI_SUCCESS) {
        check_error(errhp);
        return -1;
    }
    printf("Oracle Database connected successfully.\n");
    return 0;
}


static void db_disconnect() {
    OCILogoff(svchp, errhp);
    OCIHandleFree((dvoid*)usrhp, OCI_HTYPE_SESSION);
    OCIHandleFree((dvoid*)svchp, OCI_HTYPE_SVCCTX);
    OCIHandleFree((dvoid*)srvhp, OCI_HTYPE_SERVER);
    OCIHandleFree((dvoid*)errhp, OCI_HTYPE_ERROR);
    OCIHandleFree((dvoid*)envhp, OCI_HTYPE_ENV);
}


User* db_getUser(const char* user_id) {
    // 1) DB 연결
    if (db_connect() != 0) {
        fprintf(stderr, "[db_getUser] DB 연결 실패\n");
        return NULL;
    }

    OCIStmt *stmt = NULL;
    sword status;

    // 2) SQL
    const char* sql =
      "SELECT USER_ID, NAME, PASSWORD "
      "FROM \"USER\" "
      "WHERE USER_ID = :1";

    // 3) 스테이트먼트 핸들
    OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);
    check_error(errhp);

    // 4) 쿼리 준비
    OCIStmtPrepare(stmt, errhp, (text*)sql, (ub4)strlen(sql),
                   OCI_NTV_SYNTAX, OCI_DEFAULT);
    check_error(errhp);

    // 5) 바인딩
    OCIBind* bnd1 = NULL;
    OCIBindByPos(stmt, &bnd1, errhp, 1,
                 (dvoid *)user_id, (sb4)(strlen(user_id)+1),
                 SQLT_STR,
                 NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

    // 6) 실행
    status = OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);
    check_error(errhp);

    // 7) Define
    User* user = (User*)malloc(sizeof(User));
    if (!user) {
        fprintf(stderr, "[db_getUser] malloc failed.\n");
        // 에러 시 cleanup
        OCIHandleFree(stmt, OCI_HTYPE_STMT);
        db_disconnect();
        return NULL;
    }
    memset(user, 0, sizeof(User));

    // 8) Define
    OCIDefine *def1 = NULL, *def2 = NULL, *def3 = NULL;
    OCIDefineByPos(stmt, &def1, errhp, 1,
                   (dvoid*)user->user_id, sizeof(user->user_id),
                   SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def2, errhp, 2,
                   (dvoid*)user->name, sizeof(user->name),
                   SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def3, errhp, 3,
                   (dvoid*)user->password, sizeof(user->password),
                   SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);

    // 9) Fetch
    status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT);

    if (status == OCI_NO_DATA) {
        // => 해당 user_id가 없는 경우
        free(user);
        user = NULL;
        printf("등록된 id가 없습니다");
    }
    else if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
        // => 에러
        check_error(errhp);
        free(user);
        user = NULL;
    }
    // else if (status == OCI_SUCCESS) => user 구조체에 값이 들어가 있음

    // 자원 해제
    OCIHandleFree(stmt, OCI_HTYPE_STMT);
    db_disconnect();

    // user 포인터 반환 (없으면 NULL)
    return user;
}

/**
 * @brief 유저의 현금 자산 + 주식 자산 정보를 합쳐서 Asset[2] 배열로 반환
 *
 *  - assets[0] => 현금 (category=CASH)
 *  - assets[1] => 주식 (category=STOCK)
 *     - data.stock.user_stock => 보유 종목 배열. 없으면 NULL
 *  - 현금/주식이 없으면 amount=0, asset_id=0 으로 처리
 *
 * @param user_id   조회할 사용자 ID
 * @return Asset*    동적 할당된 Asset[2] 배열.
 *                   실패 시 NULL.
 *                   사용 후 free(assets[1].data.stock.user_stock), free(assets).
 */
Asset* db_getUserAsset(const char* user_id)
{
    if (!user_id) return NULL;

    // 1) DB 연결
    if (db_connect() != 0) {
        fprintf(stderr, "[db_getUserAsset] DB 연결 실패\n");
        return NULL;
    }

    // 2) Asset 배열 2개 할당
    Asset *assets = (Asset*)calloc(TOTAL_ASSET_NUM, sizeof(Asset)*TOTAL_ASSET_NUM);
    if (!assets) {
        fprintf(stderr, "[db_getUserAsset] 메모리 할당 실패\n");
        db_disconnect();
        return NULL;
    }

    // 초기값 세팅:
    // - assets[0] = 현금, assets[1] = 주식
    // - 만약 DB에 없으면 amount=0, asset_id=0인 상태를 유지
    assets[IDX_CASH].category = CASH;
    assets[IDX_STOCK].category = STOCK;
    // user_id도 미리 넣어둬도 되지만, DB에서 가져오면 덮어씌워짐

    // --------------------------
    // (A) 현금 자산 조회
    // --------------------------
    {
        // SQL: SELECT ASSET_ID, USER_ID, AMOUNT
        //      FROM ASSET
        //      WHERE USER_ID=:1 AND CATEGORY=0 (CASH)
        const char *sql_cash =
            "SELECT ASSET_ID, USER_ID, AMOUNT "
            "FROM ASSET "
            "WHERE USER_ID = :1 AND CATEGORY = 0";

        OCIStmt *stmt = NULL;
        // 1) 스테이트먼트 핸들 생성
        OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);

        // 2) 쿼리 준비
        OCIStmtPrepare(stmt, errhp, (text*)sql_cash, (ub4)strlen(sql_cash),
                       OCI_NTV_SYNTAX, OCI_DEFAULT);

        // 3) 바인딩
        {
            OCIBind *bnd1 = NULL;
            OCIBindByPos(stmt, &bnd1, errhp, 1,
                         (dvoid*)user_id,
                         (sb4)(strlen(user_id)+1),
                         SQLT_STR,
                         NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
        }

        // 4) 실행
        OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

        // 5) Define
        int    asset_id       = 0;
        char   db_user_id[CHAR_LEN] = {0};
        double amount_cash    = 0.0;

        OCIDefine *def1=NULL, *def2=NULL, *def3=NULL;
        OCIDefineByPos(stmt, &def1, errhp, 1, &asset_id,            sizeof(asset_id),
                       SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmt, &def2, errhp, 2, db_user_id,           sizeof(db_user_id),
                       SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmt, &def3, errhp, 3, &amount_cash,         sizeof(amount_cash),
                       SQLT_BDOUBLE, NULL, NULL, NULL, OCI_DEFAULT);

        // 6) Fetch (단일 행만 필요)
        sword status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT);
        if (status == OCI_SUCCESS) {
            // 현금 자산 정보를 assets[0]에 담는다
            assets[0].asset_id = asset_id;
            strncpy(assets[0].user_id, db_user_id, CHAR_LEN-1);
            assets[0].amount = amount_cash;
            // category=CASH는 이미 세팅
        }
        // OCI_NO_DATA => 현금 자산이 없는 경우 -> 유지 (amount=0, asset_id=0)

        // 스테이트먼트 해제
        OCIHandleFree(stmt, OCI_HTYPE_STMT);
    }

    // --------------------------
    // (B) 주식 자산 조회
    // --------------------------
    {
        // SQL: SELECT ASSET_ID, USER_ID, AMOUNT
        //      FROM ASSET
        //      WHERE USER_ID=:1 AND CATEGORY=1 (STOCK)
        const char *sql_stock =
            "SELECT ASSET_ID, USER_ID, AMOUNT "
            "FROM ASSET "
            "WHERE USER_ID = :1 AND CATEGORY = 1";

        OCIStmt *stmt = NULL;
        OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);

        OCIStmtPrepare(stmt, errhp, (text*)sql_stock, (ub4)strlen(sql_stock),
                       OCI_NTV_SYNTAX, OCI_DEFAULT);

        {
            OCIBind *bnd1 = NULL;
            OCIBindByPos(stmt, &bnd1, errhp, 1,
                         (dvoid*)user_id,
                         (sb4)(strlen(user_id)+1),
                         SQLT_STR,
                         NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
        }

        OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

        int    asset_id       = 0;
        char   db_user_id[CHAR_LEN] = {0};
        double amount_stock   = 0.0;

        OCIDefine *def1=NULL, *def2=NULL, *def3=NULL;
        OCIDefineByPos(stmt, &def1, errhp, 1, &asset_id,       sizeof(asset_id),
                       SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmt, &def2, errhp, 2, db_user_id,      sizeof(db_user_id),
                       SQLT_STR, NULL, NULL, NULL, OCI_DEFAULT);
        OCIDefineByPos(stmt, &def3, errhp, 3, &amount_stock,   sizeof(amount_stock),
                       SQLT_BDOUBLE, NULL, NULL, NULL, OCI_DEFAULT);

        sword status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT);
        if (status == OCI_SUCCESS) {
            assets[1].asset_id = asset_id;
            strncpy(assets[1].user_id, db_user_id, CHAR_LEN-1);
            assets[1].amount   = amount_stock;
            // category=STOCK은 이미 세팅
        }
        // OCI_NO_DATA => 주식 자산이 없는 경우 -> 유지 (amount=0, asset_id=0)

        OCIHandleFree(stmt, OCI_HTYPE_STMT);
    }

    // --------------------------
    // (C) 보유 주식 목록 조회
    // --------------------------
    int stockCount = 0;
    User_Stock *stockArray = db_getUserStockList(user_id, &stockCount);
    // 여기서 stockCount에는 "몇 개의 주식 종목인지"가 들어감

    // 만약 하나도 없으면 stockArray == NULL
    if (stockArray) {
        assets[IDX_STOCK].data.stock.user_stock = stockArray;
        // 필요하면 assets[1].stock_count = stockCount; // 등등
    } else {
        assets[IDX_STOCK].data.stock.user_stock = NULL;
        // count=0 => 주식 없음
    }

    // 3) DB 해제
    db_disconnect();

    // 4) 2개짜리 Asset 배열 리턴
    return assets;
}

/**
 * @brief   특정 user_id의 주식 보유 목록(User_Stock)을 고정 크기 배열로 가져온다
 * @param   user_id   사용자 아이디 (ex: "daou")
 * @param   outCount  주식 개수를 반환할 포인터 (반드시 유효한 int*를 넣어야 함)
 * @return  User_Stock*  동적 할당된 배열 (outCount개의 원소). 실패 시 NULL
 *
 * 주의: 이 함수 안에서 db_connect/db_disconnect는 호출하지 않습니다.
 *       이미 상위에서 연결이 열려 있다는 전제 하에, 스테이트먼트 준비/실행만 수행.
 */
User_Stock* db_getUserStockList(const char *user_id, int *outCount)
{
    if (!user_id || !outCount) {
        return NULL;
    }
    *outCount = 0;  // 초기화

    // --------------------------------------------------
    // (A) 먼저 보유 주식 수(count) 파악
    // --------------------------------------------------
    const char *sql_count = "SELECT COUNT(*) FROM USER_STOCK WHERE USER_ID=:1";

    OCIStmt *stmt = NULL;
    OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);

    OCIStmtPrepare(stmt, errhp, (text*)sql_count, (ub4)strlen(sql_count),
                   OCI_NTV_SYNTAX, OCI_DEFAULT);

    // 바인딩
    {
        OCIBind *bnd1 = NULL;
        OCIBindByPos(stmt, &bnd1, errhp, 1,
                     (dvoid*)user_id,
                     (sb4)(strlen(user_id)+1),
                     SQLT_STR,
                     NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    }

    OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    // Define
    int countVal = 0;
    {
        OCIDefine *def1=NULL;
        OCIDefineByPos(stmt, &def1, errhp, 1, &countVal, sizeof(countVal),
                       SQLT_INT, NULL, NULL, NULL, OCI_DEFAULT);
    }

    // Fetch (단일 행)
    sword status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT);
    if (status == OCI_SUCCESS) {
        *outCount = countVal;  // outCount에 저장
    }

    OCIHandleFree(stmt, OCI_HTYPE_STMT);

    if (*outCount <= 0) {
        // 주식을 하나도 보유 안 함
        return NULL;
    }

    // --------------------------------------------------
    // (B) count 크기만큼 배열 할당
    // --------------------------------------------------
    User_Stock *stockArray = (User_Stock*)malloc(sizeof(User_Stock)*(*outCount));
    if (!stockArray) {
        fprintf(stderr, "[db_getUserStockList] malloc failed.\n");
        return NULL;
    }
    memset(stockArray, 0, sizeof(User_Stock)*(*outCount));

    // --------------------------------------------------
    // (C) 주식 목록 SELECT (JOIN STOCK)
    // --------------------------------------------------
    const char *sql_user_stocks =
        "SELECT us.USER_STOCK_ID, us.STOCK_ID, s.NAME, us.QUANTITY, us.TOTAL_PRICE, s.PRICE"
        "FROM USER_STOCK us, STOCK s "
        "WHERE us.USER_ID = :1 AND us.STOCK_ID = s.STOCK_ID";

    OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);

    OCIStmtPrepare(stmt, errhp, (text*)sql_user_stocks, (ub4)strlen(sql_user_stocks),
                   OCI_NTV_SYNTAX, OCI_DEFAULT);
    {
        OCIBind *bnd1 = NULL;
        OCIBindByPos(stmt, &bnd1, errhp, 1,
                     (dvoid*)user_id,
                     (sb4)(strlen(user_id)+1),
                     SQLT_STR,
                     NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    }
    OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    // Define 변수
    int    user_stock_id;
    int    stock_id;
    char   stock_name[64];
    int    quantity;
    double    total_price;
    double    current_price;

    memset(stock_name, 0, sizeof(stock_name));

    OCIDefine *def1=NULL, *def2=NULL, *def3=NULL, *def4=NULL, *def5=NULL, *def6=NULL;
    OCIDefineByPos(stmt, &def1, errhp, 1, &user_stock_id, sizeof(user_stock_id), SQLT_INT,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def2, errhp, 2, &stock_id,      sizeof(stock_id),      SQLT_INT,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def3, errhp, 3, stock_name,     sizeof(stock_name),    SQLT_STR,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def4, errhp, 4, &quantity,      sizeof(quantity),       SQLT_INT,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def5, errhp, 5, &total_price,   sizeof(total_price),    SQLT_BDOUBLE,
                   NULL, NULL, NULL, OCI_DEFAULT);
OCIDefineByPos(stmt, &def6, errhp, 6, &current_price,   sizeof(current_price),    SQLT_BDOUBLE,
                   NULL, NULL, NULL, OCI_DEFAULT);

    // Fetch count번 반복
    int idx = 0;
    while ((status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT)) == OCI_SUCCESS)
    {
        if (idx >= *outCount) {
            // DB 레코드가 더 많아졌다면(동시에 INSERT 등)
            // 여기서는 초과분을 무시
            break;
        }
        stockArray[idx].user_stock_id = user_stock_id;
        stockArray[idx].stock_id      = stock_id;
        strncpy(stockArray[idx].stock_name, stock_name,
                sizeof(stockArray[idx].stock_name)-1);
        stockArray[idx].quantity      = quantity;
        stockArray[idx].total_price   = total_price;
        stockArray[idx].current_price   = current_price;

        idx++;
    }

    OCIHandleFree(stmt, OCI_HTYPE_STMT);

    // 반환
    return stockArray;
}

User_Stock* db_getUserStock(const char *user_id, const char *stock_name) {
    if (!user_id || !stock_name) return NULL;

    if (db_connect() != 0) {
        fprintf(stderr, "[db_getUserStock] DB 연결 실패\n");
        return NULL;
    }

    const char *sql =
        "SELECT us.USER_STOCK_ID, us.STOCK_ID, s.NAME, us.QUANTITY, us.TOTAL_PRICE, s.PRICE as CURRENT_PRICE "
        "FROM STOCK s, USER_STOCK us "
        "WHERE s.STOCK_ID = us.STOCK_ID "
        "AND us.USER_ID = :1 "
        "AND s.NAME = :2";

    OCIStmt *stmt = NULL;
    OCIHandleAlloc(envhp, (dvoid**)&stmt, OCI_HTYPE_STMT, 0, NULL);

    OCIStmtPrepare(stmt, errhp, (text*)sql, (ub4)strlen(sql),
                   OCI_NTV_SYNTAX, OCI_DEFAULT);

    // 바인딩
    OCIBind *bnd1 = NULL, *bnd2 = NULL;
    OCIBindByPos(stmt, &bnd1, errhp, 1, (dvoid*)user_id, (sb4)(strlen(user_id)+1),
                 SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    OCIBindByPos(stmt, &bnd2, errhp, 2, (dvoid*)stock_name, (sb4)(strlen(stock_name)+1),
                 SQLT_STR, NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

    OCIStmtExecute(svchp, stmt, errhp, 0, 0, NULL, NULL, OCI_DEFAULT);

    // Define 변수
    int user_stock_id = 0;
    int stock_id = 0;
    char db_stock_name[CHAR_LEN] = {0};
    int quantity = 0;
    double total_price = 0;
    double current_price = 0;

    OCIDefine *def1 = NULL, *def2 = NULL, *def3 = NULL, *def4 = NULL, *def5 = NULL, *def6 = NULL;
    OCIDefineByPos(stmt, &def1, errhp, 1, &user_stock_id, sizeof(user_stock_id), SQLT_INT,
               NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def2, errhp, 2, &stock_id, sizeof(stock_id), SQLT_INT,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def3, errhp, 3, db_stock_name, sizeof(db_stock_name), SQLT_STR,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def4, errhp, 4, &quantity, sizeof(quantity), SQLT_INT,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def5, errhp, 5, &total_price, sizeof(total_price), SQLT_BDOUBLE,
                   NULL, NULL, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt, &def6, errhp, 6, &current_price, sizeof(current_price), SQLT_BDOUBLE,
                   NULL, NULL, NULL, OCI_DEFAULT);

    // Fetch
    sword status = OCIStmtFetch2(stmt, errhp, 1, OCI_FETCH_NEXT, 0, OCI_DEFAULT);

    if (status == OCI_NO_DATA) {
        printf("[db_getUserStock] No stock found for user_id='%s', stock_name='%s'\n", user_id, stock_name);
    } else if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
        printf("[db_getUserStock] OCIStmtFetch2 failed.\n");
        check_error(errhp);
    }
    User_Stock *result = NULL;
    if (status == OCI_SUCCESS) {
        result = (User_Stock*)malloc(sizeof(User_Stock));
        result->user_stock_id = user_stock_id;
        result->stock_id = stock_id;
        strncpy(result->stock_name, db_stock_name, CHAR_LEN - 1);
        result->quantity = quantity;
        result->total_price = total_price;
        result->current_price = current_price;
    }

    OCIHandleFree(stmt, OCI_HTYPE_STMT);
    db_disconnect();

    return result;
}

double db_updateAsset(int asset_id, double delta) {
    if (asset_id <= 0) {
        fprintf(stderr, "[db_addToAssetAmount] Invalid asset_id: %d\n", asset_id);
        return -1;
    }

    if (db_connect() != 0) {
        fprintf(stderr, "[db_addToAssetAmount] DB 연결 실패\n");
        return -1;
    }

    double current_amount = 0.0;

    // 1) 현재 amount 조회
    const char *sql_select = "SELECT AMOUNT FROM ASSET WHERE ASSET_ID = :1";
    OCIStmt *stmt_select = NULL;
    OCIHandleAlloc(envhp, (dvoid**)&stmt_select, OCI_HTYPE_STMT, 0, NULL);
    OCIStmtPrepare(stmt_select, errhp, (text*)sql_select, strlen(sql_select), OCI_NTV_SYNTAX, OCI_DEFAULT);

    OCIBind *bnd1 = NULL;
    OCIDefine *def1 = NULL;

    OCIBindByPos(stmt_select, &bnd1, errhp, 1, (dvoid*)&asset_id, sizeof(asset_id), SQLT_INT,
                 NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    OCIDefineByPos(stmt_select, &def1, errhp, 1, &current_amount, sizeof(current_amount), SQLT_BDOUBLE,
                   NULL, NULL, NULL, OCI_DEFAULT);

    sword status = OCIStmtExecute(svchp, stmt_select, errhp, 1, 0, NULL, NULL, OCI_DEFAULT);
    if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO) {
        fprintf(stderr, "[db_addToAssetAmount] 현재 amount 조회 실패\n");
        check_error(errhp);
        OCIHandleFree(stmt_select, OCI_HTYPE_STMT);
        db_disconnect();
        return -1;
    }
    OCIHandleFree(stmt_select, OCI_HTYPE_STMT);

    // 2) 더한 값 계산
    double new_amount = current_amount + delta;

    // 3) 업데이트
    const char *sql_update = "UPDATE ASSET SET AMOUNT = :1 WHERE ASSET_ID = :2";
    OCIStmt *stmt_update = NULL;
    OCIHandleAlloc(envhp, (dvoid**)&stmt_update, OCI_HTYPE_STMT, 0, NULL);
    OCIStmtPrepare(stmt_update, errhp, (text*)sql_update, strlen(sql_update), OCI_NTV_SYNTAX, OCI_DEFAULT);

    OCIBind *bnd_amt = NULL, *bnd_id = NULL;
    OCIBindByPos(stmt_update, &bnd_amt, errhp, 1, &new_amount, sizeof(new_amount), SQLT_BDOUBLE,
                 NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);
    OCIBindByPos(stmt_update, &bnd_id, errhp, 2, &asset_id, sizeof(asset_id), SQLT_INT,
                 NULL, NULL, NULL, 0, NULL, OCI_DEFAULT);

    status = OCIStmtExecute(svchp, stmt_update, errhp, 1, 0, NULL, NULL, OCI_DEFAULT);
    if (status != OCI_SUCCESS) {
        fprintf(stderr, "[db_addToAssetAmount] amount 업데이트 실패\n");
        check_error(errhp);
        OCIHandleFree(stmt_update, OCI_HTYPE_STMT);
        db_disconnect();
        return -1;
    }

    // 4) 커밋
    OCITransCommit(svchp, errhp, 0);
    OCIHandleFree(stmt_update, OCI_HTYPE_STMT);

    db_disconnect();
    return 0;
}


