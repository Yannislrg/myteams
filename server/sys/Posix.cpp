/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Posix
*/

#include "Posix.hpp"
#include <fcntl.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstdint>
#include "Errors.hpp"

namespace sys {

int Posix::socket(int domain, int type, int protocol) {
  int sockFd = ::socket(domain, type, protocol);
  if (sockFd == -1) {
    throwSystemError("socket");
  }
  return sockFd;
}

void Posix::bind(int sockfd, const struct sockaddr* addr,
                 unsigned int addrlen) {
  if (::bind(sockfd, addr, addrlen) == -1) {
    throwSystemError("bind");
  }
}

void Posix::bind(int sockfd, const sockaddr_in6& addr) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (::bind(sockfd, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr)) ==
      -1) {
    throwSystemError("bind");
  }
}

void Posix::listen(int sockfd, int backlog) {
  if (::listen(sockfd, backlog) == -1) {
    throwSystemError("listen");
  }
}

int Posix::accept(int sockfd, struct sockaddr* addr, unsigned int* addrlen) {
  // NOLINTNEXTLINE(android-cloexec-accept)
  int clientFd = ::accept(sockfd, addr, addrlen);
  if (clientFd == -1) {
    throwSystemError("accept");
  }
  return clientFd;
}

int Posix::accept(int sockfd, sockaddr_in6& addr, unsigned int& addrlen) {
  // NOLINTNEXTLINE(android-cloexec-accept,cppcoreguidelines-pro-type-reinterpret-cast)
  int clientFd = ::accept(sockfd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
  if (clientFd == -1) {
    throwSystemError("accept");
  }
  return clientFd;
}

void Posix::setsockopt(int sockfd, int level, int optname, const void* optval,
                       unsigned int optlen) {
  if (::setsockopt(sockfd, level, optname, optval, optlen) == -1) {
    throwSystemError("setsockopt");
  }
}

ssize_t Posix::read(int fileDescriptor, void* buf, std::size_t count) {
  ssize_t bytesRead = ::read(fileDescriptor, buf, count);
  if (bytesRead == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    throwSystemError("read");
  }
  return bytesRead;
}

ssize_t Posix::write(int fileDescriptor, const void* buf, std::size_t count) {
  ssize_t bytesWritten = ::write(fileDescriptor, buf, count);
  if (bytesWritten == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    throwSystemError("write");
  }
  return bytesWritten;
}

int Posix::poll(struct pollfd* fds, uint64_t nfds, int timeout) {
  int numReady = ::poll(fds, nfds, timeout);
  if (numReady == -1) {
    if (errno == EINTR) {
      return 0;
    }
    throwSystemError("poll");
  }
  return numReady;
}

void Posix::close(int fileDescriptor) {
  if (::close(fileDescriptor) == -1) {
    throwSystemError("close");
  }
}

void Posix::setNonBlocking(int fileDescriptor) {
  // NOLINTNEXTLINE(yhicpp-vararg)
  int flags = ::fcntl(fileDescriptor, F_GETFL, 0);
  if (flags == -1) {
    throwSystemError("fcntl F_GETFL");
  }
  unsigned int newFlags =
      static_cast<unsigned int>(flags) | static_cast<unsigned int>(O_NONBLOCK);
  // NOLINTNEXTLINE(hicpp-vararg)
  if (::fcntl(fileDescriptor, F_SETFL, newFlags) == -1) {
    throwSystemError("fcntl F_SETFL");
  }
}

}  // namespace sys
