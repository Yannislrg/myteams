/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Channel
*/

#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Thread.hpp"

class Channel {
 public:
  Channel();
  ~Channel();
  Channel(const Channel& other) = default;
  Channel& operator=(const Channel& other) = default;
  Channel(Channel&& other) = default;
  Channel& operator=(Channel&& other) = default;

  [[nodiscard]] const std::string& getUuid() const { return _uuid; }
  void setUuid(const std::string& uuid) { _uuid = uuid; }

  [[nodiscard]] const std::string& getName() const { return _name; }
  void setName(const std::string& name) { _name = name; }

  [[nodiscard]] const std::string& getDescription() const {
    return _description;
  }
  void setDescription(const std::string& description) {
    _description = description;
  }

  [[nodiscard]] const std::vector<Thread>& getThreads() const {
    return _threads;
  }
  void setThreads(std::vector<Thread>&& threads) {
    _threads = std::move(threads);
  }

 protected:
 private:
  std::string _uuid;
  std::string _name;
  std::string _description;
  std::vector<Thread> _threads;
};
