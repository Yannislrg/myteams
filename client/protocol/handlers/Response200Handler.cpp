/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response200Handler
*/

#include "Response200Handler.hpp"
#include <ctime>
#include <string>
#include <vector>
#include "LoggingClientC.hpp"

static constexpr std::size_t UserMinTokens = 5;
static constexpr std::size_t InfoMinTokens = 6;
static constexpr std::size_t ThreadMinTokens = 8;
static constexpr std::size_t SubMinTokens = 4;

void Response200Handler::handle(const std::vector<std::string>& tokens) const {
  if (tokens.size() < 2) {
    return;
  }
  const std::string& sub = tokens[1];

  if (sub == "USER" && tokens.size() >= UserMinTokens) {
    (void)client_print_user(tokens[2].c_str(), tokens[3].c_str(),
                            std::stoi(tokens[4]));
    return;
  }

  if (sub == "SUBSCRIBE" && tokens.size() >= SubMinTokens) {
    (void)client_print_subscribed(tokens[2].c_str(), tokens[3].c_str());
    return;
  }

  if (sub == "UNSUBSCRIBE" && tokens.size() >= SubMinTokens) {
    (void)client_print_unsubscribed(tokens[2].c_str(), tokens[3].c_str());
    return;
  }

  if (sub == "INFO" && tokens.size() >= 3) {
    const std::string& type = tokens[2];
    if (type == "USER" && tokens.size() >= InfoMinTokens) {
      (void)client_print_user(
          tokens[3].c_str(), tokens[4].c_str(),
          std::stoi(
              tokens[5]));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    } else if (type == "TEAM" && tokens.size() >= InfoMinTokens) {
      (void)client_print_team(
          tokens[3].c_str(), tokens[4].c_str(),
          tokens[5] // NOLINT(cppcoreguidelines-avoid-magic-numbers)
              .c_str());
    } else if (type == "CHANNEL" && tokens.size() >= InfoMinTokens) {
      (void)client_print_channel(
          tokens[3].c_str(), tokens[4].c_str(),
          tokens[5] // NOLINT(cppcoreguidelines-avoid-magic-numbers)
              .c_str());
    } else if (type == "THREAD" && tokens.size() >= ThreadMinTokens) {
      const auto timestamp = static_cast<time_t>(std::stoll(tokens[5]));
      (void)client_print_thread(
          tokens[3].c_str(), tokens[4].c_str(), timestamp, tokens[6].c_str(), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
          tokens[7].c_str());  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    }
    return;
  }
}
