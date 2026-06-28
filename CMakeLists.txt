cmake_minimum_required(VERSION 3.31)

project(NauEngineTest VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LUA_PATH "${CMAKE_SOURCE_DIR}/lua/lua-5.5.0/src")

file(GLOB_RECURSE SOURCES "src/*.cpp")

file(GLOB LUA_SOURCES "${LUA_PATH}/*.c" "${LUA_PATH}/*.h")
list(REMOVE_ITEM LUA_SOURCES
    "${LUA_PATH}/lua.c"
    "${LUA_PATH}/luac.c")

include_directories(include)
include_directories(${LUA_PATH})

add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCES}
    ${LUA_SOURCES}
)
