/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Database
*/

#include "Database.hpp"
#include <string>
#include "../models/Channel.hpp"
#include "../models/Team.hpp"
#include "../models/Thread.hpp"
#include "../models/User.hpp"

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

Channel* Database::findChannel(const std::string& team_uuid,
                               const std::string& channel_uuid) {
  Team* team = findTeam(team_uuid);

  if (team == nullptr) {
    return nullptr;
  }
  for (auto& channel : team->getChannels()) {
    if (channel.getUuid() == channel_uuid) {
      return &channel;
    }
  }
  return nullptr;
}

Thread* Database::findThread(const std::string& channel_uuid,
                             const std::string& thread_uuid) {
  for (auto& team : _teams) {
    for (auto& channel : team.getChannels()) {
      if (channel.getUuid() != channel_uuid) {
        continue;
      }
      for (auto& thread : channel.getThreads()) {
        if (thread.getUuid() == thread_uuid) {
          return &thread;
        }
      }
      return nullptr;
    }
  }
  return nullptr;
}
