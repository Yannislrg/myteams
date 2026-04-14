/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** subscribe
*/

#pragma once

#include "ICommand.hpp"
class Subscribe : public ICommand {
 public:
  Subscribe() = default;
  ~Subscribe() override = default;
  Subscribe(const Subscribe& other) = delete;
  Subscribe& operator=(const Subscribe& other) = delete;
  Subscribe(Subscribe&& other) = delete;
  Subscribe& operator=(Subscribe&& other) = delete;

  void execute(Client& client, Server& server) override;
};
