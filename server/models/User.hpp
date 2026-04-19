/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** User
*/

#pragma once

#include <map>
#include <string>
#include <vector>
#include "Message.hpp"

class User {
 public:
  User();
  ~User();
  User(const User& other) = default;
  User& operator=(const User& other) = default;
  User(User&& other) = default;
  User& operator=(User&& other) = default;

  [[nodiscard]] const std::string& getUuid() const { return _uuid; }
  void setUuid(const std::string& uuid) { _uuid = uuid; }

  [[nodiscard]] const std::string& getName() const { return _name; }
  void setName(const std::string& name) { _name = name; }

  [[nodiscard]] bool isConnected() const { return _connectedCount > 0; }
  [[nodiscard]] int getConnectionCount() const { return _connectedCount; }
  void setConnectionCount(int count) {
    _connectedCount = count < 0 ? 0 : count;
  }
  void incrementConnection() { _connectedCount++; }
  void decrementConnection() {
    if (_connectedCount > 0) {
      _connectedCount--;
    }
  }

  [[nodiscard]] const std::vector<std::string>& getTeams() const {
    return _teams;
  }
  void setTeams(const std::vector<std::string>& teams) { _teams = teams; }
  [[nodiscard]] bool addTeam(const std::string& teamUuid);
  [[nodiscard]] bool removeTeam(const std::string& teamUuid);

  [[nodiscard]] const std::map<std::string, std::vector<Message>>& getMessages()
      const {
    return _messages;
  }
  void setMessages(
      const std::map<std::string, std::vector<Message>>& messages) {
    _messages = messages;
  }
  void addMessage(const std::string& senderUuid, const Message& message);

 private:
  std::string _uuid;
  std::string _name;
  int _connectedCount = 0;
  std::vector<std::string> _teams;
  std::map<std::string, std::vector<Message>> _messages;
};
