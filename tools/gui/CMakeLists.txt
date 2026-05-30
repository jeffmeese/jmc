set(TARGET_NAME jmgui)

find_package(Qt6 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

set(HDR_FILES
  application/application.h
  application/mainwindow.h
  view/boardview.h
)

set(SRC_FILES
  application/application.cpp
  application/main.cpp
  application/mainwindow.cpp
  view/boardview.cpp
)

set(UI_FILES
  application/mainwindow.ui
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} Qt::Core Qt::Gui Qt::Widgets)
target_link_libraries(${TARGET_NAME} jmchess)