/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** User
*/

#include "User.hpp"

void User::login(const std::string& uuid, const std::string& name) {
  _uuid = uuid;
  _name = name;
  _loggedIn = true;
}

void User::logout() {
  _uuid.clear();
  _name.clear();
  _loggedIn = false;
}

bool User::isLoggedIn() const {
  return _loggedIn;
}

const std::string& User::getUuid() const {
  return _uuid;
}

const std::string& User::getName() const {
  return _name;
}
