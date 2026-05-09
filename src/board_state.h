#pragma once

#include <cstdint>

#include "color.h"

namespace jmchess
{

struct BoardState
{
  std::uint8_t castlingRights  = 0;
  std::uint8_t enpassantColumn = 8;
  std::uint8_t halfMoveClock   = 0;
  std::uint8_t fullMoveClock   = 0;
  Color sideToMove             = Color::White;
};

}