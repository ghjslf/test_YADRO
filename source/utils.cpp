#include <vector>
#include <string>
#include <sstream>
#include <cctype>

#include "utils.hpp"


std::vector<std::string> split(const std::string &string, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream token_stream(string);
   while (std::getline(token_stream, token, delimiter)) {
      tokens.push_back(token);
   }
   return tokens;
}

int convert_to_integer(const std::string &x) {
    int result;
    std::stringstream ss;
    ss << x;
    ss >> result;
    
    return result;
}

std::string convert_to_string(int x) {
    std::string result;
    std::stringstream ss;
    ss << x;
    ss >> result;
    
    return result;
}

bool is_number(std::string value) {
    for (size_t i = 1; i < value.size(); i++) {
        if (!std::isdigit(value[i])) {
            return false;
        }
    }
    return true;
}