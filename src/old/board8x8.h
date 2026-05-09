#ifndef BOARD8x8_H
#define BOARD8x8_H

#include "board.h"
#include <map>

namespace jmchess
{

class Board8x8
    : public Board
{
public:
  Board8x8();

  bool generateMoves(MoveList & moveList) const override;
  bool generateMoves(uint8_t row, uint8_t col, MoveList & moveList) const override;
  PieceType getPieceType(uint8_t row, uint8_t col) const override;
  bool isCellAttacked(uint8_t row, uint8_t col) const override;
  bool makeMove(const Move * move) override;
  bool unmakeMove(const Move * move) override;

protected:
  void doReset() override;

private:
  bool generateMoves(uint8_t index, MoveList & moveList) const;
  void generatePawnMoves(uint8_t index, Color sideToMove, MoveList & moveList) const;
  void init();
  void pushMove(uint8_t fromSquare, uint8_t toSquare, Piece piece, Piece capturePiece, Piece promotionPiece, Move::Type type, MoveList & moveList) const;

  Piece mPieces[64];
  Color mColors[64];
  std::map<Color, int8_t> mKingCol;
  std::map<Color, int8_t> mKingRow;
};

}

#endif // #ifndef BOARD8x8_H
