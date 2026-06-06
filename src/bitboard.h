#pragma once

#include <bit>
#include <bitset>
#include <cstdint>
#include <map>
#include <ostream>
#include <string>

#include "board.h"
#include "board_state.h"
#include "cell.h"
#include "color.h"
#include "move.h"
#include "move_list.h"
#include "piece.h"
#include "piece_type.h"
#include "square.h"

namespace jmchess
{

class BitBoard : public Board
{
public:
  BitBoard();

  void generateMoves(MoveList & moveList) const override;

  void generateMoves(
    std::int8_t row,
    std::int8_t col,
    MoveList & moveList) const override;

  BoardState getBoardState() const override;

  std::int8_t getKingColumn(Color color) const override;

  std::int8_t getKingRow(Color color) const override;

  PieceType getPieceType(
    std::int8_t row,
    std::int8_t col) const override;

  bool isCellAttacked(
    std::int8_t row,
    std::int8_t col,
    Color attackingColor) const override;

  bool isKingInCheck(Color color) const override;

  bool makeMove(const Move & move) override;

  void reset() override;

  void setPosition(const std::string & fenString) override;

  void unmakeMove(const Move & move) override;

private:
  struct Attacks
  {
    std::uint64_t diagonalAttacks[4];
    std::uint64_t straightAttacks[4];
    std::uint64_t knightAttacks;
    std::uint64_t kingAttacks;
    std::uint64_t whitePawnAttacks;
    std::uint64_t blackPawnAttacks;
  };

  int bitScanForward(std::uint64_t bb) const;

  int bitScanReverse(std::uint64_t bb) const;

  void generateCastlingMoves(
    Color sideToMove,
    MoveList & moveList) const;

  template <bool Diagonal>
  void generateSlidingMoves(
    std::uint64_t pieces,
    Piece pieceType,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generateKingMoves(
    std::uint64_t kings,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generateKnightMoves(
    std::uint64_t knights,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generateMoves(
    std::int8_t index,
    MoveList & moveList) const;

  void generatePawnCapturesBlack(
    std::uint64_t pawns,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generatePawnPushesBlack(
    std::uint64_t pawns,
    MoveList & moveList) const;

  void generatePawnCapturesWhite(
    std::uint64_t pawns,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generatePawnPushesWhite(
    std::uint64_t pawns,
    MoveList & moveList) const;

  Piece getCapturedPiece(std::int8_t square) const;

  void initAttacks();

  bool isCellAttacked(
    std::int8_t index,
    Color attackingColor) const;

  void pushMove(
    std::int8_t fromIndex,
    std::int8_t toIndex,
    std::uint8_t flags,
    Piece capturePiece,
    MoveList & moveList) const;

  void updateAggregateBitboards();

  void writeBitBoard(
    std::uint64_t bb,
    std::ostream & output) const;

  BoardState mBoardState;
  Square mBlackKingSquare;
  Square mWhiteKingSquare;
  std::uint64_t mWhitePieces;
  std::uint64_t mBlackPieces;
  std::uint64_t mAllPieces;
  std::uint64_t mBitBoards[15];
  Attacks mAttacks[64];
  Cell mCells[64];
};

// // clang-format off
// static const int index64[64] = 
// {
//     0, 47,  1, 56, 48, 27,  2, 60,
//    57, 49, 41, 37, 28, 16,  3, 61,
//    54, 58, 35, 52, 50, 42, 21, 44,
//    38, 32, 29, 23, 17, 11,  4, 62,
//    46, 55, 26, 59, 40, 36, 15, 53,
//    34, 51, 20, 43, 31, 22, 10, 45,
//    25, 39, 14, 33, 19, 30,  9, 24,
//    13, 18,  8, 12,  7,  6,  5, 63
// };
// // clang-format on

/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
inline int BitBoard::bitScanForward(
  std::uint64_t bb) const
{
  //if (__cplusplus >= 202002L)
  //{
    return std::countr_zero(bb);
  //}
  //else
  //{
  //  const std::uint64_t debruijn64 = std::uint64_t(0x03f79d71b4cb0a89);
  //  return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
  //}
}

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
inline int BitBoard::bitScanReverse(
  std::uint64_t bb) const
{
  //if (__cplusplus >= 202002L)
  //{
    return 63 - std::countl_zero(bb);
  //}
  //else
  //{
  //  const std::uint64_t debruijn64 = std::uint64_t(0x03f79d71b4cb0a89);
  //  bb |= bb >> 1;
  //  bb |= bb >> 2;
  //  bb |= bb >> 4;
  //  bb |= bb >> 8;
  //  bb |= bb >> 16;
  //  bb |= bb >> 32;
  //  return index64[(bb * debruijn64) >> 58];
  //}
}

} // namespace jmchess
