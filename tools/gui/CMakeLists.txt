set(TARGET_NAME jmgui)

find_package(Qt6 COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTOUIC TRUE)
set(CMAKE_AUTORCC TRUE)

set(HDR_FILES
  application/application.h
  application/gui_game.h
  application/mainwindow.h
  application/move_model.h
  view/boardview.h
)

set(SRC_FILES
  application/application.cpp
  application/gui_game.cpp
  application/main.cpp
  application/mainwindow.cpp
  application/move_model.cpp
  view/boardview.cpp
)

set(UI_FILES
  application/mainwindow.ui
)

set(RES_FILES
  resources/resources.qrc
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES} ${RES_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TARGET_NAME} Qt::Core Qt::Gui Qt::Widgets)
target_link_libraries(${TARGET_NAME} jmchess)