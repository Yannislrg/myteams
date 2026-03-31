/*
** EPITECH PROJECT, 2026
** myteam
** File description:
** Thread
*/

#pragma once

#include <ctime>
#include <string>
#include <utility>
#include <vector>
#include "Reply.hpp"

class Thread {
 public:
  Thread();
  ~Thread();
  Thread(const Thread& other) = default;
  Thread& operator=(const Thread& other) = default;
  Thread(Thread&& other) = default;
  Thread& operator=(Thread&& other) = default;

  [[nodiscard]] const std::string& getUuid() const { return _uuid; }
  void setUuid(const std::string& uuid) { _uuid = uuid; }

  [[nodiscard]] const std::string& getUserUuid() const { return _userUuid; }
  void setUserUuid(const std::string& userUuid) { _userUuid = userUuid; }

  [[nodiscard]] time_t getTimestamp() const { return _timestamp; }
  void setTimestamp(time_t timestamp) { _timestamp = timestamp; }

  [[nodiscard]] const std::string& getTitle() const { return _title; }
  void setTitle(const std::string& title) { _title = title; }

  [[nodiscard]] const std::string& getBody() const { return _body; }
  void setBody(const std::string& body) { _body = body; }

  [[nodiscard]] const std::vector<Reply>& getReplies() const {
    return _replies;
  }
  void setReplies(std::vector<Reply>&& replies) {
    _replies = std::move(replies);
  }

 protected:
 private:
  std::string _uuid;
  std::string _userUuid;
  time_t _timestamp;
  std::string _title;
  std::string _body;
  std::vector<Reply> _replies;
};
