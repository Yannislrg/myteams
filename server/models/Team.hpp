/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Team
*/

#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Channel.hpp"

class Team {
 public:
  Team();
  ~Team();
  Team(const Team& other) = default;
  Team& operator=(const Team& other) = default;
  Team(Team&& other) = default;
  Team& operator=(Team&& other) = default;

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

  [[nodiscard]] const std::vector<std::string>& getSubscriberUuids() const {
    return _subscriberUuids;
  }
  std::vector<std::string>& getSubscriberUuids() { return _subscriberUuids; }
  void setSubscriberUuids(const std::vector<std::string>& subscriberUuids) {
    _subscriberUuids = subscriberUuids;
  }

  [[nodiscard]] const std::vector<Channel>& getChannels() const {
    return _channels;
  }
  [[nodiscard]] std::vector<Channel>& getChannels() { return _channels; }
  void setChannels(std::vector<Channel>&& channels) {
    _channels = std::move(channels);
  }

 protected:
 private:
  std::string _uuid;
  std::string _name;
  std::string _description;
  std::vector<std::string> _subscriberUuids;
  std::vector<Channel> _channels;
};
