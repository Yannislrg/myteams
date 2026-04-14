/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** saveManager
*/

#include "saveManager.hpp"
#include <fstream>
#include <iostream>
#include "JsonParser.hpp"
#include "JsonSerializer.hpp"

static constexpr const char* SAVE_FILE = "myteams_data.json";

void SaveManager::save(const Database& database) {
  std::ofstream file(SAVE_FILE);
  if (!file.is_open()) {
    std::cerr << "[saveManager] failed to open " << SAVE_FILE
              << " for writing\n";
    return;
  }
  std::string users, teams;
  for (const auto& user : database.getUsers()) {
    if (!users.empty()) users += ',';
    users += userToJson(user);
  }
  for (const auto& team : database.getTeams()) {
    if (!teams.empty()) teams += ',';
    teams += teamToJson(team);
  }
  file << "{\"users\":[" << users << "],\"teams\":[" << teams << "]}";
  std::cerr << "[saveManager] saved to " << SAVE_FILE << "\n";
}

void SaveManager::load(Database& database) {
  std::ifstream file(SAVE_FILE);
  if (!file.is_open()) {
    return;
  }
  const std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
  JsonParser parser(content);
  const JsonVal root = parser.parse();

  for (const auto& userVal : root["users"].array())
    database.getUsers().push_back(userFromJson(userVal));
  for (const auto& teamVal : root["teams"].array())
    database.getTeams().push_back(teamFromJson(teamVal));
  std::cerr << "[saveManager] loaded " << database.getUsers().size()
            << " users, " << database.getTeams().size() << " teams\n";
}
