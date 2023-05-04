/*
Copyright 2023 Pavel Shago

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <iostream>
#include <string>
#include <regex>
#include <stdexcept>


enum parser_flags {
    Phone,
    Date,
    Fullname,
    Nothing
};


std::string parser(std::string input, parser_flags flag) {
    
    if (flag == Phone) {
        input = std::regex_replace(input, std::regex("\\D+"), "");
        if (input.length() == 11) {
            return input;
        } else {
            throw std::invalid_argument("Invalid phone number");
        }
    }
    
    if (flag == Date) {
        input = std::regex_replace(input, std::regex("\\D+"), "");
        if (input.length() == 8) {
            return input;
        } else {
            throw std::invalid_argument("Invalid date");
        }
    }
    
    if (flag == Fullname) {
        std::regex re(R"(([^\s]+)\s+([^\s]+)\s+([^\s]+))");
        std::smatch match;
        
        if (std::regex_search(input, match, re)) {
            return match[1].str() + "0" + match[2].str() + "0" + match[3].str();
        } else {
            throw std::invalid_argument("Invalid full name format");
        }
    }

    if (flag == Nothing) {
        throw std::invalid_argument("Invalid function usage");
    }

}

int main(void) {

    std::string phone;
    std::getline(std::cin, phone);
    
    try {
        std::string number = parser(phone, Phone);
        std::cout << "Phone: " << number << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    

    std::string birthday;
    std::getline(std::cin, birthday);

    try {
        std::string date = parser(birthday, Date);
        std::cout << "Birthday: " << date << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    std::string fullname;
    std::getline(std::cin, fullname);
    
    try {
        std::string date = parser(fullname, Fullname);
        std::cout << "Birthday: " << date << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
