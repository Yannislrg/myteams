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
#include "CommandLineDispatcher.hpp"
#include "LoggingClientC.hpp"
#include "sys/ClientError.hpp"

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

void ClientApplication::handleConnectionLost() {
  if (!_messageRouter.hasLoggedUser() || _messageRouter.shouldDisconnect()) {
    return;
  }
  (void)client_event_logged_out(_messageRouter.getLoggedUserUuid().c_str(),
                                _messageRouter.getLoggedUserName().c_str());
  _messageRouter.clearLoggedUser();
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
    _poller.removeFileDescriptor(STDIN_FILENO);
    return true;
  }
  if (!line.empty() && line.back() == '\r') {
    line.pop_back();
  }

  return dispatchCommandLine(line);
}

bool ClientApplication::dispatchCommandLine(const std::string& line) {
  const std::string trimmedLine = CommandLineDispatcher::trimLeft(line);
  if (trimmedLine.empty()) {
    return true;
  }

  const auto commandToken = CommandLineDispatcher::extractCommand(trimmedLine);
  if (!commandToken.has_value()) {
    std::cout << "Invalid command format\n";
    return true;
  }
  const std::string& command = commandToken.value();

  if (command == "/help") {
    CommandLineDispatcher::printHelp(std::cout);
    return true;
  }

  if (command.empty() || command.front() != '/') {
    std::cout << "Invalid command format\n";
    return true;
  }

  if (!CommandLineDispatcher::isSupportedServerCommand(command)) {
    std::cout << "Unknown command: " << command << '\n';
    return true;
  }

  std::string_view argsView = trimmedLine;
  argsView.remove_prefix(command.size());
  if (const auto err = CommandLineDispatcher::validateArgs(command, argsView)) {
    std::cout << *err << '\n';
    return true;
  }

  _messageRouter.setPendingCommand(command);
  sendCommandFrame(command + std::string(argsView));
  return true;
}

void ClientApplication::sendCommandFrame(const std::string& frame) {
  std::string wireFrame = frame;
  wireFrame.append("\r\n");
  _client.sendAll(wireFrame);
  _client.flushPendingWrites();
  updateServerWatchedEvents();
}

bool ClientApplication::handleServerEvent(const PollEvent& pollEvent) {
  if (hasEvent(pollEvent.revents, POLLERR | POLLHUP | POLLNVAL)) {
    handleConnectionLost();
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
    handleConnectionLost();
    return false;
  }
  if (bytesRead < 0) {
    return true;
  }

  _serverReadBuffer.append(buffer.data(), static_cast<std::size_t>(bytesRead));
  std::size_t parseOffset = 0;

  while (true) {
    const std::size_t lineEnd = _serverReadBuffer.find('\n', parseOffset);
    if (lineEnd == std::string::npos) {
      break;
    }

    const std::size_t frameLength = lineEnd - parseOffset;
    if (frameLength > MAX_SERVER_FRAME_SIZE) {
      throw ClientError("server frame exceeded maximum size");
    }

    std::string frame = _serverReadBuffer.substr(parseOffset, frameLength);
    if (!frame.empty() && frame.back() == '\r') {
      frame.pop_back();
    }
    _messageRouter.routeFrame(frame);
    parseOffset = lineEnd + 1;
    if (_messageRouter.shouldDisconnect()) {
      return false;
    }
  }

  if (parseOffset != 0) {
    _serverReadBuffer.erase(0, parseOffset);
  }

  if (_serverReadBuffer.size() > MAX_SERVER_FRAME_SIZE) {
    throw ClientError("server frame exceeded maximum size");
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
