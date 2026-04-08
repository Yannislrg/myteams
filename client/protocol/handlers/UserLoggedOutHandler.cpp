/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** UserLoggedOutHandler
*/

#include "UserLoggedOutHandler.hpp"
#include "LoggingClientC.hpp"

void UserLoggedOutHandler::handle(
    const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 4;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  (void)client_event_logged_out(tokens[2].c_str(), tokens[3].c_str());
}
