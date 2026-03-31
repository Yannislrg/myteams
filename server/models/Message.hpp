/*
** EPITECH PROJECT, 2026
** myteam
** File description:
** Message
*/

#pragma once

#include <ctime>
#include <string>

class Message {
 public:
  Message();
  ~Message();
  Message(const Message& other) = delete;
  Message& operator=(const Message& other) = delete;
  Message(Message&& other) = delete;
  Message& operator=(Message&& other) = delete;

  [[nodiscard]] const std::string& getSenderUuid() const { return _senderUuid; }
  void setSenderUuid(const std::string& senderUuid) {
    _senderUuid = senderUuid;
  }

  [[nodiscard]] const std::string& getReceiverUuid() const {
    return _receiverUuid;
  }
  void setReceiverUuid(const std::string& receiverUuid) {
    _receiverUuid = receiverUuid;
  }

  [[nodiscard]] time_t getTimestamp() const { return _timestamp; }
  void setTimestamp(time_t timestamp) { _timestamp = timestamp; }

  [[nodiscard]] const std::string& getBody() const { return _body; }
  void setBody(const std::string& body) { _body = body; }

 protected:
 private:
  std::string _senderUuid;
  std::string _receiverUuid;
  time_t _timestamp;
  std::string _body;
};
