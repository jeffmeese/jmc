set(TARGET_NAME jmchess)

set(HDR_FILES
  bitboard.h
  board.h
  board8x8.h
  cell.h
  engine.h
  evaluation.h
  fen.h
  magic_bitboard.h
  game.h
  move.h
  move_list.h
  perft.h
  search.h
  string_utilities.h
  timer.h
)

set(SRC_FILES
  bitboard.cpp
  board.cpp
  board8x8.cpp
  engine.cpp
  evaluation.cpp
  fen.cpp
  game.cpp
  magic_bitboard.cpp
  move.cpp
  move_list.cpp
  perft.cpp
  search.cpp
  string_utilities.cpp
  timer.cpp
)

add_library(${TARGET_NAME} STATIC ${HDR_FILES} ${SRC_FILES})

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
