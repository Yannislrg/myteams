/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Errors
*/

#pragma once

#include <string>

namespace sys {

[[noreturn]] void throwSystemError(const std::string& syscall);

}  // namespace sys
