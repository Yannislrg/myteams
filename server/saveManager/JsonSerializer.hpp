/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** JsonSerializer
*/

#pragma once

#include <string>
#include "Channel.hpp"
#include "Message.hpp"
#include "Reply.hpp"
#include "Team.hpp"
#include "Thread.hpp"
#include "User.hpp"
#include "JsonParser.hpp"

std::string escapeJson(const std::string& str);

std::string messageToJson(const Message& msg);
std::string replyToJson(const Reply& reply);
std::string threadToJson(const Thread& thread);
std::string channelToJson(const Channel& channel);
std::string teamToJson(const Team& team);
std::string userToJson(const User& user);

Message messageFromJson(const JsonVal& val);
Reply replyFromJson(const JsonVal& val);
Thread threadFromJson(const JsonVal& val);
Channel channelFromJson(const JsonVal& val);
Team teamFromJson(const JsonVal& val);
User userFromJson(const JsonVal& val);
