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
#include <list>
#include <iterator>
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


bool Person::fullname_comparator(const Person& other) const {
    if (full_name < other.full_name || full_name == "*") {
        return 1;
    }
    return 0;
}

bool Person::birthday_comparator(const Person& other) const {
    unsigned birth_year, birth_month, birth_day, birth_year_2, birth_month_2, birth_day_2;
    if (birth_date == "*") {
        return 1;
    } else {
        sscanf(birth_date.c_str(), "%2u %2u %4u", &birth_day, &birth_month, &birth_year);
        sscanf(other.birth_date.c_str(), "%2u %2u %4u", &birth_day_2, &birth_month_2, &birth_year_2);
        if (birth_year < birth_year_2) {
            return 1;
        } else if(birth_year == birth_year_2) {
            if (birth_month < birth_month_2) {
                return 1;
            } else if(birth_month == birth_month_2) {
                if (birth_day < birth_day_2) {
                    return 1;
                }
            }
        }
    }
    return 0;
}


unsigned Person::days_before_birthday() const {
    if (birth_date.find("*") != std::string::npos) { return 0; }
    time_t now = time(0);
    struct tm* current_time = localtime(&now);

    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    int birth_year, birth_month, birth_day;
    sscanf(birth_date.c_str(), "%2d %2d %4d", &birth_day, &birth_month, &birth_year);

    unsigned days_until_birthday = 0;

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

template<typename T>
ListWrapper<T>::ListWrapper() {}

template<typename T>
void ListWrapper<T>::push_back(const T& value) {
    myList.push_back(value);
}

template<typename T>
T& ListWrapper<T>::operator[](size_t index) {
    auto it = myList.begin();
    std::advance(it, index);
    return *it;
}

template<typename T>
const T& ListWrapper<T>::operator[](size_t index) const {
    auto it = myList.begin();
    std::advance(it, index);
    return *it;
}

template<typename T>
void ListWrapper<T>::clear() {
    std::for_each(myList.begin(), myList.end(), [](T obj) {
        delete obj;
    });
    myList.clear();
}

template<typename T>
typename std::list<T>::iterator ListWrapper<T>::begin() {
    return myList.begin();
}

template<typename T>
typename std::list<T>::const_iterator ListWrapper<T>::begin() const {
    return myList.begin();
}

template<typename T>
size_t ListWrapper<T>::size() const {
    return myList.size();
}


template<typename T>
ListWrapper<T>& ListWrapper<T>::operator=(const ListWrapper<T>& other) {
    if (this != &other) {
        myList = other.myList;
    }
    return *this;
}

template<typename T>
typename std::list<T>::iterator ListWrapper<T>::end() {
    return myList.end();
}

template<typename T>
typename std::list<T>::const_iterator ListWrapper<T>::end() const {
    return myList.end();
}

template<typename T>
void ListWrapper<T>::erase(typename std::list<T>::iterator it) {
    myList.erase(it);
}


std::string parser(std::string input, unsigned flag) {
    if (input == "*") {
        return "*";
    } else if (flag % 3 == 1) {
        input = std::regex_replace(input, std::regex("\\D+"), "");
        return input.substr(0, 11);
    } else if (flag % 3 == 2) {

        int birth_month, birth_day;
        sscanf(input.c_str(), "%2d %2d", &birth_day, &birth_month);
        if (birth_month == 2){
            if (birth_day > 29 || birth_day < 1) {
                throw std::invalid_argument("No such day in this month");
            }
        } else if (birth_month == 1 || birth_month == 3 || birth_month == 5 
                || birth_month == 7 || birth_month == 8 || birth_month == 10 || birth_month == 12) {
            if (birth_day > 31 || birth_day < 1) {
                throw std::invalid_argument("No such day in this month");
            }
        } else if (birth_day > 30 || birth_day < 1) {
            throw std::invalid_argument("No such day in this month");
        }
        
        if (birth_month > 12 || birth_month < 1) {
            throw std::invalid_argument("Month can't be greater 12 and less than 1");
        }
       
        input = std::regex_replace(input, std::regex("\\D+"), "");
        return input.substr(0, 8);
    } else if (flag % 3  == 0) {
        std::regex re(R"(([^\s]+)\s+([^\s]+)\s+([^\s]+))");
        std::smatch match;
        
        if (std::regex_search(input, match, re)) {
            return match[1].str() + " " + match[2].str() + " " + match[3].str();
        } else {
            throw std::invalid_argument("Invalid fullname format");
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
    std::cout << "sort [mode(3, 2)] - отсортировать объекты" << std::endl;
    std::cout << "find [mode(3, 2, 1)] <условия> - вывести объекты, удовлетворяющие условиям" << std::endl;
    std::cout << "delete [mode(3, 2, 1)] <условия> - удалить объекты, удовлетворяющие условиям" << std::endl;
    std::cout << "birthday - распечатывает людей с близким днем рождения" << std::endl;
    std::cout << "exit - завершить работу и выйти" << std::endl;
    std::cout << "спецификатор mode принимает значения 1(номер телефона), 2(дата рождения), 3(ФИО)" << std::endl;
}



void sort(ListWrapper<Person*>& objects, unsigned size, unsigned mode) {
    if (mode == 3) {
        unsigned j;
        for (unsigned i = 1; i < size; ++i) {
            j = i;
            while (j > 0 && ((*objects[j]).fullname_comparator((*objects[j - 1])))) {
                std::swap(objects[j], objects[j - 1]);
                j--;
            }
        }
    } else if (mode == 2) {
    unsigned j;
        for (unsigned i = 1; i < size; ++i) {
            j = i;
            while (j > 0 && ((*objects[j]).birthday_comparator((*objects[j - 1])))) {
                std::swap(objects[j], objects[j - 1]);
                j--;
            }
        }
    } else if (mode == 1){
        unsigned j;
        for (unsigned i = 1; i < size; ++i) {
            j = i;
            while (j > 0 && ((*objects[j]).days_before_birthday() < (*objects[j - 1]).days_before_birthday())) {
                std::swap(objects[j], objects[j - 1]);
                j--;
            }
        }
        
    } else {
        throw std::invalid_argument("Такого режима нет");
    }
}


void find_obj (ListWrapper<Person*>& objects, std::string condition, unsigned mode) {
    unsigned flag = 0;
    for (unsigned i = 0; i < objects.size(); ++i) {
        if ((mode == 3 && (*objects[i])["name"] == parser(condition, 3)) ||
                (mode == 2 && (*objects[i])["birthday"] == parser(condition, 2)) ||
                (mode == 1 && (*objects[i])["phone"] == parser(condition, 1))) {
            std::cout << (*objects[i])["name"] << " "  <<
                (*objects[i])["birthday"] << " " <<
                (*objects[i])["phone"] << std::endl;
            flag = 1;
        }
        }
    if (flag == 0) {
        std::cout << "Такой объект не найден" << std::endl;
    }
}
/*
void delete_obj (ListWrapper<Person*>& objects, std::string condition, unsigned mode, unsigned flag) {
    for (unsigned i = 0; i < objects.size(); ++i) {
        if ((mode == 3 && (*objects[i])["name"] == parser(condition, 3)) ||
                (mode == 2 && (*objects[i])["birthday"] == parser(condition, 2)) ||
                (mode == 1 && (*objects[i])["phone"] == parser(condition, 1))){
            objects.erase(std::advance(objects.begin(), i));
            flag = 1;
            delete_obj(objects, condition, mode, flag);
        }
    }
    if (flag == 0) {
        std::cout << "Такой объект не найден" << std::endl;
    }
}
*/
void delete_obj(ListWrapper<Person*>& objects, std::string condition, unsigned mode, unsigned flag) {
    for (auto it = objects.begin(); it != objects.end(); ) {
        if (((mode == 3) && ((*it)["name"] == parser(condition, 3))) ||
            ((mode == 2) && ((*it)["birthday"] == parser(condition, 2))) ||
            ((mode == 1) && ((*it)["phone"] == parser(condition, 1)))) {
            delete *it;
            objects.erase(it);
            delete_obj(objects, condition, mode, flag);
            flag = 1;
        } else {
            ++it;
        }
    }
    if (flag == 0) {
        std::cout << "Такой объект не найден" << std::endl;
    }
}

int main(void) {
    ListWrapper<Person*> objects;
    
    std::string command;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);

        if (command.substr(0, 4) == "help") {
            help();
        } else if (command.substr(0, 5) == "clear") {
            objects.clear();
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
                    file << (*objects[i])["name"] << std::endl 
                        << (*objects[i])["birthday"] << std::endl
                        << (*objects[i])["phone"] << std::endl;
                }
            file.close();
            } else {
                std::cout << "Не получилось записать в файл" << std::endl;
            }
        } else if (command.substr(0, 3) == "add") {

            std::string fullname, birthday, phone;
            std::cout << "Чтобы добавить объект введите по строчкам ФИО, дату рождения и номер телефона" << std::endl;
            std::cout << ">> ";
            std::getline(std::cin, fullname);
            std::cout << ">> ";
            std::getline(std::cin, birthday);
            std::cout << ">> ";
            std::getline(std::cin, phone);
            try {
            objects.push_back(new Person(parser(fullname, 3), parser(birthday, 2), parser(phone, 1)));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
            std::cout << std::endl;
        } else if (command.substr(0, 5) == "print") {
            for (unsigned i = 0; i < objects.size(); ++i) {
                std::cout << (*objects[i])["name"] << " " 
                    << (*objects[i])["birthday"] << " " 
                    << (*objects[i])["phone"] << " " << std::endl;
            }
        
        } else if (command.substr(0, 5) == "sort ") {
            std::string mode = command.substr(5, 6);
            sort(objects, objects.size(), std::stoul(mode));
        } else if (command.substr(0, 5) == "find ") {
            std::string mode = command.substr(5, 6), conditions;
            std::cout << "Далее введите выбранный параметр:" << std::endl;
            std::cout << "~> ";
            std::getline(std::cin, conditions);
            try {
                find_obj(objects, conditions, std::stoul(mode));
            } catch(const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } else if (command.substr(0, 7) == "delete ") {
            std::string mode = command.substr(7, 8), conditions;
            std::cout << "Далее введите выбранный параметр:" << std::endl;
            std::cout << "~> ";
            std::getline(std::cin, conditions);
            delete_obj(objects, conditions, std::stoul(mode), 0);
        } else if (command.substr(0, 8) == "birthday") {
            sort(objects, objects.size(), 1);
        } else if (command.substr(0, 4) == "exit") {
            objects.clear();
            break;
        } else {
            std::cout << "Неверная команда. Введите 'help' для списка команд." << std::endl;
        }
    }

    std::cout << "Работа программы завершена." << std::endl;
    return 0;
}
