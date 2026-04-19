/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ResponseStatusHandler
*/

#pragma once

#include <functional>
#include <string>
#include <vector>
#include "ICommandHandler.hpp"
#include "User.hpp"

class ResponseStatusHandler : public ICommandHandler {
 public:
  ResponseStatusHandler(std::string code, User& user, bool& shouldDisconnect,
                        const std::string& pendingCommand);

  [[nodiscard]] std::string getCommand() const override { return _code; }
  void handle(const std::vector<std::string>& tokens) const override;

 private:
  using Tokens = std::vector<std::string>;

  void handle200(const Tokens& tokens) const;
  void handle201(const Tokens& tokens) const;
  void handle400(const Tokens& tokens) const;
  void handle401(const Tokens& tokens) const;
  void handle403(const Tokens& tokens) const;
  void handle404(const Tokens& tokens) const;
  void handle409(const Tokens& tokens) const;

  void handle200Ok(const Tokens& tokens) const;
  void handle200User(const Tokens& tokens) const;
  void handle200Subscribe(const Tokens& tokens) const;
  void handle200Unsubscribe(const Tokens& tokens) const;
  void handle200Info(const Tokens& tokens) const;

  void handle200InfoUser(const Tokens& tokens) const;
  void handle200InfoTeam(const Tokens& tokens) const;
  void handle200InfoChannel(const Tokens& tokens) const;
  void handle200InfoThread(const Tokens& tokens) const;

  std::string _code;
  User& _user;
  bool& _shouldDisconnect;
  const std::string& _pendingCommand;
};
