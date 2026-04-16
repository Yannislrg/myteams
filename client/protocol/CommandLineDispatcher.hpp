/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** CommandLineDispatcher
*/

#pragma once

#include <iosfwd>
#include <optional>
#include <string>
#include <string_view>

class CommandLineDispatcher {
 public:
  static std::string trimLeft(const std::string& line);
  static std::optional<std::string> extractCommand(const std::string& line);
  static bool isSupportedServerCommand(std::string_view command);
  static std::optional<std::string> validateArgs(std::string_view command,
                                                 std::string_view argsView);
  static void printHelp(std::ostream& output);
};
