/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** server
*/

#include "server.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include "sys/Posix.hpp"

static constexpr int MAX_PENDING_CONNECTIONS = 10;

Server& Server::getInstance() {
  static Server instance;
  return instance;
}

Server::Server() : _serverFd(-1) {}

Server::~Server() {
  if (_serverFd != -1) {
    sys::Posix::close(_serverFd);
  }
}

void Server::init(uint16_t port) {
  const int opt = 1;
  const int v6only = 0;

  _serverFd = sys::Posix::socket(AF_INET6, SOCK_STREAM, 0);
  sys::Posix::setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt,
                         sizeof(opt));
  sys::Posix::setsockopt(_serverFd, IPPROTO_IPV6, IPV6_V6ONLY, &v6only,
                         sizeof(v6only));

  sockaddr_in6 addr{};
  addr.sin6_family = AF_INET6;
  addr.sin6_port = htons(port);
  addr.sin6_addr = in6addr_any;

  sys::Posix::bind(_serverFd, addr);
  sys::Posix::listen(_serverFd, MAX_PENDING_CONNECTIONS);
}

void Server::run() {}

void Server::broadcast(const std::string& msg) {
  (void)msg;
  (void)_clients;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
void Server::notifySubscribers(const std::string& team_uuid,
                               const std::string& msg) {
  (void)team_uuid;
  (void)msg;
  (void)_clients;
}

void Server::handleSignal(int sig) { (void)sig; }
