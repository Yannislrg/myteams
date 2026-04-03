/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Posix
*/

#pragma once

#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cstddef>

namespace sys {

class Posix {
 public:
  Posix() = delete;

  static int socket(int domain, int type, int protocol);
  static void bind(int socketFd, const struct sockaddr* addr,
                   socklen_t addressLength);
  static void bind(int socketFd, const sockaddr_in6& addr);
  static void listen(int socketFd, int backlog);
  static int accept(int socketFd, struct sockaddr* addr,
                    socklen_t* addressLength);
  static int accept(int socketFd, sockaddr_in6& addr, socklen_t& addressLength);
  static void setsockopt(int socketFd, int level, int optionName,
                         const void* optionValue, socklen_t optionLength);
  static ssize_t read(int fileDescriptor, void* buffer, std::size_t count);
  static ssize_t write(int fileDescriptor, const void* buffer,
                       std::size_t count);
  static int poll(struct pollfd* fileDescriptors, nfds_t fileDescriptorCount,
                  int timeout);
  static void close(int fileDescriptor);
  static void setNonBlocking(int fileDescriptor);
};

}  // namespace sys
