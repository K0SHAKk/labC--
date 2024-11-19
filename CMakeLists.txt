# cmake_minimum_required(VERSION 3.14)

# set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
# set(PROJECT_VESRION 0.0.${PATCH_VERSION})

# project(lab2 VERSION ${PROJECT_VESRION})

# # GoogleTest requires at least C++14
# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

cmake_minimum_required(VERSION 3.14)
project(lab2)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Добавляем библиотеку lab2
add_library(lab2 test_ip_address.cpp)

# Добавляем исполняемый файл lab2_cli
add_executable(lab2_cli main.cpp)


# Устанавливаем общие свойства для lab2
set_target_properties(lab2 PROPERTIES
 CXX_STANDARD 14
 CXX_STANDARD_REQUIRED ON
)

# Включаем директории для lab2
target_include_directories(lab2
 PRIVATE "${CMAKE_BINARY_DIR}"
)

# Связываем lab2_cli с lab2
target_link_libraries(lab2_cli
 PRIVATE lab2
)

# Флаги компилятора
if (MSVC)
 target_compile_options(lab2_cli PRIVATE
  /W4
 )
 target_compile_options(lab2 PRIVATE
  /W4
 )
 target_compile_options(test_lab2 PRIVATE
  /W4
 )
 target_compile_options(test_ip_address PRIVATE
  /W4
 )
else ()
 target_compile_options(lab2_cli PRIVATE
  -Wall -Wextra -pedantic -Werror
 )
 target_compile_options(lab2 PRIVATE
  -Wall -Wextra -pedantic -Werror
 )
endif()

# Установка
install(TARGETS lab2_cli RUNTIME DESTINATION bin)

# CPack
# Установка пакета DEB для Linux
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
 set(CPACK_GENERATOR DEB)
endif()

# Установка пакета ZIP для macOS
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
 set(CPACK_GENERATOR ZIP)
endif()

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

# Настройка тестов
enable_testing()
add_executable(test_ip_address test_ip_address.cpp)
target_link_libraries(
  test_ip_address
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_ip_address)