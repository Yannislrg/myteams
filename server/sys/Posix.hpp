/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Posix
*/

#pragma once

#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstddef>
#include <cstdint>

namespace sys {

class Posix {
 public:
  Posix() = delete;

  static int socket(int domain, int type, int protocol);
  static void bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
  static void bind(int sockfd, const sockaddr_in6& addr);
  static void listen(int sockfd, int backlog);
  static int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
  static int accept(int sockfd, sockaddr_in6& addr, socklen_t& addrlen);
  static void setsockopt(int sockfd, int level, int optname, const void* optval,
                         socklen_t optlen);
  static ssize_t read(int fileDescriptor, void* buf, std::size_t count);
  static ssize_t write(int fileDescriptor, const void* buf, std::size_t count);
  static int poll(struct pollfd* fds, nfds_t nfds, int timeout);
  static void close(int fileDescriptor);
  static void setNonBlocking(int fileDescriptor);
};

}  // namespace sys
