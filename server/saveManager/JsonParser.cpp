/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** JsonParser
*/

#include "JsonParser.hpp"
#include <charconv>
#include <string>

const JsonVal& JsonVal::operator[](const std::string& key) const {
  auto it = objVal.find(key);
  if (it == objVal.end()) {
    static const JsonVal nullVal{};
    return nullVal;
  }
  return it->second;
}

const std::string& JsonVal::str() const {
  return sVal;
}

double JsonVal::num() const {
  return nVal;
}

const std::vector<JsonVal>& JsonVal::array() const {
  return arrVal;
}

const std::map<std::string, JsonVal>& JsonVal::object() const {
  return objVal;
}

JsonParser::JsonParser(const std::string& src) : _src(src), _pos(0) {}

JsonVal JsonParser::parse() {
  return parseValue();
}

void JsonParser::skipWs() {
  _pos = _src.find_first_not_of(" \t\n\r\f\v", _pos);
  if (_pos == std::string::npos)
    _pos = _src.size();
}

std::string JsonParser::parseStr() {
  ++_pos;
  std::string result;
  while (_pos < _src.size() && _src[_pos] != '"') {
    if (_src[_pos] == '\\')
      ++_pos;
    result += _src[_pos++];
  }
  ++_pos;
  return result;
}

JsonVal JsonParser::parseNum() {
  JsonVal val;
  val.type = JsonVal::Type::Num;
  auto [ptr, ec] = std::from_chars(_src.data() + _pos, _src.data() + _src.size(), val.nVal);
  _pos = ptr - _src.data();
  return val;
}

JsonVal JsonParser::parseArray() {
  ++_pos;
  JsonVal val;
  val.type = JsonVal::Type::Arr;
  skipWs();
  while (_pos < _src.size() && _src[_pos] != ']') {
    val.arrVal.push_back(parseValue());
    skipWs();
    if (_src[_pos] == ',')
      ++_pos;
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
    if (_src[_pos] == ',')
      ++_pos;
    skipWs();
  }
  ++_pos;
  return val;
}

JsonVal JsonParser::parseValue() {
  skipWs();
  if (_pos >= _src.size()) return {};
  switch (_src[_pos]) {
    case '"': {
      JsonVal v;
      v.type = JsonVal::Type::Str;
      v.sVal = parseStr();
      return v;
    }
    case '[': return parseArray();
    case '{': return parseObject();
    case 't': {
      _pos += 4;
      JsonVal v;
      v.type = JsonVal::Type::Bool;
      v.bVal = true;
      return v;
    }
    case 'f': {
      _pos += 5;
      JsonVal v;
      v.type = JsonVal::Type::Bool;
      v.bVal = false;
      return v;
    }
    case 'n': {
      _pos += 4;
      return {};
    }
    default:  return parseNum();
  }
}
