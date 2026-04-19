/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** User
*/

#include "User.hpp"
#include <algorithm>

User::User() = default;

User::~User() = default;

bool User::addTeam(const std::string& teamUuid) {
  if (std::ranges::find(_teams, teamUuid) != _teams.end()) {
    return false;
  }
  _teams.push_back(teamUuid);
  return true;
}

bool User::removeTeam(const std::string& teamUuid) {
  const auto eraseFrom = std::ranges::remove(_teams, teamUuid);
  if (eraseFrom.begin() == eraseFrom.end()) {
    return false;
  }
  _teams.erase(eraseFrom.begin(), eraseFrom.end());
  return true;
}

void User::addMessage(const std::string& senderUuid, const Message& message) {
  _messages[senderUuid].push_back(message);
}
