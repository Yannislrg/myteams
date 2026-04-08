/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Database
*/

#include "database/Database.hpp"
#include <string>
#include <vector>
#include "models/Channel.hpp"
#include "models/Team.hpp"
#include "models/Thread.hpp"
#include "models/User.hpp"

Database::Database() = default;

Database::~Database() = default;

User* Database::findUser(const std::string& uuid) {
  for (auto& user : _users) {
    if (user.getUuid() == uuid) {
      return &user;
    }
  }
  return nullptr;
}

User* Database::findUserByName(const std::string& name) {
  for (auto& user : _users) {
    if (user.getName() == name) {
      return &user;
    }
  }
  return nullptr;
}

Team* Database::findTeam(const std::string& uuid) {
  for (auto& team : _teams) {
    if (team.getUuid() == uuid) {
      return &team;
    }
  }
  return nullptr;
}

Channel* Database::findChannel(const std::string& teamUuid,
                               const std::string& channelUuid) {
  Team* team = findTeam(teamUuid);

  if (team == nullptr) {
    return nullptr;
  }
  for (auto& channel : team->getChannels()) {
    if (channel.getUuid() == channelUuid) {
      return &channel;
    }
  }
  return nullptr;
}

Thread* Database::findThread(const std::string& channelUuid,
                             const std::string& threadUuid) {
  for (auto& team : _teams) {
    for (auto& channel : team.getChannels()) {
      if (channel.getUuid() != channelUuid) {
        continue;
      }
      for (auto& thread : channel.getThreads()) {
        if (thread.getUuid() == threadUuid) {
          return &thread;
        }
      }
      return nullptr;
    }
  }
  return nullptr;
}
