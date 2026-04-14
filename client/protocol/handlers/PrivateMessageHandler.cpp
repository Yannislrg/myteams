/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** PrivateMessageHandler
*/

#include "PrivateMessageHandler.hpp"
#include "LoggingClientC.hpp"

void PrivateMessageHandler::handle(
    const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 4;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  (void)client_event_private_message_received(tokens[2].c_str(),
                                              tokens[3].c_str());
}
