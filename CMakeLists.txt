cmake_minimum_required(VERSION 3.10)
project(CPP_REST)

set(CMAKE_CXX_STANDARD 14)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "-lpthread -Werror -Wall -lrt")
    MESSAGE(STATUS "current platform: Linux ")
ELSEIF (WIN32)
    set(CMAKE_CXX_FLAGS "-lpthread -lws2_32 -lwsock32 -Werror -Wall")
    MESSAGE(STATUS "current platform: Windows ")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")

add_executable(CPP_REST src/main.cpp # 主入口点
        src/network/defaultMsgLoop.cpp src/network/LinuxSocketObject.cpp src/network/WindowsSocketObject.cpp # 网络层
        src/proxy/HTTPProxyHandler.cpp  src/proxy/HTTPParser.cpp # 协议层
        src/router/Router.cpp src/router/RouterCallable.cpp src/router/HTMLRouter.hpp # 路由
        src/router/IHTTPRequest.cpp src/router/IHTTPResponse.cpp # 请求与响应
        src/utils/URL.cpp # URL编码相关
        lib/CPPLogger/CPPLogger.cpp # Logger库
        src/utils/stl_helper.h src/middleware/StaticResource.cpp src/middleware/StaticResource.h)


find_package(Threads)
target_link_libraries (CPP_REST ${CMAKE_THREAD_LIBS_INIT})

if (WIN32)
    find_library(WSOCK32_LIBRARY wsock32)
    find_library(WS2_32_LIBRARY ws2_32)
    target_link_libraries(CPP_REST wsock32 ws2_32)
ENDIF (WIN32)