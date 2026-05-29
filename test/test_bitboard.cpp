// BitBoard_tests.cpp

#include <gtest/gtest.h>

#include "bitboard.h"
#include "move.h"
#include "move_list.h"

using namespace jmchess;

namespace
{

bool containsMove(
  const MoveList & moveList,
  std::int8_t sourceIndex,
  std::int8_t destIndex)
{
  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    const Move & move = moveList.getMove(i);

    std::int8_t source = move.getSourceIndex();
    std::int8_t dest = move.getDestinationIndex();
    if (source == sourceIndex && dest == destIndex)
    {
      return true;
    }
    //Square src = move.getSourceSquare();
    //Square dst = move.getDestinationSquare();

    //if (src.row == sourceRow && src.col == sourceCol && dst.row == destRow && dst.col == destCol)
    //{
    //  return true;
    //}
  }

  return false;
}

} // namespace

TEST(
  BitboardTests,
  ResetPlacesKingsCorrectly)
{
  BitBoard board;

  EXPECT_EQ(board.getPieceType(0, 4), PieceType::WhiteKing);
  EXPECT_EQ(board.getPieceType(7, 4), PieceType::BlackKing);
}

TEST(
  BitBoardTests,
  ResetPlacesPawnsCorrectly)
{
  BitBoard board;

  for (int col = 0; col < 8; ++col)
  {
    EXPECT_EQ(board.getPieceType(1, col), PieceType::WhitePawn);
    EXPECT_EQ(board.getPieceType(6, col), PieceType::BlackPawn);
  }
}

TEST(
  BitBoardTests,
  InitialPositionGenerates20Moves)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_EQ(moveList.totalMoves(), 20);
}

TEST(
  BitBoardTests,
  WhitePawnSinglePushGenerated)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  EXPECT_TRUE(containsMove(moveList, 12, 20));
}

TEST(
  BitBoardTests,
  WhitePawnDoublePushGenerated)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  EXPECT_TRUE(containsMove(moveList, 12, 28));
}

TEST(
  BitBoardTests,
  KnightMovesGeneratedCorrectly)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(0, 1, moveList);

  EXPECT_TRUE(containsMove(moveList, 1, 16));
  EXPECT_TRUE(containsMove(moveList, 1, 18));
}

