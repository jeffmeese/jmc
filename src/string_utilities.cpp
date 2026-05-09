#include "string_utilities.h"

#include <cstddef>
#include <string>
#include <vector>

namespace jmchess
{

std::string trimString(
  const std::string & s)
{
  const std::string WHITESPACE = " \n\r\t\f\v";
  size_t start                 = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start, s.find_last_not_of(WHITESPACE) - start + 1);
}

std::vector<std::string> splitString(
  const std::string & s,
  const std::string & delim,
  bool allowEmpties)
{
  std::vector<std::string> tokens;
  if (s.empty() || delim.empty())
  {
    return tokens;
  }

  size_t current;
  size_t next = -1;
  do
  {
    if (!allowEmpties)
    {
      next = s.find_first_not_of(delim, next + 1);
      if (next == std::string::npos)
        break;
      next -= 1;
    }
    current = next + 1;
    next    = s.find_first_of(delim, current);
    tokens.push_back(s.substr(current, next - current));
  } while (next != std::string::npos);

  return tokens;
}

std::vector<std::string> splitString(const std::string & s, const std::string & delim)
{
  std::vector<std::string> tokens;

  std::size_t startPos = 0;
  while (true)
  {
    std::size_t endPos = s.find_first_of(delim, startPos);
    if (endPos == std::string::npos)
    {
      tokens.push_back(s.substr(startPos, endPos-startPos));
      break;
    }

    tokens.push_back(s.substr(startPos, endPos-startPos));
    startPos = endPos + 1;
  }

  return tokens;
}

}