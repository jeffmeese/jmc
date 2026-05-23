#pragma once

#include <cstdint>

namespace jmchess
{

enum class Piece : std::int32_t
{
  Pawn   = 0,
  Knight = 1,
  Bishop = 2,
  Rook   = 3,
  Queen  = 4,
  King   = 5,
  None   = 6
};

} // namespace jmchess
