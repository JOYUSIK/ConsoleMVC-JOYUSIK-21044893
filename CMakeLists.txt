cmake_minimum_required(VERSION 3.20)
project(ConsoleMVC CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows MSVC 한글 콘솔 출력 설정
if(MSVC)
    add_compile_options(/utf-8)
endif()

# nlohmann/json 자동 다운로드
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
    DOWNLOAD_NO_EXTRACT TRUE
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/third_party
)
FetchContent_MakeAvailable(nlohmann_json)

# 소스 파일 수집
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ConsoleMVC ${SOURCES})

target_include_directories(ConsoleMVC PRIVATE include)
