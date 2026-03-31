/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** server
*/

#pragma once

#include <string>
#include <vector>
#include "client/client.hpp"
#include "commands/commandHandling/commandHandling.hpp"
#include "database/Database.hpp"
#include "saveManager/saveManager.hpp"

class Server {
 public:
  Server();
  ~Server();
  Server(const Server& other) = delete;
  Server& operator=(const Server& other) = delete;
  Server(Server&& other) = delete;
  Server& operator=(Server&& other) = delete;

  static Server& getInstance();
  void run();
  void broadcast(const std::string& msg);
  void notifySubscribers(const std::string& team_uuid, const std::string& msg);
  static void handleSignal(int sig);

  [[nodiscard]] int getServerFd() const { return _serverFd; }
  void setServerFd(int serverFileDescriptor) {
    _serverFd = serverFileDescriptor;
  }

  [[nodiscard]] const std::vector<Client>& getClients() const {
    return _clients;
  }
  void setClients(std::vector<Client> clients) { _clients.swap(clients); }

  [[nodiscard]] Database& getDb() { return _db; }
  [[nodiscard]] const Database& getDb() const { return _db; }

  [[nodiscard]] SaveManager& getSaveManager() { return _saveManager; }
  [[nodiscard]] const SaveManager& getSaveManager() const {
    return _saveManager;
  }

  [[nodiscard]] CommandHandling& getCommandHandling() {
    return _commandHandling;
  }
  [[nodiscard]] const CommandHandling& getCommandHandling() const {
    return _commandHandling;
  }

 private:
  int _serverFd;
  std::vector<Client> _clients;
  Database _db;
  SaveManager _saveManager;
  CommandHandling _commandHandling;
};
