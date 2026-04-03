/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** server
*/

#include "server.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <array>
#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include "../libs/logging_server.h"
#include "client/client.hpp"
#include "net/Poller.hpp"
#include "sys/Posix.hpp"

std::atomic<bool> Server::_running{false};

Server& Server::getInstance() {
  static Server instance;
  return instance;
}

Server::Server() : _serverFd(-1) {}

Server::~Server() noexcept {
  if (_serverFd != -1) {
    try {
      sys::Posix::close(_serverFd);
    } catch (const std::exception& e) {
      std::cerr << "[server] error closing server socket: " << e.what() << "\n";
    }
  }
}

void Server::init(uint16_t port) {
  const int socketOption = 1;
  const int ipv6Only = 0;

  _serverFd = sys::Posix::socket(AF_INET6, SOCK_STREAM, 0);
  sys::Posix::setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &socketOption,
                         sizeof(socketOption));
  sys::Posix::setsockopt(_serverFd, IPPROTO_IPV6, IPV6_V6ONLY, &ipv6Only,
                         sizeof(ipv6Only));
  sys::Posix::setNonBlocking(_serverFd);

  sockaddr_in6 addr{};
  addr.sin6_family = AF_INET6;
  addr.sin6_port = htons(port);
  addr.sin6_addr = in6addr_any;

  static constexpr int maxPendingConnections = 10;
  sys::Posix::bind(_serverFd, addr);
  sys::Posix::listen(_serverFd, maxPendingConnections);
  std::cerr << "[server] listening on port " << port << "\n";
}

void Server::handleSignal(int sig) {
  (void)sig;
  _running.store(false);
  std::cerr << "\n[server] shutting down\n";
}

void Server::_acceptClient(Poller& poller) {
  sockaddr_in6 addr{};
  socklen_t addrLen = sizeof(addr);
  const int clientFd = sys::Posix::accept(_serverFd, addr, addrLen);
  sys::Posix::setNonBlocking(clientFd);

  std::array<char, INET6_ADDRSTRLEN> ipStr{};
  ::inet_ntop(AF_INET6, &addr.sin6_addr, ipStr.data(), INET6_ADDRSTRLEN);
  std::cerr << "[+] client connected  fd=" << clientFd
            << "  ip=" << ipStr.data() << "\n";

  auto client = std::make_unique<Client>();
  client->setFd(clientFd);
  poller.addFileDescriptor(clientFd, POLLIN);
  _clients.emplace(clientFd, std::move(client));
}

void Server::_parseCommands(Client& client) {
  const std::string& readBuffer = client.getReadBuffer();
  std::size_t parsePos = 0;
  std::size_t newlinePos = readBuffer.find('\n', parsePos);
  while (newlinePos != std::string::npos) {
    std::string line = readBuffer.substr(parsePos, newlinePos - parsePos);
    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }
    if (!line.empty()) {
      std::cerr << "[fd=" << client.getFd() << "] cmd: " << line << "\n";
      _commandHandling.handleCommand(line, client, *this);
    }
    parsePos = newlinePos + 1;
    newlinePos = readBuffer.find('\n', parsePos);
  }
  client.consumeFromReadBuffer(parsePos);
}

void Server::_handleRead(Client& client) {
  std::array<char, _readBufferSize> buffer{};
  const ssize_t bytesRead =
      sys::Posix::read(client.getFd(), buffer.data(), buffer.size());
  if (bytesRead == -1) {
    return;
  }
  if (bytesRead == 0) {
    client.setFd(-1);
    return;
  }
  client.appendToReadBuffer(buffer.data(), static_cast<std::size_t>(bytesRead));
  _parseCommands(client);
}

void Server::_handleWrite(Client& client) {
  const std::string& writeBuffer = client.getWriteBuffer();
  if (writeBuffer.empty()) {
    return;
  }
  const ssize_t bytesWritten =
      sys::Posix::write(client.getFd(), writeBuffer.data(), writeBuffer.size());
  if (bytesWritten > 0) {
    client.consumeFromWriteBuffer(static_cast<std::size_t>(bytesWritten));
  }
}

void Server::_disconnectClient(int clientFd, Poller& poller) {
  std::cerr << "[-] client disconnected fd=" << clientFd << "\n";
  auto clientIter = _clients.find(clientFd);
  if (clientIter != _clients.end() &&
      !clientIter->second->getUserUuid().empty()) {
    server_event_user_logged_out(clientIter->second->getUserUuid().c_str());
  }
  poller.removeFileDescriptor(clientFd);
  sys::Posix::close(clientFd);
  _clients.erase(clientFd);
}

void Server::_updatePollFlags(Poller& poller) {
  for (const auto& [fileDescriptor, client] : _clients) {
    const bool needPollOut = !client->getWriteBuffer().empty();
    if (needPollOut == client->isPollOutEnabled()) {
      continue;
    }
    client->setPollOutEnabled(needPollOut);
    int evFlags = POLLIN;
    if (needPollOut) {
      // NOLINTNEXTLINE(hicpp-signed-bitwise)
      evFlags |= POLLOUT;
    }
    poller.updateWatchedEvents(fileDescriptor, evFlags);
  }
}

void Server::_processClientEvent(Client& client, const PollEvent& pollEv,
                                 Poller& poller) {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((pollEv.revents & (POLLHUP | POLLERR)) != 0) {
    _disconnectClient(pollEv.fd, poller);
    return;
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((pollEv.revents & POLLIN) != 0) {
    _handleRead(client);
    if (client.getFd() == -1) {
      _disconnectClient(pollEv.fd, poller);
      return;
    }
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  if ((pollEv.revents & POLLOUT) != 0) {
    _handleWrite(client);
  }
}

void Server::_processEvent(const PollEvent& pollEv, Poller& poller) {
  if (pollEv.fd == _serverFd) {
    // NOLINTNEXTLINE(hicpp-signed-bitwise)
    if ((pollEv.revents & POLLIN) != 0) {
      _acceptClient(poller);
    }
    return;
  }
  auto clientIter = _clients.find(pollEv.fd);
  if (clientIter == _clients.end()) {
    return;
  }
  _processClientEvent(*clientIter->second, pollEv, poller);
}

void Server::run() {
  _running.store(true);
  (void)std::signal(SIGINT, Server::handleSignal);
  (void)std::signal(SIGTERM, Server::handleSignal);

  Poller poller;
  poller.addFileDescriptor(_serverFd, POLLIN);

  while (_running.load()) {
    _updatePollFlags(poller);
    auto pollEvents = poller.waitForEvents(_pollTimeoutMs);
    for (const auto& pollEv : pollEvents) {
      _processEvent(pollEv, poller);
    }
  }
}

void Server::broadcast(const std::string& msg) {
  for (const auto& [filedescriptor, client] : _clients) {
    (void)filedescriptor;
    client->appendToWriteBuffer(msg);
  }
}

void Server::notifySubscribers(const std::string& teamUuid,
                               const std::string& msg) {
  (void)teamUuid;
  (void)msg;
  (void)_clients;
}
