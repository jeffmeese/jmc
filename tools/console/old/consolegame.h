#ifndef CONSOLEGAME_H
#define CONSOLEGAME_H

#include <cstdint>
#include <sstream>

#include "board.h"

class ConsoleGame
{
public:
  ConsoleGame(jmchess::Board * board);

  int execute();

private:
  void doMove(const jmchess::Move * move) const;
  int getMoveIndex(uint8_t srcRow, uint8_t srcCol, uint8_t dstRow, uint8_t dstCol, const jmchess::MoveList & moveList) const;
  void handleMove(std::istringstream & iss) const;
  void handlePrint() const;
  void handleShow() const;
  bool parseMovePos(const std::string &moveString, uint8_t & srcRow, uint8_t & srcCol, uint8_t & dstRow, uint8_t & dstCol) const;

  jmchess::Board * mBoard;
};

#endif // #ifndef CONSOLEGAME_H
