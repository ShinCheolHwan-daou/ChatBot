cmake_minimum_required(VERSION 3.30)
project(ChatBot C)

# ORACLE_HOME 환경 변수 사용
include_directories(C:/oracle/instantclient_21_17/include)
link_directories(C:/oracle/instantclient_21_17)

set(CMAKE_C_STANDARD 11)

add_executable(ChatBot main.c
        db/db.c
        db/db.h
        file/file.c
        file/file.h
        chatbot/chatbot.c
        chatbot/chatbot.h
        user/user.c
        user/user.h
        asset/asset.c
        asset/asset.h)

target_link_libraries(ChatBot oci)