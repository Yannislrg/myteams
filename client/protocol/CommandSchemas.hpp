/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** CommandSchemas
*/

#pragma once

#include <array>
#include <cstddef>
#include <string_view>

static constexpr std::size_t maxName = 32;
static constexpr std::size_t maxDesc = 255;
static constexpr std::size_t maxBody = 512;
static constexpr std::size_t maxUuid = 36;

struct ArgSchema {
  std::size_t maxLen = 0;
  std::string_view name;
};

struct CommandSchema {
  std::string_view name;
  std::string_view usage;
  std::size_t minArgs = 0;
  std::size_t maxArgs = 0;
  std::array<ArgSchema, 3> args = {};
};

inline constexpr std::array<CommandSchema, 14> schemas = {{
    CommandSchema{
        .name = "/login",
        .usage = R"(/login "user_name")",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxName, .name = "user_name"}},
    },
    CommandSchema{
        .name = "/logout", .usage = "/logout", .minArgs = 0, .maxArgs = 0},
    CommandSchema{
        .name = "/users", .usage = "/users", .minArgs = 0, .maxArgs = 0},
    CommandSchema{
        .name = "/user",
        .usage = R"(/user "user_uuid")",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "user_uuid"}},
    },
    CommandSchema{
        .name = "/send",
        .usage = R"(/send "user_uuid" "message_body")",
        .minArgs = 2,
        .maxArgs = 2,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "user_uuid"},
                 ArgSchema{.maxLen = maxBody, .name = "message_body"}},
    },
    CommandSchema{
        .name = "/messages",
        .usage = R"(/messages "user_uuid")",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "user_uuid"}},
    },
    CommandSchema{
        .name = "/subscribe",
        .usage = R"(/subscribe "team_uuid")",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "team_uuid"}},
    },
    CommandSchema{
        .name = "/subscribed",
        .usage = R"(/subscribed ?"team_uuid")",
        .minArgs = 0,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "team_uuid"}},
    },
    CommandSchema{
        .name = "/unsubscribe",
        .usage = R"(/unsubscribe "team_uuid")",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "team_uuid"}},
    },
    CommandSchema{
        .name = "/use",
        .usage = R"(/use ?"team_uuid" ?"channel_uuid" ?"thread_uuid")",
        .minArgs = 0,
        .maxArgs = 3,
        .args = {ArgSchema{.maxLen = maxUuid, .name = "team_uuid"},
                 ArgSchema{.maxLen = maxUuid, .name = "channel_uuid"},
                 ArgSchema{.maxLen = maxUuid, .name = "thread_uuid"}},
    },
    // /create: 1 arg = comment body
    CommandSchema{
        .name = "/create",
        .usage = R"(/create "comment_body" (after /use team channel thread))",
        .minArgs = 1,
        .maxArgs = 1,
        .args = {ArgSchema{.maxLen = maxBody, .name = "comment_body"}},
    },
    // /create: 2 args = name + description or thread message
    CommandSchema{
        .name = "/create",
        .usage =
            R"(/create "name" "description/message" (team, channel, or thread))",
        .minArgs = 2,
        .maxArgs = 2,
        .args = {ArgSchema{.maxLen = maxName, .name = "name"},
                 ArgSchema{.maxLen = maxBody, .name = "description/message"}},
    },
    CommandSchema{
        .name = "/list", .usage = "/list", .minArgs = 0, .maxArgs = 0},
    CommandSchema{
        .name = "/info", .usage = "/info", .minArgs = 0, .maxArgs = 0},
}};
