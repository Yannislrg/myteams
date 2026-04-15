/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** logout
*/

#pragma once

#include "ICommand.hpp"

class Logout : public ICommand {
 public:
  Logout() = default;
  ~Logout() override = default;
  Logout(const Logout& other) = delete;
  Logout& operator=(const Logout& other) = delete;
  Logout(Logout&& other) = delete;
  Logout& operator=(Logout&& other) = delete;

  void execute(Client& client, Server& server) override;
};
