/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TcpClient
*/

#pragma once

#include <sys/types.h>
#include <cstddef>
#include <cstdint>
#include <string>

class TcpClient {
 public:
  TcpClient() = default;
  ~TcpClient() noexcept;

  TcpClient(const TcpClient& other) = delete;
  TcpClient& operator=(const TcpClient& other) = delete;
  TcpClient(TcpClient&& other) = delete;
  TcpClient& operator=(TcpClient&& other) = delete;

  void connectTo(const std::string& host, uint16_t port);
  void sendAll(const std::string& data) const;
  ssize_t receiveSome(char* buffer, std::size_t bufferSize) const;
  void disconnect() noexcept;

  [[nodiscard]] int getSocketFd() const { return _socketFd; }

 private:
  int _socketFd = -1;
};
