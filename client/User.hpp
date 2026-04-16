/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** User
*/

#pragma once

#include <string>

class User {
 public:
  void login(const std::string& uuid, const std::string& name);
  void logout();

  [[nodiscard]] bool isLoggedIn() const;
  [[nodiscard]] const std::string& getUuid() const;
  [[nodiscard]] const std::string& getName() const;

 private:
  std::string _uuid;
  std::string _name;
  bool _loggedIn = false;
};
