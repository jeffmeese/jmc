#ifndef CONSTS_H
#define CONSTS_H

#include <cstdint>

namespace jmchess
{

constexpr uint8_t INVALID_EP_COLUMN   = 8;
constexpr uint8_t CASTLE_NONE         = 0x00;
constexpr uint8_t CASTLE_WHITE_KING   = 0x01;
constexpr uint8_t CASTLE_WHITE_QUEEN  = 0x02;
constexpr uint8_t CASTLE_BLACK_KING   = 0x03;
constexpr uint8_t CASTLE_BLACK_QUEEN  = 0x04;
constexpr uint8_t WHITE_PROMOTION_ROW = 7;
constexpr uint8_t BLACK_PROMOTION_ROW = 0;

}
#endif // #ifndef CONSTS_H
