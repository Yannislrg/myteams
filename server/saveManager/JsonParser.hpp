/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** JsonParser
*/

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct JsonVal {
  enum class Type : std::uint8_t { Null, Bool, Num, Str, Arr, Obj };
  Type type =
      Type::Null;     // NOLINT(misc-non-private-member-variables-in-classes)
  bool bVal = false;  // NOLINT(misc-non-private-member-variables-in-classes)
  double nVal = 0.0;  // NOLINT(misc-non-private-member-variables-in-classes)
  std::string sVal;   // NOLINT(misc-non-private-member-variables-in-classes)
  std::vector<JsonVal>
      arrVal;  // NOLINT(misc-non-private-member-variables-in-classes)
  std::map<std::string, JsonVal>
      objVal;  // NOLINT(misc-non-private-member-variables-in-classes)

  const JsonVal& operator[](const std::string& key) const;
  [[nodiscard]] const std::string& str() const;
  [[nodiscard]] double num() const;
  [[nodiscard]] const std::vector<JsonVal>& array() const;
  [[nodiscard]] const std::map<std::string, JsonVal>& object() const;
};

class JsonParser {
 public:
  explicit JsonParser(const std::string& src);
  JsonVal parse();

 private:
  std::string _src;
  std::size_t _pos;

  void skipWs();
  std::string parseStr();
  JsonVal parseValue();
  JsonVal parseArray();
  JsonVal parseObject();
};
