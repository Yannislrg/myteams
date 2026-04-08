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
    if (chr == '"') {
      out += "\\\"";
    } else if (chr == '\\') {
      out += "\\\\";
    } else {
      out += chr;
    }
  }
  out += '"';
  return out;
}

std::string messageToJson(const Message& msg) {
  std::ostringstream output;
  output << '{' << "\"sender\":" << escapeJson(msg.getSenderUuid()) << ','
         << "\"receiver\":" << escapeJson(msg.getReceiverUuid()) << ','
         << "\"timestamp\":" << msg.getTimestamp() << ','
         << "\"body\":" << escapeJson(msg.getBody()) << '}';
  return output.str();
}

std::string replyToJson(const Reply& reply) {
  std::ostringstream output;
  output << '{' << "\"uuid\":" << escapeJson(reply.getUuid()) << ','
         << "\"user_uuid\":" << escapeJson(reply.getUserUuid()) << ','
         << "\"timestamp\":" << reply.getTimestamp() << ','
         << "\"body\":" << escapeJson(reply.getBody()) << '}';
  return output.str();
}

std::string threadToJson(const Thread& thread) {
  std::ostringstream output;
  output << '{' << "\"uuid\":" << escapeJson(thread.getUuid()) << ','
         << "\"user_uuid\":" << escapeJson(thread.getUserUuid()) << ','
         << "\"timestamp\":" << thread.getTimestamp() << ','
         << "\"title\":" << escapeJson(thread.getTitle()) << ','
         << "\"body\":" << escapeJson(thread.getBody()) << ','
         << "\"replies\":[";
  bool first = true;
  for (const auto& reply : thread.getReplies()) {
    if (!first) {
      output << ',';
    }
    output << replyToJson(reply);
    first = false;
  }
  output << "]}";
  return output.str();
}

std::string channelToJson(const Channel& channel) {
  std::ostringstream output;
  output << '{' << "\"uuid\":" << escapeJson(channel.getUuid()) << ','
         << "\"name\":" << escapeJson(channel.getName()) << ','
         << "\"description\":" << escapeJson(channel.getDescription()) << ','
         << "\"threads\":[";
  bool first = true;
  for (const auto& thread : channel.getThreads()) {
    if (!first) {
      output << ',';
    }
    output << threadToJson(thread);
    first = false;
  }
  output << "]}";
  return output.str();
}

std::string teamToJson(const Team& team) {
  std::ostringstream output;
  output << '{' << "\"uuid\":" << escapeJson(team.getUuid()) << ','
         << "\"name\":" << escapeJson(team.getName()) << ','
         << "\"description\":" << escapeJson(team.getDescription()) << ','
         << "\"subscribers\":[";
  bool first = true;
  for (const auto& sub : team.getSubscriberUuids()) {
    if (!first) {
      output << ',';
    }
    output << escapeJson(sub);
    first = false;
  }
  output << "],\"channels\":[";
  first = true;
  for (const auto& channel : team.getChannels()) {
    if (!first) {
      output << ',';
    }
    output << channelToJson(channel);
    first = false;
  }
  output << "]}";
  return output.str();
}

std::string userToJson(const User& user) {
  std::ostringstream output;
  output << '{' << "\"uuid\":" << escapeJson(user.getUuid()) << ','
         << "\"name\":" << escapeJson(user.getName()) << ','
         << "\"connected\":false,"
         << "\"teams\":[";
  bool first = true;
  for (const auto& teamUuid : user.getTeams()) {
    if (!first) {
      output << ',';
    }
    output << escapeJson(teamUuid);
    first = false;
  }
  output << "],\"messages\":{";
  first = true;
  for (const auto& [interlocutor, msgs] : user.getMessages()) {
    if (!first) {
      output << ',';
    }
    output << escapeJson(interlocutor) << ":[";
    bool firstMsg = true;
    for (const auto& msg : msgs) {
      if (!firstMsg) {
        output << ',';
      }
      output << messageToJson(msg);
      firstMsg = false;
    }
    output << ']';
    first = false;
  }
  output << "]}";
  return output.str();
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
  for (const auto& replyVal : val["replies"].array()) {
    replies.push_back(replyFromJson(replyVal));
  }
  thread.setReplies(std::move(replies));
  return thread;
}

Channel channelFromJson(const JsonVal& val) {
  Channel channel;
  channel.setUuid(val["uuid"].str());
  channel.setName(val["name"].str());
  channel.setDescription(val["description"].str());
  std::vector<Thread> threads;
  for (const auto& threadVal : val["threads"].array()) {
    threads.push_back(threadFromJson(threadVal));
  }
  channel.setThreads(std::move(threads));
  return channel;
}

Team teamFromJson(const JsonVal& val) {
  Team team;
  team.setUuid(val["uuid"].str());
  team.setName(val["name"].str());
  team.setDescription(val["description"].str());
  std::vector<std::string> subs;
  for (const auto& subVal : val["subscribers"].array()) {
    subs.push_back(subVal.str());
  }
  team.setSubscriberUuids(subs);
  std::vector<Channel> channels;
  for (const auto& channelVal : val["channels"].array()) {
    channels.push_back(channelFromJson(channelVal));
  }
  team.setChannels(std::move(channels));
  return team;
}

User userFromJson(const JsonVal& val) {
  User user;
  user.setUuid(val["uuid"].str());
  user.setName(val["name"].str());
  user.setConnected(false);
  std::vector<std::string> teams;
  for (const auto& teamVal : val["teams"].array()) {
    teams.push_back(teamVal.str());
  }
  user.setTeams(teams);
  std::map<std::string, std::vector<Message>> messages;
  for (const auto& [interlocutor, msgsVal] : val["messages"].object()) {
    std::vector<Message> msgs;
    for (const auto& msgVal : msgsVal.array()) {
      msgs.push_back(messageFromJson(msgVal));
    }
    messages[interlocutor] = std::move(msgs);
  }
  user.setMessages(messages);
  return user;
}
