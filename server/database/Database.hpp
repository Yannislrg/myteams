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
  Channel* findChannel(const std::string& teamUuid,
                       const std::string& channelUuid);
  Thread* findThread(const std::string& channelUuid,
                     const std::string& threadUuid);

  [[nodiscard]] std::vector<Thread> getAllThreads() const;
  [[nodiscard]] std::vector<Channel> getAllChannels() const;

  [[nodiscard]] std::vector<User>& getUsers() { return _users; }
  [[nodiscard]] const std::vector<User>& getUsers() const { return _users; }
  [[nodiscard]] std::vector<Team>& getTeams() { return _teams; }
  [[nodiscard]] const std::vector<Team>& getTeams() const { return _teams; }

 protected:
 private:
  std::vector<User> _users;
  std::vector<Team> _teams;
};
