/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** Errors
*/

#include "Errors.hpp"
#include <cerrno>
#include <system_error>

namespace sys {

void throwSystemError(const std::string& syscall) {
  throw std::system_error(errno, std::generic_category(), syscall);
}

}  // namespace sys
