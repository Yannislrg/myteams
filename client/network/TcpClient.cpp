/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** TcpClient
*/

#include "TcpClient.hpp"
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include "ClientError.hpp"
#include "Posix.hpp"

TcpClient::~TcpClient() noexcept { disconnect(); }

void TcpClient::connectTo(const std::string& host, uint16_t port) {
  disconnect();

  struct addrinfo hints {};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* addresses = nullptr;
  const std::string portAsString = std::to_string(port);
  const int status =
      ::getaddrinfo(host.c_str(), portAsString.c_str(), &hints, &addresses);
  if (status != 0) {
    throw ClientError(std::string("getaddrinfo: ") + ::gai_strerror(status));
  }

  for (addrinfo* address = addresses; address != nullptr;
       address = address->ai_next) {
    int socketFd = -1;
    try {
      socketFd = sys::Posix::socket(address->ai_family, address->ai_socktype,
                                    address->ai_protocol);
      sys::Posix::connect(socketFd, address->ai_addr,
                          address->ai_addrlen);
      _socketFd = socketFd;
      break;
    } catch (const std::exception& error) {
      if (socketFd != -1) {
        try {
          sys::Posix::close(socketFd);
        } catch (const std::exception& closeError) {
          std::cerr
              << "warning: failed to close connection attempt socket: "
              << closeError.what() << '\n';
        }
      }
      std::cerr << "warning: connection attempt failed: " << error.what()
                << '\n';
    }
  }

  ::freeaddrinfo(addresses);

  if (_socketFd == -1) {
    throw ClientError("unable to connect to server");
  }
}

void TcpClient::sendAll(const std::string& data) const {
  std::size_t sent = 0;
  while (sent < data.size()) {
    const ssize_t bytesWritten =
        sys::Posix::write(_socketFd, data.data() + sent, data.size() - sent);
    if (bytesWritten <= 0) {
      throw ClientError("socket write failed");
    }
    sent += static_cast<std::size_t>(bytesWritten);
  }
}

ssize_t TcpClient::receiveSome(char* buffer, std::size_t bufferSize) const {
  return sys::Posix::read(_socketFd, buffer, bufferSize);
}

void TcpClient::disconnect() noexcept {
  if (_socketFd == -1) {
    return;
  }
  try {
    sys::Posix::close(_socketFd);
  } catch (const std::exception& error) {
    std::cerr << "warning: failed to close client socket: " << error.what()
              << '\n';
  }
  _socketFd = -1;
}
