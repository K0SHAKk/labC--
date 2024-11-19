
cmake_minimum_required(VERSION 3.5.0)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(lab2 VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(CPACK_PROJECT_NAME lab2)

# Установка пакета DEB для Linux
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR DEB) # Настраивает CPack для создания пакетов DEB (Linux).
    # Укажите зависимости пакета
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libgtk-3-dev libstdc++6-dev")
endif()
# Установка пакета ZIP для macOS
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CPACK_GENERATOR ZIP) # Настройка CPack для создания пакетов ZIP (macOS)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing() # Включает тестирование
add_test(NAME test_main COMMAND tests) 
add_subdirectory(gtest)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
