#pragma once

#include <memory>
#include <stack>

#include "board.h"
#include "engine.h"
#include "search.h"

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

  Search * getSearch();

  const Search * getSearch() const;

  bool isCheckmate() const;

  bool isStalemate() const;

  void makeMove(const Move & move);

  void performEngineCommand();
  
  void setBoard(std::unique_ptr<Board> board);

  void setSearch(std::unique_ptr<Search> search);

  void startNew();

  void undoLastMove();

private:
  bool mCheckmate = false;
  bool mStalemate = false;
  std::unique_ptr<Board> mBoard;
  std::unique_ptr<Engine> mEngine;
  std::unique_ptr<Search> mSearch;
  std::stack<Move> mMoves;
};

}