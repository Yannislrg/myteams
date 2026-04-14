/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** unsubscribe
*/

#pragma once

#include "ICommand.hpp"

class Unsubscribe : public ICommand {
 public:
  Unsubscribe() = default;
  ~Unsubscribe() override = default;
  Unsubscribe(const Unsubscribe& other) = delete;
  Unsubscribe& operator=(const Unsubscribe& other) = delete;
  Unsubscribe(Unsubscribe&& other) = delete;
  Unsubscribe& operator=(Unsubscribe&& other) = delete;

  void execute(Client& client, Server& server) override;
};
