/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Posix
*/

#pragma once

#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstddef>

namespace sys {

class Posix {
 public:
  Posix() = delete;

  static int socket(int domain, int type, int protocol);
  static void connect(int socketFd, const struct sockaddr* addr,
                      socklen_t addressLength);
  static ssize_t read(int fileDescriptor, void* buffer, std::size_t count);
  static ssize_t write(int fileDescriptor, const void* buffer,
                       std::size_t count);
  static int poll(struct pollfd* fileDescriptors, nfds_t fileDescriptorCount,
                  int timeout);
  static void close(int fileDescriptor);
};

}  // namespace sys
