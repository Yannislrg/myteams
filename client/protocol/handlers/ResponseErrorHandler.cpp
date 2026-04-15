/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ResponseErrorHandler
*/

#include "ResponseErrorHandler.hpp"
#include <iostream>
#include "LoggingClientC.hpp"

void ResponseErrorHandler::handle(
    const std::vector<std::string>& tokens) const {
  if (_code == "401") {
    (void)client_error_unauthorized();
  } else if (_code == "409") {
    (void)client_error_already_exist();
  } else if (_code == "404" && tokens.size() >= 3) {
    std::cout << "Error: NOT_FOUND\n"; // TODO(lukas): more specific error messages based on context and tokens
  } else if (_code == "403") {
    std::cout << "Error: forbidden\n";
  } else if (_code == "400") {
    std::cout << "Error: bad request\n";
  }
}
