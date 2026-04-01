/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Poller
*/

#include "Poller.hpp"
#include <algorithm>
#include <cstdint>
#include "../sys/Posix.hpp"

void Poller::addFileDescriptor(int fileDescriptor, int events) {
  struct pollfd pfd{};
  pfd.fd = fileDescriptor;
  pfd.events = static_cast<int16_t>(events);
  pfd.revents = 0;
  _fds.push_back(pfd);
}

void Poller::updateWatchedEvents(int fileDescriptor, int events) {
  auto iter =
      std::ranges::find_if(_fds, [fileDescriptor](const struct pollfd& entry) {
        return entry.fd == fileDescriptor;
      });
  if (iter != _fds.end()) {
    iter->events = static_cast<int16_t>(events);
  }
}

void Poller::removeFileDescriptor(int fileDescriptor) {
  auto iter =
      std::ranges::find_if(_fds, [fileDescriptor](const struct pollfd& entry) {
        return entry.fd == fileDescriptor;
      });
  if (iter != _fds.end()) {
    _fds.erase(iter);
  }
}

std::vector<PollEvent> Poller::waitForEvents(int timeout) {
  int ready = sys::Posix::poll(_fds.data(), static_cast<uint64_t>(_fds.size()),
                               timeout);
  std::vector<PollEvent> events;
  if (ready <= 0) {
    return events;
  }
  events.reserve(static_cast<std::size_t>(ready));
  for (const auto& entry : _fds) {
    if (entry.revents != 0) {
      events.push_back({entry.fd, static_cast<int>(entry.revents)});
    }
  }
  return events;
}
