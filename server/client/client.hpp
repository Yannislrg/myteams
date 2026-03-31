/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** client
*/

#pragma once

#include <string>
#include "../Context.hpp"

class Client {
 public:
  Client();
  ~Client();
  Client(const Client& other) = delete;
  Client& operator=(const Client& other) = delete;
  Client(Client&& other) = delete;
  Client& operator=(Client&& other) = delete;

  [[nodiscard]] int getFd() const { return _fd; }
  void setFd(int fileDescriptor) { _fd = fileDescriptor; }

  [[nodiscard]] const std::string& getUserUuid() const { return _userUuid; }
  void setUserUuid(const std::string& userUuid) { _userUuid = userUuid; }

  [[nodiscard]] const Context& getContext() const { return _context; }
  void setContext(const Context& context) { _context = context; }

  [[nodiscard]] const std::string& getReadBuffer() const { return _readBuffer; }
  void setReadBuffer(const std::string& readBuffer) {
    _readBuffer = readBuffer;
  }

  [[nodiscard]] const std::string& getWriteBuffer() const {
    return _writeBuffer;
  }
  void setWriteBuffer(const std::string& writeBuffer) {
    _writeBuffer = writeBuffer;
  }

 protected:
 private:
  int _fd;
  std::string _userUuid;
  Context _context;
  std::string _readBuffer;
  std::string _writeBuffer;
};
