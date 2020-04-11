cmake_minimum_required(VERSION 3.12)
project(Hydra)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE HYDRA_SOURCE_FILES "Source/*.cpp")
file(GLOB_RECURSE HYDRA_HEADER_FILES "Include/*.h")

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(Hydra WIN32 ${HYDRA_HEADER_FILES} ${HYDRA_SOURCE_FILES})
target_include_directories(Hydra PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")
target_link_libraries(Hydra Qt5::Widgets SQLite::SQLite3)

add_custom_command(TARGET Hydra POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/logo.svg
        $<TARGET_FILE_DIR:Hydra>)

# On Windows we want to copy the shared libraries next to the application.
if (WIN32)
    # Copy SQLite DLL
    get_filename_component(SQLite3_LIB_DIR ${SQLite3_LIBRARIES} DIRECTORY)
    add_custom_command(TARGET Hydra POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SQLite3_LIB_DIR}/sqlite3.dll
            $<TARGET_FILE_DIR:Hydra>)
    # Run windeployqt on the generated executable
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET Hydra POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                --release
                --verbose 0
                --no-compiler-runtime
                --no-translations
                \"$<TARGET_FILE:Hydra>\"
        COMMENT "Deploying Qt...")
endif(WIN32)