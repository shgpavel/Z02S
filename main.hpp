#ifndef MAIN_HPP
#define MAIN_HPP


class Person {
private:
    std::string full_name;
    std::string birth_date;
    std::string phone_number;

public:
    Person(const std::string& name, const std::string& date, const std::string& phone);
    
    ~Person();
    
    std::string operator[] (const std::string& property) const;


    bool fullname_comparator(const Person& other) const;
    
    bool birthday_comparator(const Person& other) const;
    
    unsigned days_before_birthday() const;
};

#endif
