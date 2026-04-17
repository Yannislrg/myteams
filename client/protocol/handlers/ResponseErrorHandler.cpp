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
  } else if (_code == "404") {
    if (tokens.size() >= 4) {
      const std::string& type = tokens[2];
      const std::string& uuid = tokens[3];
      if (type == "TEAM") {
        (void)client_error_unknown_team(uuid.c_str());
      } else if (type == "CHANNEL") {
        (void)client_error_unknown_channel(uuid.c_str());
      } else if (type == "THREAD") {
        (void)client_error_unknown_thread(uuid.c_str());
      } else if (type == "USER") {
        (void)client_error_unknown_user(uuid.c_str());
      } else {
        std::cout << "Error: NOT_FOUND\n";
      }
    } else if (tokens.size() >= 3) {
      if (_pendingCommand == "/user" || _pendingCommand == "/send" ||
          _pendingCommand == "/messages") {
        (void)client_error_unknown_user(tokens[2].c_str());
      } else {
        std::cout << "Error: NOT_FOUND " << tokens[2] << '\n';
      }
    } else {
      std::cout << "Error: NOT_FOUND\n";
    }
  } else if (_code == "403") {
    (void)client_error_unauthorized();
  } else if (_code == "400") {
    std::cout << "Error: bad request\n";
  }
}
