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

template<typename T>
class ListWrapper {
public:
    ListWrapper();

    void push_back(const T& value);

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    typename std::list<T>::iterator begin();
    typename std::list<T>::const_iterator begin() const;

    size_t size() const;

    void clear();
    void erase(typename std::list<T>::iterator it);

    ListWrapper<T>& operator=(const ListWrapper<T>& other);
    typename std::list<T>::iterator end();
    typename std::list<T>::const_iterator end() const;
private:
    std::list<T> myList;
};

#endif
