/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** utils
*/

#include "utils.hpp"
#include <string>

std::string Utils::quoteProtocolField(const std::string& value) {
  std::string escaped;
  escaped.reserve(value.size());
  for (const char character : value) {
    if (character == '\\' || character == '"') {
      escaped += '\\';
    }
    escaped += character;
  }
  return "\"" + escaped + "\"";
}
