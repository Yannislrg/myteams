/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ReplyCreatedHandler
*/

#include "ReplyCreatedHandler.hpp"
#include "LoggingClientC.hpp"

void ReplyCreatedHandler::handle(const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 6;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  (void)client_event_thread_reply_received(tokens[2].c_str(), tokens[3].c_str(),
                                           tokens[4].c_str(), tokens[5].c_str());
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
