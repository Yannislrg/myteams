/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ICommand
*/

#include "ICommand.hpp"

ICommand::ICommand() = default;

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void ICommand::execute(Client& client, Server& server) {
  (void)client;
  (void)server;
}
