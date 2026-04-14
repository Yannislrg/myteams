/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Team
*/

#include "Team.hpp"

Team::Team() = default;

Team::~Team() = default;

bool Team::addSubscriber(const std::string& userUuid) {
  if (isUserSubscribed(userUuid)) {
    return false;
  }
  _subscriberUuids.push_back(userUuid);
  return true;
}

bool Team::removeSubscriber(const std::string& userUuid) {
  const auto eraseFrom = std::ranges::remove(_subscriberUuids, userUuid);
  if (eraseFrom.begin() == eraseFrom.end()) {
    return false;
  }
  _subscriberUuids.erase(eraseFrom.begin(), eraseFrom.end());
  return true;
}
