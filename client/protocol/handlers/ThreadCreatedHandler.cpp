/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ThreadCreatedHandler
*/

#include "ThreadCreatedHandler.hpp"
#include <exception>
#include <string>
#include "LoggingClientC.hpp"

void ThreadCreatedHandler::handle(
    const std::vector<std::string>& tokens) const {
  constexpr std::size_t EXPECTED_TOKENS = 7;
  if (tokens.size() < EXPECTED_TOKENS) {
    return;
  }
  time_t timestamp = 0;
  try {
    timestamp = static_cast<time_t>(std::stoll(tokens[4]));
  } catch (const std::exception&) {
    return;
  }
  // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
  (void)client_event_thread_created(tokens[2].c_str(), tokens[3].c_str(),
                                    timestamp, tokens[5].c_str(),
                                    tokens[6].c_str());
  // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
}
