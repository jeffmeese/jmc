#ifndef MOVELIST_H
#define MOVELIST_H

#include <cstdint>
#include <memory>

#include "move.h"

namespace jmchess
{

class MoveList
{
public:
  MoveList();

public:
  uint8_t size() const;

public:
  void addMove(std::unique_ptr<Move> newMove);
  void clear();
  const Move * moveAt(uint8_t index) const;
  void print(std::ostream & output) const;
  void removeLast();

public:
  const Move * operator[](uint8_t index) const;

private:
  std::unique_ptr<Move> mMoves[256];
  uint8_t mTotalMoves;
};

}

#endif // MOVELIST_H
