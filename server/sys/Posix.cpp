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
#include "Errors.hpp"

namespace sys {

int Posix::socket(int domain, int type, int protocol) {
  int socketFd = ::socket(domain, type, protocol);
  if (socketFd == -1) {
    throwSystemError("socket");
  }
  return socketFd;
}

void Posix::bind(int socketFd, const struct sockaddr* addr,
                 socklen_t addressLength) {
  if (::bind(socketFd, addr, addressLength) == -1) {
    throwSystemError("bind");
  }
}

void Posix::bind(int socketFd, const sockaddr_in6& addr) {
  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
  if (::bind(socketFd, reinterpret_cast<const sockaddr*>(&addr),
             sizeof(addr)) == -1) {
    throwSystemError("bind");
  }
}

void Posix::listen(int socketFd, int backlog) {
  if (::listen(socketFd, backlog) == -1) {
    throwSystemError("listen");
  }
}

int Posix::accept(int socketFd, struct sockaddr* addr,
                  socklen_t* addressLength) {
  // NOLINTNEXTLINE(android-cloexec-accept)
  int clientFd = ::accept(socketFd, addr, addressLength);
  if (clientFd == -1) {
    throwSystemError("accept");
  }
  return clientFd;
}

int Posix::accept(int socketFd, sockaddr_in6& addr, socklen_t& addressLength) {
  // NOLINTNEXTLINE(android-cloexec-accept,cppcoreguidelines-pro-type-reinterpret-cast)
  int clientFd =
      ::accept(socketFd, reinterpret_cast<sockaddr*>(&addr), &addressLength);
  if (clientFd == -1) {
    throwSystemError("accept");
  }
  return clientFd;
}

void Posix::setsockopt(int socketFd, int level, int optionName,
                       const void* optionValue, socklen_t optionLength) {
  if (::setsockopt(socketFd, level, optionName, optionValue, optionLength) ==
      -1) {
    throwSystemError("setsockopt");
  }
}

ssize_t Posix::read(int fileDescriptor, void* buffer, std::size_t count) {
  ssize_t bytesRead = ::read(fileDescriptor, buffer, count);
  if (bytesRead == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return -1;
    }
    throwSystemError("read");
  }
  return bytesRead;
}

ssize_t Posix::write(int fileDescriptor, const void* buffer,
                     std::size_t count) {
  ssize_t bytesWritten = ::write(fileDescriptor, buffer, count);
  if (bytesWritten == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return 0;
    }
    throwSystemError("write");
  }
  return bytesWritten;
}

int Posix::poll(struct pollfd* fileDescriptors, nfds_t fileDescriptorCount,
                int timeout) {
  int numReady = ::poll(fileDescriptors, fileDescriptorCount, timeout);
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
