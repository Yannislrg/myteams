/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** client
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "Context.hpp"

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

  [[nodiscard]] std::string& getReadBuffer() { return _readBuffer; }
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

  [[nodiscard]] bool isPollOutEnabled() const { return _pollOutEnabled; }
  void setPollOutEnabled(bool enabled) { _pollOutEnabled = enabled; }

  void appendToReadBuffer(const char* data, std::size_t len) {
    _readBuffer.append(data, len);
  }
  void consumeFromReadBuffer(std::size_t n) { _readBuffer.erase(0, n); }

  void appendToWriteBuffer(const std::string& data) { _writeBuffer += data; }
  void consumeFromWriteBuffer(std::size_t n) { _writeBuffer.erase(0, n); }

  [[nodiscard]] const std::vector<std::string>& getArgs() const {
    return _args;
  }
  void setArgs(const std::vector<std::string>& args) { _args = args; }

 protected:
 private:
  int _fd;
  std::string _userUuid;
  Context _context;
  std::string _readBuffer;
  std::string _writeBuffer;
  std::vector<std::string> _args;
  bool _pollOutEnabled{false};
};
