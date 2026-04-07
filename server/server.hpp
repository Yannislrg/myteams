/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** server
*/

#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include "client/client.hpp"
#include "commands/commandHandling/commandHandling.hpp"
#include "database/Database.hpp"
#include "net/Poller.hpp"
#include "saveManager/saveManager.hpp"

class Server {
 public:
  Server(const Server& other) = delete;
  Server& operator=(const Server& other) = delete;
  Server(Server&& other) = delete;
  Server& operator=(Server&& other) = delete;

  ~Server() noexcept;

  static Server& getInstance();
  void init(uint16_t port);
  void run();
  void sendToClient(const std::string& msg, Client& client);
  void broadcast(const std::string& msg);
  void notifySubscribers(const std::string& teamUuid, const std::string& msg);
  static void handleSignal(int sig);

  [[nodiscard]] int getServerFd() const { return _serverFd; }
  void setServerFd(int serverFileDescriptor) {
    _serverFd = serverFileDescriptor;
  }

  [[nodiscard]] const std::unordered_map<int, std::unique_ptr<Client>>&
  getClients() const {
    return _clients;
  }

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
  static constexpr std::size_t _readBufferSize = 4096;
  static constexpr int _pollTimeoutMs = 1000;

  Server();

  void _acceptClient(Poller& poller);
  void _parseCommands(Client& client);
  void _handleRead(Client& client);
  static void _handleWrite(Client& client);
  void _disconnectClient(int clientFd, Poller& poller);
  void _updatePollFlags(Poller& poller);
  void _processClientEvent(Client& client, const PollEvent& pollEv,
                           Poller& poller);
  void _processEvent(const PollEvent& pollEv, Poller& poller);

  static std::atomic<bool> _running;

  int _serverFd;
  std::unordered_map<int, std::unique_ptr<Client>> _clients;
  Database _db;
  SaveManager _saveManager;
  CommandHandling _commandHandling;
};
