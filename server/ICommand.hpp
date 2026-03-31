/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ICommand
*/

#pragma once

class ICommand {
 public:
  ICommand();
  ~ICommand();
  ICommand(const ICommand& other) = delete;
  ICommand& operator=(const ICommand& other) = delete;
  ICommand(ICommand&& other) = delete;
  ICommand& operator=(ICommand&& other) = delete;

 protected:
 private:
};
