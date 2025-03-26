//
// Created by daou__jaejin on 2025-03-25.
//

#ifndef CHATBOT_H
#define CHATBOT_H

typedef struct {
    int chat_id;
    char *user_id;
    char *title;
    char *summary;
    char *content;
    char *created_at;
    char *updated_at;
    char *deleted_at;
} Chat;

void chatbot_chat();

#endif //CHATBOT_H
