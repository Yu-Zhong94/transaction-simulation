#include "CmdEntry.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"
#include <set>
#include <map>
#include <charconv>
#include <cctype>
#include <ostream>
#include <iostream>

//iterate over vector s and return a string of OperatorType
std::string CmdEntry::stringToOperatorType(std::vector<std::string> s, std::vector<std::string> operatorset)
{
  std::string operatorType;
  for(const auto & elem : s)
  {
    for(const auto & e : operatorset)
    {
      if (e == elem) {
        operatorType.append(elem);
      }
    }
  }
  return operatorType;
}

//find corresponding function
std::string CmdEntry::findOperation(std::string s, std::vector<std::string> operFunction)
{
  std::string t;
  for (const auto &e : operFunction) {
    if (e == s) {
        t = s;
        break;
    }
  }
  return t;
}

//find corresponding product
std::string CmdEntry::findProduct(std::vector<std::string> s, std::vector<std::string> productset)
{
  std::string t;
  for(const auto & elem : s)
  {
    for (const auto &e : productset) {
      if (e == elem) {
          t = elem;
          break;
      }
    }
  }
  return t;
}

//find corresponding orderbooktype
std::string CmdEntry::findOrderBookType(std::vector<std::string> s, std::vector<std::string> orderbookset)
{
  std::string t;
  for(const auto & elem : s)
  {
    for (const auto &e : orderbookset) {
      if (e == elem) {
          t = elem;
          break;
      }
    }
  }
  return t;
}

//find corresponding timestamps
size_t CmdEntry::findTimeover(std::vector<std::string> s,size_t passedTime)
{
  size_t t;
  for(const auto & elem : s)
  {
    int result={};
    auto res = std::from_chars( elem.data(), elem.data() + elem.size(), result );
    if (res.ec == std::errc())
    {
      if (result < passedTime)
      {
        t = result;
        break;
      }
    }
  }
  return t;
}