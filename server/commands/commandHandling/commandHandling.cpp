/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** commandHandling
*/

#include "commandHandling.hpp"

CommandHandling::CommandHandling() = default;

void CommandHandling::handleCommand(const std::string& raw, Client& client,
                                    Server& server) {
  (void)raw;
  (void)client;
  (void)server;
  (void)_commands.empty();
}
