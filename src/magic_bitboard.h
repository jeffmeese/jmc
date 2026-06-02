#pragma once

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

class MagicBitboard : public Board
{
public:
  MagicBitboard();

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

  void generateMoves(
    std::int8_t index,
    MoveList & moveList) const;
    
  void initAttacks();

  bool isCellAttacked(
    std::int8_t index,
    Color attackingColor) const;

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
