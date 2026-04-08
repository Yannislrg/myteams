/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** saveManager
*/

#pragma once

#include "database/Database.hpp"

class SaveManager {
 public:
  SaveManager() = default;
  ~SaveManager() = default;
  SaveManager(const SaveManager& other) = delete;
  SaveManager& operator=(const SaveManager& other) = delete;
  SaveManager(SaveManager&& other) = delete;
  SaveManager& operator=(SaveManager&& other) = delete;

  static void save(const Database& database);
  static void load(Database& database);

 protected:
 private:
};
