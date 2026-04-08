/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** JsonParser
*/

#include "JsonParser.hpp"
#include <cctype>
#include <string>

const JsonVal& JsonVal::operator[](const std::string& key) const {
  auto it = objVal.find(key);
  if (it == objVal.end()) {
    static const JsonVal nullVal{};
    return nullVal;
  }
  return it->second;
}

const std::string& JsonVal::str() const { return sVal; }

double JsonVal::num() const { return nVal; }

const std::vector<JsonVal>& JsonVal::array() const { return arrVal; }

const std::map<std::string, JsonVal>& JsonVal::object() const { return objVal; }

JsonParser::JsonParser(const std::string& src) : _src(src), _pos(0) {}

JsonVal JsonParser::parse() { return parseValue(); }

void JsonParser::skipWs() {
  while (_pos < _src.size() &&
         std::isspace(static_cast<unsigned char>(_src[_pos])))
    ++_pos;
}

std::string JsonParser::parseStr() {
  ++_pos;
  std::string result;
  while (_pos < _src.size() && _src[_pos] != '"') {
    if (_src[_pos] == '\\') {
      ++_pos;
    }
    result += _src[_pos++];
  }
  ++_pos;
  return result;
}

JsonVal JsonParser::parseArray() {
  ++_pos;
  JsonVal val;
  val.type = JsonVal::Type::Arr;
  skipWs();
  while (_pos < _src.size() && _src[_pos] != ']') {
    val.arrVal.push_back(parseValue());
    skipWs();
    if (_src[_pos] == ',') {
      ++_pos;
    }
  }
  ++_pos;
  return val;
}

JsonVal JsonParser::parseObject() {
  ++_pos;
  JsonVal val;
  val.type = JsonVal::Type::Obj;
  skipWs();
  while (_pos < _src.size() && _src[_pos] != '}') {
    std::string key = parseStr();
    skipWs();
    ++_pos;  // ':'
    val.objVal[key] = parseValue();
    skipWs();
    if (_src[_pos] == ',') {
      ++_pos;
    }
    skipWs();
  }
  ++_pos;
  return val;
}

JsonVal JsonParser::parseValue() {
  skipWs();
  if (_pos >= _src.size()) return {};
  char cur = _src[_pos];
  if (cur == '"') {
    JsonVal val;
    val.type = JsonVal::Type::Str;
    val.sVal = parseStr();
    return val;
  }
  if (cur == '[') return parseArray();
  if (cur == '{') return parseObject();
  JsonVal val;
  val.type = JsonVal::Type::Num;
  std::size_t start = _pos;
  if (_src[_pos] == '-') ++_pos;
  while (_pos < _src.size() &&
         std::isdigit(static_cast<unsigned char>(_src[_pos])))
    ++_pos;
  val.nVal = std::stod(_src.substr(start, _pos - start));
  return val;
}
