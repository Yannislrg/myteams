/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** use
*/

#pragma once

#include "ICommand.hpp"
class Use : public ICommand {
 public:
  Use() = default;
  ~Use() override = default;
  Use(const Use& other) = delete;
  Use& operator=(const Use& other) = delete;
  Use(Use&& other) = delete;
  Use& operator=(Use&& other) = delete;

  void execute(Client& client, Server& server) override;
};
