/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** send
*/

#pragma once

#include "ICommand.hpp"

class Send : public ICommand {
 public:
  Send() = default;
  ~Send() override = default;
  Send(const Send& other) = delete;
  Send& operator=(const Send& other) = delete;
  Send(Send&& other) = delete;
  Send& operator=(Send&& other) = delete;

  void execute(Client& client, Server& server) override;
};
