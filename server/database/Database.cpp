/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Database
*/

#include "Database.hpp"

Database::Database() = default;

Database::~Database() = default;

User* Database::findUser(const std::string& uuid) {
  (void)uuid;
  (void)_users;
  return nullptr;
}

User* Database::findUserByName(const std::string& name) {
  (void)name;
  (void)_users;
  return nullptr;
}

Team* Database::findTeam(const std::string& uuid) {
  (void)uuid;
  (void)_teams;
  return nullptr;
}

Channel* Database::findChannel(const std::string& teamUuid,
                               const std::string& channelUuid) {
  (void)teamUuid;
  (void)channelUuid;
  (void)_teams;
  return nullptr;
}

Thread* Database::findThread(const std::string& channelUuid,
                             const std::string& threadUuid) {
  (void)channelUuid;
  (void)threadUuid;
  (void)_teams;
  return nullptr;
}
