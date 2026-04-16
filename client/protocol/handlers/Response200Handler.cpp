/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Response200Handler
*/

#include "Response200Handler.hpp"
#include <ctime>
#include <exception>
#include <string>
#include <vector>
#include "LoggingClientC.hpp"

Response200Handler::Response200Handler(User& user, bool& shouldDisconnect)
    : _user(user), _shouldDisconnect(shouldDisconnect) {}

static constexpr std::size_t UserMinTokens = 5;
static constexpr std::size_t InfoMinTokens = 6;
static constexpr std::size_t ThreadMinTokens = 8;
static constexpr std::size_t SubMinTokens = 4;

void Response200Handler::handle(const std::vector<std::string>& tokens) const {
  if (tokens.size() < 2) {
    return;
  }
  const std::string& sub = tokens[1];

  if (sub == "OK") {
    constexpr std::size_t LoginTokens = 4;
    if (tokens.size() >= LoginTokens) {
      _user.login(tokens[2], tokens[3]);
    } else {
      _user.logout();
      _shouldDisconnect = true;
    }
    return;
  }

  if (sub == "USER" && tokens.size() >= UserMinTokens) {
    int status = 0;
    try {
      status = std::stoi(tokens[4]);
    } catch (const std::exception&) {
      return;
    }
    (void)client_print_user(tokens[2].c_str(), tokens[3].c_str(), status);
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
      int status = 0;
      try {
        status = std::stoi(tokens[5]);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      } catch (const std::exception&) {
        return;
      }
      (void)client_print_user(tokens[3].c_str(), tokens[4].c_str(), status);
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
      time_t timestamp = 0;
      try {
        timestamp = static_cast<time_t>(std::stoll(tokens[5]));  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
      } catch (const std::exception&) {
        return;
      }
      (void)client_print_thread(
          tokens[3].c_str(), tokens[4].c_str(), timestamp, tokens[6].c_str(), // NOLINT(cppcoreguidelines-avoid-magic-numbers)
          tokens[7].c_str());  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
    }
    return;
  }
}
