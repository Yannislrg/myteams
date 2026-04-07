/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ClientApplication
*/

#pragma once

#include <cstdint>
#include <string>
#include "network/Poller.hpp"
#include "network/TcpClient.hpp"

class ClientApplication {
 public:
  ClientApplication(std::string host, uint16_t port);
  static uint16_t parsePort(const char* portArg);
  void run();

 private:
  static bool hasEvent(int revents, int mask);
  bool handleStdinEvent(const PollEvent& pollEvent);
  bool handleServerEvent(const PollEvent& pollEvent);
  void runEventLoop();

  std::string _host;
  uint16_t _port;
  TcpClient _client;
  Poller _poller;

  static constexpr uint64_t MAX_PORT = 65535;
  static constexpr std::size_t READ_BUFFER_SIZE = 4096;
};
