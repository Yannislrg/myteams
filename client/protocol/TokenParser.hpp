/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TokenParser
*/

#pragma once

#include <optional>
#include <string>
#include <vector>

class TokenParser {
 public:
  static std::optional<std::vector<std::string>> parse(
      const std::string& rawFrame);
};
