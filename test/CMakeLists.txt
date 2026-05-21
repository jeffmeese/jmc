add_executable(test_board8x8 test_board8x8.cpp)
target_link_libraries(test_board8x8 jmchess)
target_link_libraries(test_board8x8 GTest::gtest_main)

add_executable(test_bitboard test_bitboard.cpp)
target_link_libraries(test_bitboard jmchess)
target_link_libraries(test_bitboard GTest::gtest_main)