TEST(
  BitBoardTests,
  BishopInitiallyBlocked)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(0, 2, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  BitBoardTests,
  RookInitiallyBlocked)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(0, 0, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  BitBoardTests,
  QueenInitiallyBlocked)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(0, 3, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  BitBoardTests,
  KingInitiallyBlocked)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(0, 4, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  BitBoardTests,
  EmptyBoardHasNoMoves)
{
  BitBoard board;
  board.setPosition("8/8/8/8/8/8/8/8 w - - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  BitBoardTests,
  WhiteKingAttackDetection)
{
  BitBoard board;

  board.setPosition("8/8/8/8/4r3/8/4K3/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(1, 4, Color::Black));
}

TEST(
  BitBoardTests,
  BlackKingAttackDetection)
{
  BitBoard board;

  board.setPosition("8/8/8/B7/8/8/8/4k3 b - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(0, 4, Color::White));
}

TEST(
  BitBoardTests,
  KnightAttackDetection)
{
  BitBoard board;

  board.setPosition("8/8/8/8/3n4/8/4K3/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(1, 4, Color::Black));
}

TEST(
  BitBoardTests,
  PawnAttackDetection)
{
  BitBoard board;

  board.setPosition("8/8/8/8/3p4/4K3/8/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(2, 4, Color::Black));
}

TEST(
  BitBoardTests,
  CastlingGeneratedWhiteKingside)
{
  BitBoard board;

  board.setPosition("4k3/8/8/8/8/8/8/4K2R w K - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_TRUE(containsMove(moveList, 4, 6));
}

TEST(
  BitBoardTests,
  CastlingGeneratedWhiteQueenside)
{
  BitBoard board;

  board.setPosition("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_TRUE(containsMove(moveList, 4, 2));
}

TEST(
  BitBoardTests,
  CastlingBlockedByAttack)
{
  BitBoard board;

  board.setPosition("4k1r1/8/8/8/8/8/8/4K2R w K - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_FALSE(containsMove(moveList, 4, 6));
}

TEST(
  BitBoardTests,
  PawnPromotionGenerated)
{
  BitBoard board;

  board.setPosition("4k3/P7/8/8/8/8/8/4K3 w - - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  int promotionCount = 0;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    if (moveList.getMove(i).isPromotion())
    {
      ++promotionCount;
    }
  }

  EXPECT_EQ(promotionCount, 4);
}

TEST(
  BitBoardTests,
  EnPassantGenerated)
{
  BitBoard board;

  board.setPosition("4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1");

  MoveList moveList;

  board.generateMoves(moveList);
  std::cout << moveList.totalMoves() << std::endl;

  bool found = false;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    if (moveList.getMove(i).isEnpassantCapture())
    {
      found = true;
      break;
    }
  }

  EXPECT_TRUE(found);
}

TEST(
  BitBoardTests,
  MakeMoveUpdatesBoard)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  Move selectedMove = moveList.getMove(0);

  bool found = false;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    std::int8_t src = moveList.getMove(i).getSourceIndex();
    std::int8_t dst = moveList.getMove(i).getDestinationIndex();
    //Square src = moveList.getMove(i).getSourceSquare();
    //Square dst = moveList.getMove(i).getDestinationSquare();

    //if (src.row == 1 && src.col == 4 && dst.row == 3 && dst.col == 4)
    if (src == 12 && dst == 28)
    {
      selectedMove = moveList.getMove(i);
      found        = true;
      break;
    }
  }

  ASSERT_TRUE(found);

  board.makeMove(selectedMove);

  EXPECT_EQ(board.getPieceType(3, 4), PieceType::WhitePawn);
  EXPECT_EQ(board.getPieceType(1, 4), PieceType::None);
}

TEST(
  BitBoardTests,
  UnmakeMoveRestoresBoard)
{
  BitBoard board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  Move selectedMove = moveList.getMove(0);

  bool found = false;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    std::int8_t src = moveList.getMove(i).getSourceIndex();
    std::int8_t dst = moveList.getMove(i).getDestinationIndex();
    //Square src = moveList.getMove(i).getSourceSquare();
    //Square dst = moveList.getMove(i).getDestinationSquare();

    //if (src.row == 1 && src.col == 4 && dst.row == 3 && dst.col == 4)
    if (src == 12 && dst == 28)
    {
      selectedMove = moveList.getMove(i);
      found        = true;
      break;
    }
  }

  ASSERT_TRUE(found);

  board.makeMove(selectedMove);
  board.unmakeMove(selectedMove);

  EXPECT_EQ(board.getPieceType(1, 4), PieceType::WhitePawn);
  EXPECT_EQ(board.getPieceType(3, 4), PieceType::None);
}

TEST(
  BitBoardTests,
  MakeUnmakePreservesBoardState)
{
  BitBoard board;

  BoardState originalState = board.getBoardState();

  MoveList moveList;
  board.generateMoves(moveList);

  ASSERT_GT(moveList.totalMoves(), 0);

  Move move = moveList.getMove(0);

  board.makeMove(move);
  board.unmakeMove(move);

  BoardState restoredState = board.getBoardState();

  EXPECT_EQ(restoredState.sideToMove, originalState.sideToMove);
  EXPECT_EQ(restoredState.castlingRights, originalState.castlingRights);
  EXPECT_EQ(restoredState.enpassantColumn, originalState.enpassantColumn);
  EXPECT_EQ(restoredState.halfMoveClock, originalState.halfMoveClock);
  EXPECT_EQ(restoredState.fullMoveClock, originalState.fullMoveClock);
}

TEST(
  BitBoardTests,
  SlidingMovesGeneratedCorrectly)
{
  BitBoard board;

  board.setPosition("4k3/8/8/8/3R4/8/8/4K3 w - - 0 1");

  MoveList moveList;

  board.generateMoves(3, 3, moveList);

  EXPECT_TRUE(containsMove(moveList, 27, 24));
  EXPECT_TRUE(containsMove(moveList, 27, 31));
  EXPECT_TRUE(containsMove(moveList, 27, 3));
  EXPECT_TRUE(containsMove(moveList, 27, 59));
}

TEST(
  BitBoardTests,
  CaptureMovesGenerated)
{
  BitBoard board;

  board.setPosition("4k3/8/8/4p3/3R4/8/8/4K3 w - - 0 1");

  MoveList moveList;

  board.generateMoves(3, 3, moveList);

  EXPECT_TRUE(containsMove(moveList, 27, 35));
}
