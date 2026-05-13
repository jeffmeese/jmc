#include "engine.h"

namespace jmchess
{

Engine::Engine()
  : mSearch(new Search)
{
}

Move Engine::getMove(
  Board * board) const
{
  return mSearch->execute(board, board->getBoardState().sideToMove, mSearchDepth);
}

} // namespace jmchess
