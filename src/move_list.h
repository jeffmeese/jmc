#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "move.h"

namespace jmchess
{

class MoveList
{
public:
  static constexpr std::int32_t MAX_MOVES = 256;

  MoveList();
  MoveList(const MoveList & moveList) = delete;
  MoveList & operator=(const MoveList & moveList) = delete;

  void addMove(const Move & move);
  void clear();
  const Move & getMove(std::int32_t index) const;
  std::int32_t getMoveIndex(const Move & move) const;
  void removeLast();
  void sort();
  std::int32_t totalMoves() const;

private:
  std::vector<Move> mMoves;
  std::int32_t mIndex = 0;
};

} // namespace jmchess
