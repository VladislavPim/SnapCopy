cmake_minimum_required(VERSION 3.20)
project(SnapCopy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "D:/Softs/Qt6.5.3-Windows-x86_64-VS2022-17.10.1-20240608/Qt6.5.3-Windows-x86_64-VS2022-17.10.1")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(SOURCES
    src/main.cpp
    src/SnapBar.cpp
    src/TrayIcon.cpp
    src/ClipboardListener.cpp
    src/HistoryWindow.cpp
    src/SettingsWindow.cpp
    src/Logger.cpp
    src/AppState.cpp
)

set(HEADERS
    src/SnapBar.h
    src/TrayIcon.h
    src/ClipboardListener.h
    src/HistoryWindow.h
    src/SettingsWindow.h
    src/Logger.h
    src/AppState.h
)

# Добавляем файл ресурсов (иконка + версия)
set(RESOURCES
    resources/app.rc
)

add_executable(SnapCopy WIN32 ${SOURCES} ${HEADERS} ${RESOURCES})

target_link_libraries(SnapCopy
    Qt6::Core
    Qt6::Widgets
    user32
    ole32
    shell32
)

add_custom_command(TARGET SnapCopy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_PREFIX_PATH}/bin/Qt6Core.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt6Widgets.dll
        ${CMAKE_PREFIX_PATH}/bin/Qt6Gui.dll
        $<TARGET_FILE_DIR:SnapCopy>
    COMMENT "Копируем Qt DLL"
)

add_custom_command(TARGET SnapCopy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_PREFIX_PATH}/plugins/platforms
        $<TARGET_FILE_DIR:SnapCopy>/platforms
    COMMENT "Копируем platform plugins"
)

# Копируем иконку рядом с exe (если нужно для каких-то целей)
add_custom_command(TARGET SnapCopy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/app.ico
        $<TARGET_FILE_DIR:SnapCopy>/app.ico
    COMMENT "Копируем иконку"
)