cmake_minimum_required(VERSION 3.0.0)
project(url-shortener VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include(CTest)
enable_testing()

add_executable(url-shortener main.cpp shortener.cpp)

find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost REQUIRED system)

target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_link_libraries(${PROJECT_NAME} ${OpenSSL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
