set(TARGET_NAME jmconsole)

set(HDR_FILES
  console_game.h
)

set(SRC_FILES
  console_game.cpp
  main.cpp
)

add_executable(${TARGET_NAME} ${HDR_FILES} ${SRC_FILES})

target_link_libraries(${TARGET_NAME} jmchess)
