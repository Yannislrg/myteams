/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Database
*/

#pragma once

#include <string>
#include <vector>
#include "../models/Channel.hpp"
#include "../models/Team.hpp"
#include "../models/Thread.hpp"
#include "../models/User.hpp"

class Database {
 public:
  Database();
  ~Database();
  Database(const Database& other) = delete;
  Database& operator=(const Database& other) = delete;
  Database(Database&& other) = delete;
  Database& operator=(Database&& other) = delete;

  User* findUser(const std::string& uuid);
  User* findUserByName(const std::string& name);
  Team* findTeam(const std::string& uuid);
  Channel* findChannel(const std::string& team_uuid,
                       const std::string& channel_uuid);
  Thread* findThread(const std::string& channel_uuid,
                     const std::string& thread_uuid);

 protected:
 private:
  std::vector<User> _users;
  std::vector<Team> _teams;
};
