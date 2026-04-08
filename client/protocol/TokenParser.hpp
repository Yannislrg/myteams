/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TokenParser
*/

#pragma once

#include <string>
#include <vector>

class TokenParser {
 public:
  static std::vector<std::string> parse(const std::string& rawFrame);
};
