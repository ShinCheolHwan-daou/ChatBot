#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>


// 응답 데이터를 저장할 구조체
struct Memory {
    char *response;
    size_t size;
};

// 데이터를 받아서 저장하는 함수
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct Memory *mem = (struct Memory *) userp;

    // 응답 데이터 크기 증가
    char *ptr = realloc(mem->response, mem->size + total_size + 1);
    if (ptr == NULL) {
        printf("메모리 할당 실패\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, total_size);
    mem->size += total_size;
    mem->response[mem->size] = 0; // Null-terminate

    return total_size;
}

int curlTest_curl() {
    CURL *curl;
    CURLcode res;

    struct Memory chunk;
    chunk.response = malloc(1); // 초기 할당
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // 요청할 URL 설정
        curl_easy_setopt(curl, CURLOPT_URL, "https://jsonplaceholder.typicode.com/posts/1");

        // 응답 데이터를 WriteCallback 함수로 저장
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // 요청 실행
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() 실패: %s\n", curl_easy_strerror(res));
        } else {
            printf("응답 데이터:\n%s\n", chunk.response);
        }

        // 정리
        curl_easy_cleanup(curl);
    }

    // 메모리 해제
    free(chunk.response);
    curl_global_cleanup();

    return 0;
}
