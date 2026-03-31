/*
** EPITECH PROJECT, 2026
** myteam
** File description:
** Reply
*/

#pragma once

#include <ctime>
#include <string>

class Reply {
 public:
  Reply();
  ~Reply();
  Reply(const Reply& other) = default;
  Reply& operator=(const Reply& other) = default;
  Reply(Reply&& other) = default;
  Reply& operator=(Reply&& other) = default;

  [[nodiscard]] const std::string& getUuid() const { return _uuid; }
  void setUuid(const std::string& uuid) { _uuid = uuid; }

  [[nodiscard]] const std::string& getUserUuid() const { return _userUuid; }
  void setUserUuid(const std::string& userUuid) { _userUuid = userUuid; }

  [[nodiscard]] time_t getTimestamp() const { return _timestamp; }
  void setTimestamp(time_t timestamp) { _timestamp = timestamp; }

  [[nodiscard]] const std::string& getBody() const { return _body; }
  void setBody(const std::string& body) { _body = body; }

 protected:
 private:
  std::string _uuid;
  std::string _userUuid;
  time_t _timestamp;
  std::string _body;
};
