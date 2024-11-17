cmake_minimum_required(VERSION 3.7.2)
project(physics)

set(CMAKE_CXX_STANDARD 17)

# Пытаемся найти предустановленный SFML
find_package(SFML COMPONENTS graphics)

if (SFML_FOUND)
    message(STATUS "The SFML installed in the system will be used")
else ()
    # Если SFML не найден - выкачиваем и собираем из исходников
    message(STATUS "Get SFML from the source code")
    include(FetchContent)
    FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 2.6.x)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    # Обход потенциальных проблем при сборке sfml-graphics
    # (stb_image.h: 'argument': conversion from 'int' to 'short')
    set(WARNINGS_AS_ERRORS FALSE CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SFML)
endif ()

# К сожалению, вплоть до C++20 https://en.cppreference.com/w/cpp/numeric/constants
# нет удобного кроссплатформенного способа получить число pi.
# define ниже нужен для включения M_PI
if (MSVC)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

add_executable(physics
        main.cpp World.cpp Ball.cpp Physics.cpp Color.cpp Painter.cpp Dust.cpp
        black_box/Application.cpp black_box/View.cpp black_box/PainterImpl.cpp)
 
set(CMAKE_INSTALL_PREFIX "/usr/local")

target_link_libraries(physics PRIVATE sfml-graphics)


target_compile_options(physics PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
        )

  # Установка по умолчани
# Установка исполняемого файла
install(TARGETS physics DESTINATION bin)

# Установка конфигурации для пакета
if(APPLE)
    set(CPACK_GENERATOR "DMG")  # Для macOS используем DMG
    set(CPACK_PACKAGE_FILE_NAME "MyExecutable-macos")  # Имя пакета для macOS
elseif(UNIX)
    set(CPACK_GENERATOR "DEB")  # Для Linux используем DEB
    set(CPACK_PACKAGE_FILE_NAME "MyExecutable-linux")  # Имя пакета для Linux
else()
    set(CPACK_GENERATOR "ZIP")  # Для Windows используем ZIP
    set(CPACK_PACKAGE_FILE_NAME "MyExecutable-windows")  # Имя пакета для Windows
endif()

set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_DESCRIPTION "MyExecutable package description")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6)")
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")
set(CPACK_PACKAGE_HOMEPAGE "https://example.com")
set(CPACK_PACKAGE_LICENSE "MIT")

include(CPack)