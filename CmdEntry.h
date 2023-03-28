#pragma once

#include <string>
#include <list>
#include <vector>
#include <set>


class CmdEntry
{
    public:
        static std::string stringToOperatorType(std::vector<std::string> s, std::vector<std::string> operatorset);
        static std::string findOperation(std::string operatortype, std::vector<std::string> operFunction);
        static std::string findProduct(std::vector<std::string> s, std::vector<std::string> productset);
        static std::string findOrderBookType(std::vector<std::string> s, std::vector<std::string> orderbookset);
        static size_t findTimeover(std::vector<std::string> s,size_t passedTime);

};
