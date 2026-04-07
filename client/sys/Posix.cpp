/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Posix
*/

#include "Posix.hpp"
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <cstddef>
#include <system_error>
#include "ClientError.hpp"

namespace {
void throwClient(const char* syscallName) {
  const int errorCode = errno;
  throw ClientError(syscallName,
                    std::error_code(errorCode, std::system_category()));
}
}  // namespace

namespace sys {

int Posix::socket(int domain, int type, int protocol) {
  int socketType = type;
#ifdef SOCK_NONBLOCK
  socketType |= SOCK_NONBLOCK;
#endif
  const int socketFd = ::socket(domain, socketType, protocol);
  if (socketFd == -1) {
    throwClient("socket");
  }

#ifndef SOCK_NONBLOCK
  // NOLINTNEXTLINE(hicpp-vararg)
  const int flags = ::fcntl(socketFd, F_GETFL, 0);
  if (flags == -1) {
    throwClient("fcntl F_GETFL");
  }
  // NOLINTNEXTLINE(hicpp-vararg, hicpp-signed-bitwise)
  if (::fcntl(socketFd, F_SETFL, flags | O_NONBLOCK) == -1) {
    throwClient("fcntl F_SETFL");
  }
#endif
  return socketFd;
}

void Posix::connect(int socketFd, const struct sockaddr* addr,
                    socklen_t addressLength) {
  while (true) {
    if (::connect(socketFd, addr, addressLength) == -1) {
      if (errno == EINTR) {
        continue;
      }
      throwClient("connect");
    }
    return;
  }
}

ssize_t Posix::read(int fileDescriptor, void* buffer, std::size_t count) {
  while (true) {
    const ssize_t bytesRead = ::read(fileDescriptor, buffer, count);
    if (bytesRead == -1) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return -1;
      }
      throwClient("read");
    }
    return bytesRead;
  }
}

ssize_t Posix::write(int fileDescriptor, const void* buffer,
                     std::size_t count) {
  while (true) {
    const ssize_t bytesWritten = ::write(fileDescriptor, buffer, count);
    if (bytesWritten == -1) {
      if (errno == EINTR) {
        continue;
      }
      if (errno == EAGAIN || errno == EWOULDBLOCK) {
        return 0;
      }
      throwClient("write");
    }
    return bytesWritten;
  }
}

int Posix::poll(struct pollfd* fileDescriptors, nfds_t fileDescriptorCount,
                int timeout) {
  const int numReady = ::poll(fileDescriptors, fileDescriptorCount, timeout);
  if (numReady == -1) {
    if (errno == EINTR) {
      return 0;
    }
    throwClient("poll");
  }
  return numReady;
}

void Posix::close(int fileDescriptor) {
  if (::close(fileDescriptor) == -1) {
    if (errno == EINTR) {
      return;
    }
    throwClient("close");
  }
}

}  // namespace sys
