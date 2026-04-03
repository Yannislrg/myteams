/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Poller
*/

#pragma once

#include <sys/poll.h>
#include <vector>

struct PollEvent {
  int fd;
  int revents;
};

class Poller {
 public:
  void addFileDescriptor(int fileDescriptor, int events);
  void updateWatchedEvents(int fileDescriptor, int events);
  void removeFileDescriptor(int fileDescriptor);
  std::vector<PollEvent> waitForEvents(int timeout = -1);

 private:
  std::vector<struct pollfd> _fds;
};
