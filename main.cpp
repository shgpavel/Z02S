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

std::string parse_phone(std::string input) {

    input = std::regex_replace(input, std::regex("\\D+"), "");
    
    if (input.length() == 11) {
    return input;
    } else {
        throw std::invalid_argument("Invalid phone number");
    }
}

std::string parse_birthday(std::string date) {
    

}

int main(void) {

    std::string phone;
    std::cin >> phone;
    
    try {
        std::string digits = parse_phone(phone);
        std::cout << "Phone: " << digits << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    std::string birthday;
    std::cin >> birthday;

    try {
        std::string digits = parse_birthday(birthday);
        std::cout << "Birthday: " << digits << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
