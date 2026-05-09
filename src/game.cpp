#include "game.h"

#include <memory>

#include "board.h"
#include "board8x8.h"

namespace jmchess
{

Game::Game()
{
  mBoard.reset(new Board8x8);
  mEvaluation.reset(new Evaluation);
  mSearch.reset(new Search);
  mEngine.reset(new Engine);
}

Board * Game::getBoard()
{
  return mBoard.get();
}

const Board * Game::getBoard() const
{
  return mBoard.get();
}

void Game::makeMove(const Move & move)
{
  mBoard->makeMove(move);
  mMoves.push(move);
}

void Game::startNew()
{
  mBoard->reset();
  while (!mMoves.empty())
  {
    mMoves.pop();
  }
}

void Game::undoLastMove()
{
  if (mMoves.size() == 0)
  {
    return;
  }

  Move move = mMoves.top();
  mBoard->unmakeMove(move);
  mMoves.pop();
}

}