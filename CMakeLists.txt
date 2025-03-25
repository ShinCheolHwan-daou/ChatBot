cmake_minimum_required(VERSION 3.30)
project(ChatBot C)

# ORACLE_HOME 환경 변수 사용
include_directories(C:/oracle/instantclient_21_17/include)
link_directories(C:/oracle/instantclient_21_17)

set(CMAKE_C_STANDARD 11)

add_executable(ChatBot main.c)

target_link_libraries(ChatBot oci)