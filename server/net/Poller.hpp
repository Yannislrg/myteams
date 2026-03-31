/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Poller
*/

#pragma once

#include <poll.h>
#include <vector>

struct PollEvent {
  int fd;
  int revents;
};

class Poller {
 public:
  void add(int fileDescriptor, int events);
  void mod(int fileDescriptor, int events);
  void del(int fileDescriptor);
  std::vector<PollEvent> wait(int timeout = -1);

 private:
  std::vector<struct pollfd> _fds;
};
