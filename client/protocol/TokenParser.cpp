/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TokenParser
*/

#include "TokenParser.hpp"
#include <sstream>

std::optional<std::vector<std::string>> TokenParser::parse(
    const std::string& rawFrame) {
  std::vector<std::string> tokens;
  std::istringstream stream(rawFrame);
  std::string word;

  while (stream >> word) {
    if (word.front() == '"') {
      std::string quoted = word.substr(1);
      if (!quoted.empty() && quoted.back() == '"') {
        quoted.pop_back();
        tokens.push_back(quoted);
        continue;
      }
      std::string nextWord;
      bool closedQuote = false;
      while (stream >> nextWord) {
        quoted += " " + nextWord;
        if (!nextWord.empty() && nextWord.back() == '"') {
          quoted.pop_back();
          closedQuote = true;
          break;
        }
      }
      if (!closedQuote) {
        return std::nullopt;
      }
      tokens.push_back(quoted);
      continue;
    }
    tokens.push_back(word);
  }
  return tokens;
}
