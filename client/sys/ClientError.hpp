/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Errors
*/

#pragma once

#include <exception>
#include <string>
#include <system_error>

class ClientError : public std::exception {
 public:
  explicit ClientError(std::string message);
  ClientError(std::string context, std::error_code errorCode);

  [[nodiscard]] const char* what() const noexcept override;
  [[nodiscard]] const std::error_code& code() const noexcept;

 private:
  std::string _message;
  std::error_code _errorCode;
};
