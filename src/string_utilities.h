#pragma once

#include <string>
#include <vector>

namespace jmchess
{
std::string trimString(const std::string & s);

std::vector<std::string> splitString(
  const std::string & s,
  const std::string & delim,
  bool allowEmpties);

std::vector<std::string> splitString(
  const std::string & s,
  const std::string & delim = " ");
  
} // namespace jmchess
