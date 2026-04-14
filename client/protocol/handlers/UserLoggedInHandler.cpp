/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** UserLoggedInHandler
*/

#include "UserLoggedInHandler.hpp"
#include "LoggingClientC.hpp"

void UserLoggedInHandler::handle(const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 4;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  (void)client_event_logged_in(tokens[2].c_str(), tokens[3].c_str());
}
