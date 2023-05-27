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
#include <fstream>
#include <vector>
#include <stdexcept>
#include <ctime>

#include "main.hpp"


Person::Person(const std::string& name, const std::string& date, const std::string& phone)
    : full_name(name), birth_date(date), phone_number(phone) {}

Person::~Person() {}

std::string Person::operator[] (const std::string& property) const {
        if (property == "name") {
            return full_name;
        } else if(property == "birthday") {
            return birth_date;
        } else if(property == "phone") {
            return phone_number;
        } else {
            return "";
        }
}

bool Person::satisfiesConditions(const std::string& name_condition, const std::string& date_condition,
                                 const std::string& phone_condition) const {
    
    if ((name_condition != "*" && full_name != name_condition) ||
    (date_condition != "*" && birth_date != date_condition) ||
    (phone_condition != "*" && phone_number != phone_condition)) {
        return false;
    }
    return true;
}

int Person::daysBeforeBirthday() const {
    time_t now = time(0);
    struct tm* current_time = localtime(&now);

    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    int birth_year, birth_month, birth_day;
    sscanf(birth_date.c_str(), "%2d %2d %4d", &birth_day, &birth_month, &birth_year);

    int days_until_birthday = 0;

    if (current_month < birth_month || (current_month == birth_month && current_day < birth_day)) {
        struct tm next_birthday = {0};
        next_birthday.tm_year = current_year - 1900;
        next_birthday.tm_mon = birth_month - 1;
        next_birthday.tm_mday = birth_day;
        time_t next_birthday_time = mktime(&next_birthday);
        days_until_birthday = static_cast<int>((next_birthday_time - now) / (60 * 60 * 24));
    } else {
        struct tm next_birthday = {0};
        next_birthday.tm_year = current_year - 1900 + 1;
        next_birthday.tm_mon = birth_month - 1;
        next_birthday.tm_mday = birth_day;
        time_t next_birthday_time = mktime(&next_birthday);
        days_until_birthday = static_cast<int>((next_birthday_time - now) / (60 * 60 * 24));
    }

    return days_until_birthday;
}



std::string parser(std::string input, unsigned flag) {
    
    if (input == "*") {
        return "*";
    } else if (flag % 3 == 1) {
        input = std::regex_replace(input, std::regex("\\D+"), "");
        return input.substr(0, 11);
    } else if (flag % 3 == 2) {

        int birth_year, birth_month, birth_day;
        sscanf(input.c_str(), "%2d %2d %4d", &birth_day, &birth_month, &birth_year);
        if (birth_day > 31 || birth_day < 1) {
            throw std::runtime_error("Day can't be greater 31 and less than 1");
        }
        
        if (birth_month > 12 || birth_month < 1) {
            throw std::runtime_error("Month can't be greater 12 and less than 1");
        }

        if (birth_year > 2023) {
            throw std::runtime_error("We are'nt working in future");
        }

        input = std::regex_replace(input, std::regex("\\D+"), "");
        return input.substr(0, 8);
    } else if (flag % 3  == 0) {
        std::regex re(R"(([^\s]+)\s+([^\s]+)\s+([^\s]+))");
        std::smatch match;
        
        if (std::regex_search(input, match, re)) {
            return match[1].str() + " " + match[2].str() + " " + match[3].str();
        } else {
            throw std::runtime_error("Invalid fullname format");
        }
    }
}

void help() {
    std::cout << "Доступные команды:" << std::endl;
    std::cout << "help - распечатывает это сообщение" << std::endl;
    std::cout << "print - распечатывает все объекты" << std::endl;
    std::cout << "clear - удаляет все объекты класса" << std::endl;
    std::cout << "load <filename> - загружает объекты из файла" << std::endl;
    std::cout << "save <filename> - сохраняет в файл объекты" << std::endl;
    std::cout << "add - добавить объект (введите '>exit' для завершения)" << std::endl;
    std::cout << "sort - отсортировать объекты" << std::endl;
    std::cout << "find <условия> - вывести объекты, удовлетворяющие условиям" << std::endl;
    std::cout << "delete <условия> - удалить объекты, удовлетворяющие условиям" << std::endl;
    std::cout << "birthday - распечатывает людей с близким днем рождения" << std::endl;
    std::cout << "exit - завершить работу и выйти" << std::endl;
}

void clear(std::vector<Person*>& objects) {
    for (Person* obj : objects) {
        delete obj;
    }
    objects.clear();
}


int main(void) {
    std::vector<Person*> objects;
    
    /*  objects.push_back(new Person(fullname, birthday, phone)); */

    std::string command;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command == "help") {
            help();
        } else if (command == "clear") {
            clear(objects);
        } else if (command.substr(0, 5) == "load ") {
            std::string filename = command.substr(5);
            std::ifstream file(filename);
            if (file.is_open()) {
                std::string fullname, birthday, phone, tmp;
                unsigned i = 1;
                while (std::getline(file, tmp)) {
                    switch (i % 3) {
                        case 1:
                            fullname = parser(tmp, 3);
                            break;
                        case 2:
                            birthday = parser(tmp, 2);
                            break;
                        case 0:
                            phone = parser(tmp, 1);
                            objects.push_back(new Person(fullname, birthday, phone));
                            break;
                    }   
                    ++i;
                }
            file.close();
            } else {
                std::cout << "Не получилось получить доступ к файлу" << std::endl;
            }
       } else if (command.substr(0, 5) == "save ") {
            std::string filename = command.substr(5);
            std::ofstream file(filename);
            if (file.is_open()) {
                for (unsigned i = 0; i < objects.size(); ++i) {
                    file << (*objects[i])["name"] << std::endl << (*objects[i])["birthday"] << std::endl << (*objects[i])["phone"] << std::endl;
                }
            file.close();
            } else {
                std::cout << "Не получилось записать в файл" << std::endl;
            }
        } else if (command == "add") {

            std::string fullname, birthday, phone;
            std::cout << "Чтобы добавить человека введите по строчкам имя, дату рождения и номер телефона" << std::endl;
            std::cout << ">> ";
            std::getline(std::cin, fullname);
            std::cout << ">> ";
            std::getline(std::cin, birthday);
            std::cout << ">> ";
            std::getline(std::cin, phone);
            objects.push_back(new Person(parser(fullname, 3), parser(birthday, 2), parser(phone, 1)));
            std::cout << std::endl;
        } else if (command == "print") {
            for (unsigned i = 0; i < objects.size(); ++i) {
                std::cout << (*objects[i])["name"] << " " << (*objects[i])["birthday"] << " " << (*objects[i])["phone"] << " " << std::endl;
            }
        
        /*} else if (command == "sort") {
            sort(list);
        } else if (command.substr(0, 5) == "find ") {
            std::string conditions = command.substr(5);
            find(list, conditions);
        } else if (command.substr(0, 7) == "delete ") {
            std::string conditions = command.substr(7);
            deleteItems(list, conditions);
        */
        } else if (command == "exit") {
            clear(objects);
            break;
        } else {
            std::cout << "Неверная команда. Введите 'help' для списка команд." << std::endl;
        }
    }

    std::cout << "Работа программы завершена." << std::endl;
    return 0;
}
