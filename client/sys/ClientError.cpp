/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Errors
*/

#include "ClientError.hpp"
#include <utility>

ClientError::ClientError(std::string message) : _message(std::move(message)) {}

ClientError::ClientError(std::string context, std::error_code errorCode)
    : _message(std::move(context) + ": " + errorCode.message()),
      _errorCode(errorCode) {}

const char* ClientError::what() const noexcept { return _message.c_str(); }

const std::error_code& ClientError::code() const noexcept { return _errorCode; }
