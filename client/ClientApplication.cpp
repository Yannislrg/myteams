/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ClientApplication
*/

#include "ClientApplication.hpp"
#include <sys/poll.h>
#include <unistd.h>
#include <array>
#include <iostream>
#include <string>
#include <utility>

ClientApplication::ClientApplication(std::string host, uint16_t port)
    : _host(std::move(host)), _port(port) {}

uint16_t ClientApplication::parsePort(const char* portArg) {
  const uint64_t rawPort = std::stoull(portArg);
  if (rawPort == 0 || rawPort > MAX_PORT) {
    throw std::invalid_argument("Invalid port");
  }
  return static_cast<uint16_t>(rawPort);
}

void ClientApplication::run() {
  _client.connectTo(_host, _port);
  _poller.addFileDescriptor(STDIN_FILENO, POLLIN);
  _poller.addFileDescriptor(_client.getSocketFd(), POLLIN);
  runEventLoop();
  _client.disconnect();
}

bool ClientApplication::hasEvent(int revents, int mask) {
  return (static_cast<unsigned int>(revents) &
          static_cast<unsigned int>(mask)) != 0U;
}

bool ClientApplication::handleStdinEvent(const PollEvent& pollEvent) {
  if (hasEvent(pollEvent.revents, POLLERR | POLLHUP | POLLNVAL)) {
    return false;
  }
  if (!hasEvent(pollEvent.revents, POLLIN)) {
    return true;
  }

  std::string line;
  if (!std::getline(std::cin, line)) {
    return false;
  }
  line.append("\r\n");
  _client.sendAll(line);
  _client.flushPendingWrites();
  updateServerWatchedEvents();
  return true;
}

bool ClientApplication::handleServerEvent(const PollEvent& pollEvent) {
  if (hasEvent(pollEvent.revents, POLLERR | POLLHUP | POLLNVAL)) {
    return false;
  }

  if (hasEvent(pollEvent.revents, POLLOUT)) {
    if (!handleServerWritable()) {
      return false;
    }
  }
  if (hasEvent(pollEvent.revents, POLLIN)) {
    if (!handleServerReadable()) {
      return false;
    }
  }

  updateServerWatchedEvents();
  return true;
}

bool ClientApplication::handleServerReadable() {
  std::array<char, READ_BUFFER_SIZE> buffer{};
  const ssize_t bytesRead = _client.receiveSome(buffer.data(), buffer.size());
  if (bytesRead == 0) {
    return false;
  }
  if (bytesRead < 0) {
    return true;
  }

  _serverReadBuffer.append(buffer.data(), static_cast<std::size_t>(bytesRead));
  std::size_t lineEnd = _serverReadBuffer.find('\n');
  while (lineEnd != std::string::npos) {
    std::string frame = _serverReadBuffer.substr(0, lineEnd);
    if (!frame.empty() && frame.back() == '\r') {
      frame.pop_back();
    }
    _messageRouter.routeFrame(frame);
    _serverReadBuffer.erase(0, lineEnd + 1);
    lineEnd = _serverReadBuffer.find('\n');
  }
  return true;
}

bool ClientApplication::handleServerWritable() {
  _client.flushPendingWrites();
  return true;
}

void ClientApplication::updateServerWatchedEvents() {
  int events = POLLIN;
  if (_client.hasPendingWrites()) {
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    events |= POLLOUT;
  }
  _poller.updateWatchedEvents(_client.getSocketFd(), events);
}

void ClientApplication::runEventLoop() {
  bool running = true;
  while (running) {
    const auto events = _poller.waitForEvents(-1);
    for (const auto& pollEvent : events) {
      if (pollEvent.fd == STDIN_FILENO) {
        running = handleStdinEvent(pollEvent);
      } else if (pollEvent.fd == _client.getSocketFd()) {
        running = handleServerEvent(pollEvent);
      }
      if (!running) {
        break;
      }
    }
  }
}
