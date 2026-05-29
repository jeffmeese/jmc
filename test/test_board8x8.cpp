// Board8x8_tests.cpp

#include <gtest/gtest.h>

#include "board8x8.h"
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
  }

  return false;
}

} // namespace

TEST(
  Board8x8Tests,
  ResetPlacesKingsCorrectly)
{
  Board8x8 board;

  EXPECT_EQ(board.getPieceType(0, 4), PieceType::WhiteKing);
  EXPECT_EQ(board.getPieceType(7, 4), PieceType::BlackKing);
}

TEST(
  Board8x8Tests,
  ResetPlacesPawnsCorrectly)
{
  Board8x8 board;

  for (int col = 0; col < 8; ++col)
  {
    EXPECT_EQ(board.getPieceType(1, col), PieceType::WhitePawn);
    EXPECT_EQ(board.getPieceType(6, col), PieceType::BlackPawn);
  }
}

TEST(
  Board8x8Tests,
  InitialPositionGenerates20Moves)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_EQ(moveList.totalMoves(), 20);
}

TEST(
  Board8x8Tests,
  WhitePawnSinglePushGenerated)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  EXPECT_TRUE(containsMove(moveList, 12, 20));
}

TEST(
  Board8x8Tests,
  WhitePawnDoublePushGenerated)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  EXPECT_TRUE(containsMove(moveList, 12, 28));
}

TEST(
  Board8x8Tests,
  KnightMovesGeneratedCorrectly)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(0, 1, moveList);

  EXPECT_TRUE(containsMove(moveList, 1, 16));
  EXPECT_TRUE(containsMove(moveList, 1, 18));
}

TEST(
  Board8x8Tests,
  BishopInitiallyBlocked)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(0, 2, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  Board8x8Tests,
  RookInitiallyBlocked)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(0, 0, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  Board8x8Tests,
  QueenInitiallyBlocked)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(0, 3, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  Board8x8Tests,
  KingInitiallyBlocked)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(0, 4, moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  Board8x8Tests,
  EmptyBoardHasNoMoves)
{
  Board8x8 board;
  board.setPosition("8/8/8/8/8/8/8/8 w - - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_EQ(moveList.totalMoves(), 0);
}

TEST(
  Board8x8Tests,
  WhiteKingAttackDetection)
{
  Board8x8 board;

  board.setPosition("8/8/8/8/4r3/8/4K3/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(1, 4, Color::Black));
}

TEST(
  Board8x8Tests,
  BlackKingAttackDetection)
{
  Board8x8 board;

  board.setPosition("8/8/8/B7/8/8/8/4k3 b - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(0, 4, Color::White));
}

TEST(
  Board8x8Tests,
  KnightAttackDetection)
{
  Board8x8 board;

  board.setPosition("8/8/8/8/3n4/8/4K3/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(1, 4, Color::Black));
}

TEST(
  Board8x8Tests,
  PawnAttackDetection)
{
  Board8x8 board;

  board.setPosition("8/8/8/8/3p4/4K3/8/8 w - - 0 1");

  EXPECT_TRUE(board.isCellAttacked(2, 4, Color::Black));
}

TEST(
  Board8x8Tests,
  CastlingGeneratedWhiteKingside)
{
  Board8x8 board;

  board.setPosition("4k3/8/8/8/8/8/8/4K2R w K - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_TRUE(containsMove(moveList, 4, 6));
}

TEST(
  Board8x8Tests,
  CastlingGeneratedWhiteQueenside)
{
  Board8x8 board;

  board.setPosition("4k3/8/8/8/8/8/8/R3K3 w Q - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_TRUE(containsMove(moveList, 4, 2));
}

TEST(
  Board8x8Tests,
  CastlingBlockedByAttack)
{
  Board8x8 board;

  board.setPosition("4k1r1/8/8/8/8/8/8/4K2R w K - 0 1");

  MoveList moveList;

  board.generateMoves(moveList);

  EXPECT_FALSE(containsMove(moveList, 4, 6));
}

TEST(
  Board8x8Tests,
  PawnPromotionGenerated)
{
  Board8x8 board;

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
  Board8x8Tests,
  EnPassantGenerated)
{
  Board8x8 board;

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
  Board8x8Tests,
  MakeMoveUpdatesBoard)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  Move selectedMove = moveList.getMove(0);

  bool found = false;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    std::int8_t src = moveList.getMove(i).getSourceIndex();
    std::int8_t dst = moveList.getMove(i).getDestinationIndex();
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
  Board8x8Tests,
  UnmakeMoveRestoresBoard)
{
  Board8x8 board;
  MoveList moveList;

  board.generateMoves(1, 4, moveList);

  Move selectedMove = moveList.getMove(0);

  bool found = false;

  for (std::int32_t i = 0; i < moveList.totalMoves(); ++i)
  {
    std::int8_t src = moveList.getMove(i).getSourceIndex();
    std::int8_t dst = moveList.getMove(i).getDestinationIndex();
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
  Board8x8Tests,
  MakeUnmakePreservesBoardState)
{
  Board8x8 board;

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
  Board8x8Tests,
  SlidingMovesGeneratedCorrectly)
{
  Board8x8 board;

  board.setPosition("4k3/8/8/8/3R4/8/8/4K3 w - - 0 1");

  MoveList moveList;

  board.generateMoves(3, 3, moveList);

  EXPECT_TRUE(containsMove(moveList, 27, 24));
  EXPECT_TRUE(containsMove(moveList, 27, 31));
  EXPECT_TRUE(containsMove(moveList, 27, 3));
  EXPECT_TRUE(containsMove(moveList, 27, 59));
}

TEST(
  Board8x8Tests,
  CaptureMovesGenerated)
{
  Board8x8 board;

  board.setPosition("4k3/8/8/4p3/3R4/8/8/4K3 w - - 0 1");

  MoveList moveList;

  board.generateMoves(3, 3, moveList);

  EXPECT_TRUE(containsMove(moveList, 27, 35));
}
