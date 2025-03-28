#include "chatbot.h"
#include "../user/user.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>


#define ROLE_LENGTH 10
#define CONTENT_LENGTH 4096

// 응답 데이터를 저장할 버퍼 구조체
typedef struct {
    char *data;
    size_t size;
} ResponseData;

typedef struct {
    char role[ROLE_LENGTH];
    char content[CONTENT_LENGTH];
} Message;

char *perplexity_api_key;

static void add_message(Message *messages, int *message_length, Message new_message) {
    if (messages == NULL) {
        perror("messages should not be null.\n");
        return;
    }

    if (sizeof(messages) / sizeof(Message) >= *message_length) {
        Message *new_messages = realloc(messages, sizeof(Message) * *message_length);
        if (new_messages == NULL) {
            perror("messages realloc fail.\n");
        }
        messages = new_messages;
    }

    strcpy(messages[*message_length].role, new_message.role);
    strcpy(messages[*message_length].content, new_message.content);
    (*message_length)++;
}

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    ResponseData *response = userdata;

    // 응답 데이터를 동적으로 확장
    char *temp = realloc(response->data, response->size + total_size + 1);
    if (temp == NULL) {
        perror("Failed to allocate memory\n");
        return 0;
    }

    response->data = temp;
    memcpy(&(response->data[response->size]), ptr, total_size);
    response->size += total_size;
    response->data[response->size] = '\0';

    return total_size;
}

static char *create_messages_json(const int message_length, const Message *messages) {
    cJSON *messages_json = cJSON_CreateArray();
    for (int i = 0; i < message_length; i++) {
        cJSON *message_json = cJSON_CreateObject();
        cJSON_AddStringToObject(message_json, "role", messages[i].role);
        cJSON_AddStringToObject(message_json, "content", messages[i].content);
        cJSON_AddItemToArray(messages_json, message_json);
    }
    char *json_str = cJSON_PrintUnformatted(messages_json); // JSON을 문자열로 변환
    cJSON_Delete(messages_json); // JSON 객체 해제
    return json_str;
}

static char *create_request_body(const int message_length, const Message *messages) {
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "model", "sonar");

    char *messages_str = create_messages_json(message_length, messages);
    cJSON *messages_json = cJSON_Parse(messages_str);

    cJSON_AddItemToObject(json, "messages", messages_json);
    // cJSON_AddNumberToObject(json, "max_tokens", 200);

    char *json_str = cJSON_PrintUnformatted(json); // JSON을 문자열로 변환
    cJSON_Delete(json); // JSON 객체 해제
    free(messages_str);

    return json_str; // 동적으로 할당된 문자열 반환 (free 필요)
}

static Message parse_response_to_message(const char *json_str) {
    // JSON 파서 생성
    cJSON *json = cJSON_Parse(json_str);
    Message answer = {};

    if (json == NULL) {
        perror("Failed to parse JSON.\n");
    }

    // JSON 데이터 파싱
    cJSON *choices = cJSON_GetObjectItem(json, "choices");
    cJSON *choice = cJSON_GetArrayItem(choices, 0);
    cJSON *item = cJSON_GetObjectItem(choice, "message");
    cJSON *role = cJSON_GetObjectItem(item, "role");

    strcpy(answer.role, cJSON_GetStringValue(role));
    cJSON *content = cJSON_GetObjectItem(item, "content");
    strcpy(answer.content, cJSON_GetStringValue(content));
    cJSON_Delete(json);
    return answer;
}

static Message chatbot_chat_completions(const Message *messages, int message_length) {
    CURL *curl;
    CURLcode res;

    ResponseData response;
    response.data = malloc(1);
    response.size = 0;
    Message answer;

    char *post_data = create_request_body(message_length, messages);

    curl = curl_easy_init();
    if (curl) {
        // 요청할 URL 설정
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.perplexity.ai/chat/completions");

        // POST 요청 설정
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

        // 요청 헤더 설정 (JSON 데이터 전송)
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        char auth[1024];
        sprintf(auth, "Authorization: Bearer %s", perplexity_api_key);
        headers = curl_slist_append(
            headers, auth);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // 응답을 받을 콜백 함수 설정
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response);

        // 요청 실행
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            perror("curl_easy_perform() failed.\n");
        } else {
            answer = parse_response_to_message(response.data);
        }

        // 정리
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    } else {
        perror("Failed to create curl.\n");
    }

    free(response.data);
    free(post_data);
    return answer;
}

void chatbot_chat() {
    getchar();
    if (g_user_data == NULL) {
        perror("Need user data.\n");
        return;
    }

    perplexity_api_key = getenv("PERPLEXITY_API_KEY");
    if (perplexity_api_key == NULL) {
        perror("Failed to get PERPLEXITY_API_KEY.\n");
        return;
    }

    printf("%s님 반갑습니다!\n", g_user_data->name);
    printf("챗봇을 중단하려면 quit 또는 q를 입력하세요.\n");

    int message_length = 0;
    Message *messages = malloc(sizeof(Message) * 128);
    Message system_messages[] = {
        {
            "system",
            "Please provide a plain text response without any formatting or emphasis. Use simple language and avoid idiomatic expressions. And answer by Korean."
        },
        {
            "system",
            "너는 지금부터 고객 자금 관리 시스템을 가지고 상담원 역할을 할거야."
        },
        {
            "system",
            "유저의 질문에 대한 답을 자산 관리와 연관시켜서 대답해줘."
        }
    };

    for (int i = 0; i < sizeof(system_messages) / sizeof(Message); i++) {
        strcpy(messages[i].role, system_messages[i].role);
        strcpy(messages[i].content, system_messages[i].content);
        message_length++;
    }


    while (true) {
        fflush(stdin);
        Message question = {"user"};
        printf("\n질문을 입력해주세요: ");
        if (fgets(question.content, sizeof(question.content), stdin) == NULL) {
            perror("Error reading input.\n");
            continue;
        }

        if (strcmp(question.content, "") == 0 || strcmp(question.content, "\n") == 0) {
            continue;
        }

        question.content[strcspn(question.content, "\n")] = 0;
        if (strcmp(question.content, "quit") == 0 || strcmp(question.content, "q") == 0) {
            break;
        }

        add_message(messages, &message_length, question);
        Message answer = chatbot_chat_completions(messages, message_length);
        add_message(messages, &message_length, answer);
        printf("%s\n", answer.content);
    }

    // 질문을 안했을 경우, return
    if (sizeof(system_messages) / sizeof(Message) == message_length) {
        free(messages);
        return;
    }

    // 1. 제목 생성
    Message title_request = {"user", "Please summarize our conversation so far as a title (maximum 20 length)."};
    add_message(messages, &message_length, title_request);
    Message title = chatbot_chat_completions(messages, message_length);
    message_length--;

    // 2. 요약 생성
    Message summary_request = {
        "user", "Please summarize our conversation so far as a brief summary. (maximum 3 lines)"
    };
    add_message(messages, &message_length, summary_request);
    Message summary = chatbot_chat_completions(messages, message_length);
    message_length--;

    char *messages_str = create_messages_json(message_length, messages);
    Chat new_chat;
    new_chat.user_id = g_user_data->user_id;
    new_chat.title = title.content;
    new_chat.summary = summary.content;
    new_chat.content = messages_str;

    printf("제목: %s\n", new_chat.title);
    printf("요약: %s\n", new_chat.summary);

    // todo::
    // db_insertUserChat(new_chat);

    free(messages_str);
    free(messages);
    printf("채팅을 종료합니다.\n");
}
