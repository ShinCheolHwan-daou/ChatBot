#include "file.h"
#include "../user/user.h"
#include "../db/db.h"
#include "../asset/asset.h"
#include "../chatbot/chatbot.h"

#include <stdio.h>
#include <stdlib.h>
#include <cjson/cJSON.h>

void file_export_json() {
    Asset *assets = db_getUserAsset(g_user_data->user_id);
    int user_stock_count = 0;
    User_Stock *user_stocks = db_getUserStockList(g_user_data->user_id, &user_stock_count);
    int chat_count = 2;
    Chat chats[] = {
        {
            1,
            "daou",
            "주식 시장 분석 및 추천 종목",
            "우리는 주식 시장과 관련된 정보를 논의했습니다. 최근에는 AI와 반도체 분야에서 성장하는 기업들이 주목받고 있으며,\n2025년에는 주식 시장의 성장세가 다소 부진할 것으로 예상됩니다. 빅테크 기업 중에서는 엔비디아와 메타 플랫폼스가 긍정적인\n전망을 보이고 있습니다.",
            "[{\"role\":\"system\",\"content\":\"Please provide a plain text response without any formatting or emphasis. Use simpl e language and avoid idiomatic expressions. And answer by Korean.\"},{\"role\":\"system\",\"content\":\"너는 지금부터 고객 자금 관리 시스템을 가지고 상담원 역할을 할거야.\"},{\"role\":\"system\",\"content\":\"유저의 질문에 대한 답을 자산 관리와 연관시켜서 대답해줘.\"},{\"role\":\"user\",\"content\":\"주식 관련 정보 알려줘\"},{\"role\":\"assistant\",\"content\":\"현재 주식 시장은 전 세계적 으로 다양한 요인에 의해 영향을 받고 있습니다. 2025년에는 미국의 S&P 500 지수가 6500에서 7000까지 도달할 것으로 예상됩니 다[1][2]. 이와 함께 기술주 중심의 '매그니피센트 7'의 성장세 둔화 가능성도 지적되고 있습니다[1]. 한국 국내에서는 금융, 바 이오, AI 관련 산업이 유망한 투자처로 꼽히고 있습니다[3].\n\n또한, 주식시장 붕괴에 대한 우려도 있는데, 이는 주가가 기업의  기본적인 가치를 반영하지 않을 수 있다는 점에서 비롯됩니다[5]. 따라서, 투자자들은 장기적인 관점에서 시장 변동성을 준비하 고 포트폴리오를 다각화하는 것이 중요합니다[1][3]. 2025년에는 IPO 및 M&A 활성화도 기대되고 있습니다[2].\"},{\"role\":\"user\", \"content\":\"그럼 지금 추천 종목 있어?\"},{\"role\":\"assistant\",\"content\":\"현재 추천할 만한 종목으로는 AI 및 반도체 분야에서 앞서고 있는 엔비디아와 바이오테크 분야에서 차세대 백신 개발에 주력하는 모더나가 있습니다. 이们은 각각 AI 반도체 시장 점 유율 1위와 차세대 백신 및 항암 치료제 개발에 중점을 두고 있어, 미래 성장 가능성이 높습니다. 또한, 삼성전자는 AI 반도체 및 반도체 수요 증가로 성장할 가능성이 있고, 애플은 신제품 출시와 AI 서비스 확대에 기대를 모아고 있습니다. 그 외에 전기차  분야에서는 테슬라와 리비안이 주목받고 있습니다. 신규 상장 예정인 기업들 중에서는 씨케이솔루션, 한텍, 티엑스알로보틱스 등이 관심 거리를 모으고 있습니다. 하지만 주식 투자는 변동성이 크기 때문에 신중하게 결정하는 것이 중요합니다.\"},{\"role\":\" user\",\"content\":\"최근 1년 동안 가장 많이 오른 종목 알려줘\"},{\"role\":\"assistant\",\"content\":\"최근 1년 동안 한국 주식 시장 에서 가장 많이 상승한 종목에 대한 구체적인 정보는 제공된 자료에 없지만, 최근 상승률이 높은 종목들 중에서는 한텍과 티엑스 알로보틱스가 신규 상장 첫날 급등한 사례가 있습니다[1]. 지난해 2024년에는 특정 테마 주식들이 높은 상승률을 기록했으며, 이 는 주로 반도체, 2차 전지, 생명공학 등 관련 산업에 집중된 것으로 보입니다[5]. 최근에는 바이오인프라, 태영건설, 딥마인드 등이 상승세를 보이며 주목받고 있습니다[1]. 이러한 정보는 최신 시장 동향을 반영하며, 투자 전에는 시장 분석과 전문가의 의 견을 참고하는 것이 중요합니다.\"},{\"role\":\"user\",\"content\":\"빅테크 중에서는?\"},{\"role\":\"assistant\",\"content\":\"빅테크 중에 서는 최근 몇 년간 엔비디아와 메타 플랫폼스가 주목받고 있습니다. 엔비디아는 AI와 반도체 사업에서 강세를 보이고 있으며, 20 25년에도 주가 상승 가능성이 높다는 전망이 있습니다[1][2]. 메타 플랫폼스는 S&P 500 지수를 상회하는 수익률을 기록하며, 202 5년 기준 31.49% 상승했습니다[4]. 반면, 애플과 마이크로소프트 등 다른 빅테크 기업들은 상대적으로 성장률 둔화 문제로 인해 주가가 지지부진한 상태입니다[3][4]. 테슬라는 최근 주가 하락세를 보였으나,_AI와 자율주행 기술 개발로 인해 장기적인 성장 가능성은 여전히 존재합니다[1][4].\"}]",
            "2025-03-25 10:00:00",
            "2025-03-26 15:00:00",
            NULL
        },
        {
            2,
            "daou",
            "주식 시장 분석 및 추천 종목",
            "우리는 주식 시장과 관련된 정보를 논의했습니다. 최근에는 AI와 반도체 분야에서 성장하는 기업들이 주목받고 있으며,\n2025년에는 주식 시장의 성장세가 다소 부진할 것으로 예상됩니다. 빅테크 기업 중에서는 엔비디아와 메타 플랫폼스가 긍정적인\n전망을 보이고 있습니다.",
            "[{\"role\":\"system\",\"content\":\"Please provide a plain text response without any formatting or emphasis. Use simpl e language and avoid idiomatic expressions. And answer by Korean.\"},{\"role\":\"system\",\"content\":\"너는 지금부터 고객 자금 관리 시스템을 가지고 상담원 역할을 할거야.\"},{\"role\":\"system\",\"content\":\"유저의 질문에 대한 답을 자산 관리와 연관시켜서 대답해줘.\"},{\"role\":\"user\",\"content\":\"주식 관련 정보 알려줘\"},{\"role\":\"assistant\",\"content\":\"현재 주식 시장은 전 세계적 으로 다양한 요인에 의해 영향을 받고 있습니다. 2025년에는 미국의 S&P 500 지수가 6500에서 7000까지 도달할 것으로 예상됩니 다[1][2]. 이와 함께 기술주 중심의 '매그니피센트 7'의 성장세 둔화 가능성도 지적되고 있습니다[1]. 한국 국내에서는 금융, 바 이오, AI 관련 산업이 유망한 투자처로 꼽히고 있습니다[3].\n\n또한, 주식시장 붕괴에 대한 우려도 있는데, 이는 주가가 기업의  기본적인 가치를 반영하지 않을 수 있다는 점에서 비롯됩니다[5]. 따라서, 투자자들은 장기적인 관점에서 시장 변동성을 준비하 고 포트폴리오를 다각화하는 것이 중요합니다[1][3]. 2025년에는 IPO 및 M&A 활성화도 기대되고 있습니다[2].\"},{\"role\":\"user\", \"content\":\"그럼 지금 추천 종목 있어?\"},{\"role\":\"assistant\",\"content\":\"현재 추천할 만한 종목으로는 AI 및 반도체 분야에서 앞서고 있는 엔비디아와 바이오테크 분야에서 차세대 백신 개발에 주력하는 모더나가 있습니다. 이们은 각각 AI 반도체 시장 점 유율 1위와 차세대 백신 및 항암 치료제 개발에 중점을 두고 있어, 미래 성장 가능성이 높습니다. 또한, 삼성전자는 AI 반도체 및 반도체 수요 증가로 성장할 가능성이 있고, 애플은 신제품 출시와 AI 서비스 확대에 기대를 모아고 있습니다. 그 외에 전기차  분야에서는 테슬라와 리비안이 주목받고 있습니다. 신규 상장 예정인 기업들 중에서는 씨케이솔루션, 한텍, 티엑스알로보틱스 등이 관심 거리를 모으고 있습니다. 하지만 주식 투자는 변동성이 크기 때문에 신중하게 결정하는 것이 중요합니다.\"},{\"role\":\" user\",\"content\":\"최근 1년 동안 가장 많이 오른 종목 알려줘\"},{\"role\":\"assistant\",\"content\":\"최근 1년 동안 한국 주식 시장 에서 가장 많이 상승한 종목에 대한 구체적인 정보는 제공된 자료에 없지만, 최근 상승률이 높은 종목들 중에서는 한텍과 티엑스 알로보틱스가 신규 상장 첫날 급등한 사례가 있습니다[1]. 지난해 2024년에는 특정 테마 주식들이 높은 상승률을 기록했으며, 이 는 주로 반도체, 2차 전지, 생명공학 등 관련 산업에 집중된 것으로 보입니다[5]. 최근에는 바이오인프라, 태영건설, 딥마인드 등이 상승세를 보이며 주목받고 있습니다[1]. 이러한 정보는 최신 시장 동향을 반영하며, 투자 전에는 시장 분석과 전문가의 의 견을 참고하는 것이 중요합니다.\"},{\"role\":\"user\",\"content\":\"빅테크 중에서는?\"},{\"role\":\"assistant\",\"content\":\"빅테크 중에 서는 최근 몇 년간 엔비디아와 메타 플랫폼스가 주목받고 있습니다. 엔비디아는 AI와 반도체 사업에서 강세를 보이고 있으며, 20 25년에도 주가 상승 가능성이 높다는 전망이 있습니다[1][2]. 메타 플랫폼스는 S&P 500 지수를 상회하는 수익률을 기록하며, 202 5년 기준 31.49% 상승했습니다[4]. 반면, 애플과 마이크로소프트 등 다른 빅테크 기업들은 상대적으로 성장률 둔화 문제로 인해 주가가 지지부진한 상태입니다[3][4]. 테슬라는 최근 주가 하락세를 보였으나,_AI와 자율주행 기술 개발로 인해 장기적인 성장 가능성은 여전히 존재합니다[1][4].\"}]",
            "2025-03-25 10:00:00",
            "2025-03-26 15:00:00",
            NULL
        }
    };
    // Chat *chats = db_getUserChats(g_user_data->user_id, &chat_count);

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
    free(assets);
    free(user_stocks);
    // for (int i = 0; i < chat_count; i++) {
    //     free(chats[i].user_id);
    //     free(chats[i].title);
    //     free(chats[i].summary);
    //     free(chats[i].content);
    //     free(chats[i].created_at);
    //     free(chats[i].updated_at);
    //     free(chats[i].deleted_at);
    // }
    // free(chats);
    cJSON_free(json);
}
