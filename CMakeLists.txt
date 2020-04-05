cmake_minimum_required(VERSION 3.8)
project(hydra)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE HYDRA_SOURCE_FILES "Source/*.cpp")
file(GLOB_RECURSE HYDRA_HEADER_FILES "Include/*.h")

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(hydra ${HYDRA_HEADER_FILES} ${HYDRA_SOURCE_FILES})
target_include_directories(hydra PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_link_libraries(hydra Threads::Threads Qt5::Widgets SQLite::SQLite3)
