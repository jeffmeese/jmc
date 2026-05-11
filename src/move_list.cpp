#include "move_list.h"

#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

namespace jmchess
{

MoveList::MoveList()
{
  mMoves.reserve(MAX_MOVES);
}

void MoveList::addMove(const Move & move)
{
  if (mIndex >= MAX_MOVES)
  {
    std::ostringstream oss;
    oss << "Move list already contains the maximum number of moves allowed";
    throw std::runtime_error(oss.str());
  } 

  mMoves.push_back(move);
  mIndex++;
}

void MoveList::clear()
{
  mIndex = 0;
  mMoves.clear();
  mMoves.reserve(MAX_MOVES);
}

const Move & MoveList::getMove(std::int32_t index) const
{
  if (index < 0 || index >= mIndex)
  {
    std::ostringstream oss;
    oss << "Requested move index is beyond stored moves";
    throw std::runtime_error(oss.str());
  }

  return mMoves[index];
}

std::int32_t MoveList::getMoveIndex(const Move & move) const
{
  for (std::int32_t i = 0; i <= mIndex; i++)
  {
    const Move & thisMove = mMoves[i];
    if (thisMove == move)
    {
      return i;
    }
  }

  return -1;
}

void MoveList::removeLast()
{
  if (mIndex == 0)
  {
    return;
  }

  mIndex--;
}

void MoveList::sort()
{
  // MoveList newList;
  // for (std::int32_t i = 0; i < mIndex; i++)
  // {
  //   newList.mMoves[i] = mMoves[i];
  // }
  // newList.mIndex = mIndex;
  // return newList;
}

std::int32_t MoveList::totalMoves() const
{
  return mIndex;
}

}