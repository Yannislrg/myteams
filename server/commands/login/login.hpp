/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** login
*/

#pragma once

#include "ICommand.hpp"

class Login : public ICommand {
 public:
  Login() = default;
  ~Login() override = default;
  Login(const Login& other) = delete;
  Login& operator=(const Login& other) = delete;
  Login(Login&& other) = delete;
  Login& operator=(Login&& other) = delete;

  void execute(Client& client, Server& server) override;
};
