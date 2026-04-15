/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response201Handler
*/

#include "Response201Handler.hpp"
#include <ctime>
#include <string>
#include <vector>
#include "LoggingClientC.hpp"

static constexpr std::size_t TeamChanMinTokens = 6;
static constexpr std::size_t ThreadMinTokens = 8;
static constexpr std::size_t ReplyMinTokens = 7;
static constexpr std::size_t DescIndex = 5;
static constexpr std::size_t ThreadTitleIndex = 6;
static constexpr std::size_t ThreadBodyIndex = 7;

void Response201Handler::handle(const std::vector<std::string>& tokens) const {
  if (tokens.size() < 3) {
    return;
  }
  const std::string& type = tokens[2];

  if (type == "TEAM" && tokens.size() >= TeamChanMinTokens) {
    (void)client_print_team_created(tokens[3].c_str(), tokens[4].c_str(),
                                    tokens[DescIndex].c_str());
  } else if (type == "CHANNEL" && tokens.size() >= TeamChanMinTokens) {
    (void)client_print_channel_created(tokens[3].c_str(), tokens[4].c_str(),
                                       tokens[DescIndex].c_str());
  } else if (type == "THREAD" && tokens.size() >= ThreadMinTokens) {
    const auto timestamp = static_cast<time_t>(std::stoll(tokens[DescIndex]));
    (void)client_print_thread_created(
        tokens[3].c_str(), tokens[4].c_str(), timestamp,
        tokens[ThreadTitleIndex].c_str(), tokens[ThreadBodyIndex].c_str());
  } else if (type == "REPLY" && tokens.size() >= ReplyMinTokens) {
    const auto timestamp = static_cast<time_t>(std::stoll(tokens[DescIndex]));
    (void)client_print_reply_created(tokens[3].c_str(), tokens[4].c_str(),
                                     timestamp,
                                     tokens[ThreadTitleIndex].c_str());
  }
}
