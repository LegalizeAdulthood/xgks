cmake_minimum_required(VERSION 3.26)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(misc_target)
include(target_folder)

set(XGKS_PROJECT_NAME "xgks" CACHE STRING "Project name for IDEs")
project("${XGKS_PROJECT_NAME}" VERSION 1.0.0 LANGUAGES C CXX)

add_subdirectory(demo)
add_subdirectory(fontdb)
add_subdirectory(lib)

misc_target(FILES
    "CMakePresets.json"
    "README")
misc_group_sources("CMake Scripts" FILES
    "cmake/misc_target.cmake"
    "cmake/target_folder.cmake")
misc_group_sources("Editing" FILES
    ".clang-format"
    ".editorconfig")
misc_group_sources("Git" FILES
    ".gitignore")
misc_group_sources("GitHub" FILES
    ".github/workflows/cmake.yml")
