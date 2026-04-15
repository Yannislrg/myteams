/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** subscribed
*/

#pragma once

#include "ICommand.hpp"

class Subscribed : public ICommand {
 public:
  Subscribed() = default;
  ~Subscribed() override = default;
  Subscribed(const Subscribed& other) = delete;
  Subscribed& operator=(const Subscribed& other) = delete;
  Subscribed(Subscribed&& other) = delete;
  Subscribed& operator=(Subscribed&& other) = delete;

  void execute(Client& client, Server& server) override;
};
