#pragma once

namespace jmchess
{

enum class Color
{
  None = 0,
  White,
  Black
};

inline bool isOppositeColor(
  Color color1,
  Color color2)
{
  bool isFirstWhite  = (color1 == Color::White);
  bool isSecondWhite = (color2 == Color::White);
  bool isFirstBlack  = (color1 == Color::Black);
  bool isSecondBlack = (color2 == Color::Black);
  return ((isFirstWhite && isSecondBlack) || (isFirstBlack && isSecondWhite));
}

inline bool isSameColor(
  Color color1,
  Color color2)
{
  bool isFirstWhite  = (color1 == Color::White);
  bool isSecondWhite = (color2 == Color::White);
  bool isFirstBlack  = (color1 == Color::Black);
  bool isSecondBlack = (color2 == Color::Black);
  return ((isFirstWhite && isSecondWhite) || (isFirstBlack && isSecondBlack));
}

} // namespace jmchess
