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


    bool days_before_birthday_comp(const Person& other);

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

    typename std::list<T>::iterator begin();

    size_t size() const;

    void clear();
    void erase(typename std::list<T>::iterator it);

    ListWrapper<T>& operator=(const ListWrapper<T>& other);
    typename std::list<T>::iterator end();

    void quicksort(typename std::list<T>::iterator, typename std::list<T>::iterator, unsigned);


private:
    std::list<T> myList;
};

#endif
