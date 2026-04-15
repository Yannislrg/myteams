/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** User
*/

#include "User.hpp"

User::User() : _connected(false) {}

User::~User() = default;

void User::addMessage(const std::string& senderUuid, const Message& message) {
  _messages[senderUuid].push_back(message);
}
