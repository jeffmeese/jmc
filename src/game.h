#pragma once

#include <memory>
#include <stack>

#include "board.h"
#include "engine.h"

namespace jmchess
{

class Game
{
public:
  Game();
  
  Board * getBoard();
  const Board * getBoard() const;

  Engine * getEngine();
  const Engine * getEngine() const;

  void makeMove(const Move & move);
  void startNew();
  void undoLastMove();

private:
  std::unique_ptr<Board> mBoard;
  std::unique_ptr<Engine> mEngine;
  std::stack<Move> mMoves;
};

}