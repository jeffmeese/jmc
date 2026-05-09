#include "movelist.h"

namespace jmchess
{

MoveList::MoveList()
  : mTotalMoves(0)
{
}

void MoveList::addMove(std::unique_ptr<Move> newMove)
{
  mMoves[mTotalMoves++] = std::move(newMove);
}

void MoveList::clear()
{
  mTotalMoves = 0;
}

const Move * MoveList::operator[](uint8_t index) const
{
  return mMoves[index].get();
}

const Move * MoveList::moveAt(uint8_t index) const
{
  return mMoves[index].get();
}

// Yes, there's no error checking so this could end badly
void MoveList::removeLast()
{
  --mTotalMoves;
}

uint8_t MoveList::size() const
{
  return mTotalMoves;
}

}
