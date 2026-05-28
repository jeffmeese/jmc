#pragma once

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

  std::int8_t getKingColumn(Color color) const;

  std::int8_t getKingRow(Color color) const;

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

  // void generateDiagonalMoves(
  //   std::uint64_t pieces,
  //   Piece pieceType,
  //   std::uint64_t enemyPieces,
  //   bool diagonal,
  //   MoveList & moveList) const;

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

  void generatePawnPushesBlack(MoveList & moveList) const;

  void generatePawnCapturesWhite(
    std::uint64_t pawns,
    std::uint64_t enemyPieces,
    MoveList & moveList) const;

  void generatePawnPushesWhite(MoveList & moveList) const;

  // void generateStraightMoves(
  //   std::uint64_t pieces,
  //   Piece pieceType,
  //   std::uint64_t enemyPieces,
  //   MoveList & moveList) const;

  Piece getCapturedPiece(std::int8_t square) const;

  constexpr std::int8_t getIndex(
    std::int8_t row,
    std::int8_t col) const;

  constexpr std::int8_t getRow(std::int8_t index) const;

  constexpr std::int8_t getCol(std::int8_t index) const;

  void initAttacks();

  bool isCellAttacked(
    std::int8_t index,
    Color attackingColor) const;

  void pushMove(
    std::int8_t fromIndex,
    std::int8_t toIndex,
    Piece piece,
    Piece capturePiece,
    Piece promotionPiece,
    Move::Type type,
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
  std::uint64_t mBitBoards[12];
  Attacks mAttacks[64];
  Cell mCells[64];
};

} // namespace jmchess
