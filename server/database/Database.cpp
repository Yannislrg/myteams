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

Channel* Database::findChannel(const std::string& team_uuid,
                               const std::string& channel_uuid) {
  (void)team_uuid;
  (void)channel_uuid;
  (void)_teams;
  return nullptr;
}

Thread* Database::findThread(const std::string& channel_uuid,
                             const std::string& thread_uuid) {
  (void)channel_uuid;
  (void)thread_uuid;
  (void)_teams;
  return nullptr;
}
