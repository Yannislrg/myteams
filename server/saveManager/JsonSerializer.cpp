/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** JsonSerializer
*/

#include "JsonSerializer.hpp"
#include <ctime>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::string escapeJson(const std::string& str) {
  std::string out;
  out.reserve(str.size() + 2);
  out += '"';
  for (char chr : str) {
    if (chr == '"')
      out += "\\\"";
    else if (chr == '\\')
      out += "\\\\";
    else
      out += chr;
  }
  out += '"';
  return out;
}

template <typename Range, typename Mapper>
static std::string jsonArray(const Range& range, Mapper mapper) {
  std::string out;
  for (const auto& item : range) {
    if (!out.empty()) out += ',';
    out += mapper(item);
  }
  return '[' + out + ']';
}

template <typename Map, typename Mapper>
static std::string jsonMapOfArrays(const Map& map, Mapper mapper) {
  std::string out;
  for (const auto& [key, values] : map) {
    if (!out.empty()) out += ',';
    out += escapeJson(key) + ':' + jsonArray(values, mapper);
  }
  return '{' + out + '}';
}

std::string messageToJson(const Message& msg) {
  std::ostringstream out;
  out << '{' << "\"sender\":" << escapeJson(msg.getSenderUuid()) << ','
      << "\"receiver\":" << escapeJson(msg.getReceiverUuid()) << ','
      << "\"timestamp\":" << msg.getTimestamp() << ','
      << "\"body\":" << escapeJson(msg.getBody()) << '}';
  return out.str();
}

std::string replyToJson(const Reply& reply) {
  std::ostringstream out;
  out << '{' << "\"uuid\":" << escapeJson(reply.getUuid()) << ','
      << "\"user_uuid\":" << escapeJson(reply.getUserUuid()) << ','
      << "\"timestamp\":" << reply.getTimestamp() << ','
      << "\"body\":" << escapeJson(reply.getBody()) << '}';
  return out.str();
}

std::string threadToJson(const Thread& thread) {
  std::ostringstream out;
  out << '{' << "\"uuid\":" << escapeJson(thread.getUuid()) << ','
      << "\"user_uuid\":" << escapeJson(thread.getUserUuid()) << ','
      << "\"timestamp\":" << thread.getTimestamp() << ','
      << "\"title\":" << escapeJson(thread.getTitle()) << ','
      << "\"body\":" << escapeJson(thread.getBody()) << ','
      << "\"replies\":" << jsonArray(thread.getReplies(), replyToJson) << '}';
  return out.str();
}

std::string channelToJson(const Channel& channel) {
  std::ostringstream out;
  out << '{' << "\"uuid\":" << escapeJson(channel.getUuid()) << ','
      << "\"name\":" << escapeJson(channel.getName()) << ','
      << "\"description\":" << escapeJson(channel.getDescription()) << ','
      << "\"threads\":" << jsonArray(channel.getThreads(), threadToJson) << '}';
  return out.str();
}

std::string teamToJson(const Team& team) {
  std::ostringstream out;
  out << '{' << "\"uuid\":" << escapeJson(team.getUuid()) << ','
      << "\"name\":" << escapeJson(team.getName()) << ','
      << "\"description\":" << escapeJson(team.getDescription()) << ','
      << "\"subscribers\":" << jsonArray(team.getSubscriberUuids(), escapeJson)
      << ',' << "\"channels\":" << jsonArray(team.getChannels(), channelToJson)
      << '}';
  return out.str();
}

std::string userToJson(const User& user) {
  std::ostringstream out;
  out << '{' << "\"uuid\":" << escapeJson(user.getUuid()) << ','
      << "\"name\":" << escapeJson(user.getName()) << ','
      << "\"connected\":false,"
      << "\"teams\":" << jsonArray(user.getTeams(), escapeJson) << ','
      << "\"messages\":" << jsonMapOfArrays(user.getMessages(), messageToJson)
      << '}';
  return out.str();
}

Message messageFromJson(const JsonVal& val) {
  Message msg;
  msg.setSenderUuid(val["sender"].str());
  msg.setReceiverUuid(val["receiver"].str());
  msg.setTimestamp(static_cast<time_t>(val["timestamp"].num()));
  msg.setBody(val["body"].str());
  return msg;
}

Reply replyFromJson(const JsonVal& val) {
  Reply reply;
  reply.setUuid(val["uuid"].str());
  reply.setUserUuid(val["user_uuid"].str());
  reply.setTimestamp(static_cast<time_t>(val["timestamp"].num()));
  reply.setBody(val["body"].str());
  return reply;
}

Thread threadFromJson(const JsonVal& val) {
  Thread thread;
  thread.setUuid(val["uuid"].str());
  thread.setUserUuid(val["user_uuid"].str());
  thread.setTimestamp(static_cast<time_t>(val["timestamp"].num()));
  thread.setTitle(val["title"].str());
  thread.setBody(val["body"].str());
  std::vector<Reply> replies;
  for (const auto& replyVal : val["replies"].array())
    replies.push_back(replyFromJson(replyVal));
  thread.setReplies(std::move(replies));
  return thread;
}

Channel channelFromJson(const JsonVal& val) {
  Channel channel;
  channel.setUuid(val["uuid"].str());
  channel.setName(val["name"].str());
  channel.setDescription(val["description"].str());
  std::vector<Thread> threads;
  for (const auto& threadVal : val["threads"].array())
    threads.push_back(threadFromJson(threadVal));
  channel.setThreads(std::move(threads));
  return channel;
}

Team teamFromJson(const JsonVal& val) {
  Team team;
  team.setUuid(val["uuid"].str());
  team.setName(val["name"].str());
  team.setDescription(val["description"].str());
  std::vector<std::string> subs;
  for (const auto& subVal : val["subscribers"].array())
    subs.push_back(subVal.str());
  team.setSubscriberUuids(subs);
  std::vector<Channel> channels;
  for (const auto& channelVal : val["channels"].array())
    channels.push_back(channelFromJson(channelVal));
  team.setChannels(std::move(channels));
  return team;
}

User userFromJson(const JsonVal& val) {
  User user;
  user.setUuid(val["uuid"].str());
  user.setName(val["name"].str());
  user.setConnectionCount(0);
  std::vector<std::string> teams;
  for (const auto& teamVal : val["teams"].array())
    teams.push_back(teamVal.str());
  user.setTeams(teams);
  std::map<std::string, std::vector<Message>> messages;
  for (const auto& [interlocutor, msgsVal] : val["messages"].object()) {
    std::vector<Message> msgs;
    for (const auto& msgVal : msgsVal.array())
      msgs.push_back(messageFromJson(msgVal));
    messages[interlocutor] = std::move(msgs);
  }
  user.setMessages(messages);
  return user;
}
