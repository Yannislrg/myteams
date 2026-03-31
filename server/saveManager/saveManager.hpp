/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** saveManager
*/

#pragma once

#include "../database/Database.hpp"

class SaveManager {
 public:
  SaveManager();
  ~SaveManager();
  SaveManager(const SaveManager& other) = delete;
  SaveManager& operator=(const SaveManager& other) = delete;
  SaveManager(SaveManager&& other) = delete;
  SaveManager& operator=(SaveManager&& other) = delete;

  void save(const Database& database);
  void load(Database& database);

 protected:
 private:
};
