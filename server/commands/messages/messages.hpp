/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** messages
*/

#pragma once

#include "ICommand.hpp"

class Messages : public ICommand {
 public:
  Messages() = default;
  ~Messages() override = default;
  Messages(const Messages& other) = delete;
  Messages& operator=(const Messages& other) = delete;
  Messages(Messages&& other) = delete;
  Messages& operator=(Messages&& other) = delete;

  void execute(Client& client, Server& server) override;
};
