/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ChannelCreatedHandler
*/

#include "ChannelCreatedHandler.hpp"
#include "LoggingClientC.hpp"

void ChannelCreatedHandler::handle(
    const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 5;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  (void)client_event_channel_created(tokens[2].c_str(), tokens[3].c_str(),
                                     tokens[4].c_str());
}
