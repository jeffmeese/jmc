#pragma once

#include <istream>

#include "game.h"

class ConsoleGame : public jmchess::Game
{
public:
  void execute();

private:
  void handleDivideCommand(std::istream & input);
  void handleHelpCommand() const;
  void handleMoveCommand(std::istream & input);
  void handleNewCommand();
  void handlePerftCommand(std::istream & input);
  void handlePrintCommand();
  void handleSetBoardCommand(std::istream & input);
  void handleShowCommand();
  void handleTestMoveGen();
  void handleUndoCommand();

  bool parseMovePos(
    const std::string & moveString,
    uint8_t & srcRow,
    uint8_t & srcCol,
    uint8_t & dstRow,
    uint8_t & dstCol) const;

  int32_t readLevel(std::istream & iss) const;
};
