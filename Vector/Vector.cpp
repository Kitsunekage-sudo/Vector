#include <iostream>
#include <string>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <utility>

template<class T>
class Vector {
public:
    Vector() noexcept : array(nullptr), capacity(0), size_(0) {}

    explicit Vector(size_t initialCapacity)
        : array(nullptr), capacity(0), size_(0) {
        if (initialCapacity > 0) {
            array = new T[initialCapacity];
            capacity = initialCapacity;
        }
    }

    Vector(std::initializer_list<T> initList)
        : array(nullptr), capacity(0), size_(0) {
        if (initList.size() > 0) {
            array = new T[initList.size()];
            capacity = initList.size();
            size_t i = 0;
            for (const auto& element : initList) {
                array[i++] = element;
            }
            size_ = i;
        }
    }

    
    Vector(const Vector& other)
        : array(nullptr), capacity(0), size_(0) {
        if (other.capacity > 0) {
            T* newArray = new T[other.capacity];
            try {
                for (size_t i = 0; i < other.size_; ++i)
                    newArray[i] = other.array[i];
            }
            catch (...) {
                delete[] newArray;
                throw;
            }
            array = newArray;
            capacity = other.capacity;
            size_ = other.size_;
        }
    }

    
    Vector(Vector&& other) noexcept
        : array(other.array), capacity(other.capacity), size_(other.size_) {
        other.array = nullptr;
        other.capacity = 0;
        other.size_ = 0;
    }

    
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] array;
            array = other.array;
            capacity = other.capacity;
            size_ = other.size_;
            other.array = nullptr;
            other.capacity = 0;
            other.size_ = 0;
        }
        return *this;
    }

    ~Vector() {
        delete[] array;
    }

    void push_back(const T& element) {
        if (size_ >= capacity) {
            size_t newCap = (capacity == 0) ? 1 : capacity * 2;
            reserve(newCap);
        }
        array[size_++] = element;
    }

    void pop_back() {
        if (size_ == 0) throw std::out_of_range("pop_back on empty Vector");
        --size_;
        
        array[size_].~T();
    }

    void eraseAt(size_t index) {
        if (index >= size_) throw std::out_of_range("eraseAt index out of range");
        for (size_t j = index; j + 1 < size_; ++j) {
            array[j] = array[j + 1];
        }
        pop_back();
    }

    void reserve(size_t newCapacity) {
        if (newCapacity <= capacity) return;
        T* newArray = new T[newCapacity];
        try {
            for (size_t i = 0; i < size_; ++i)
                newArray[i] = array[i];
        }
        catch (...) {
            delete[] newArray;
            throw;
        }
        delete[] array;
        array = newArray;
        capacity = newCapacity;
    }

    size_t getSize() const noexcept { return size_; }
    size_t capacitySize() const noexcept { return capacity; } 

    T& operator[] (size_t index) {
        
        return array[index];
    }
    const T& operator[] (size_t index) const {
        return array[index];
    }

    T& at(size_t index) {
        if (index >= size_) throw std::out_of_range("Vector::at index out of range");
        return array[index];
    }
    const T& at(size_t index) const {
        if (index >= size_) throw std::out_of_range("Vector::at index out of range");
        return array[index];
    }

    
    Vector<T> sort() const {
        Vector<T> result(*this);
        if (result.size_ < 2) return result;
        std::sort(result.array, result.array + result.size_);
        return result;
    }

    
    static Vector<T> sort(Vector<T> vector) {
        if (vector.size_ >= 2) {
            std::sort(vector.array, vector.array + vector.size_);
        }
        return vector;
    }

    void swap(Vector& other) noexcept {
        std::swap(array, other.array);
        std::swap(capacity, other.capacity);
        std::swap(size_, other.size_);
    }

private:
    T* array;
    size_t capacity;
    size_t size_;
};



class User {
    int _id;
    std::string _name;
    std::string _surname;
    std::string _phone;

public:
    User() : _id(0) {}
    User(int id, std::string name, std::string surname, std::string phone)
        : _id(id), _name(std::move(name)), _surname(std::move(surname)), _phone(std::move(phone)) {
    }

    int getId() const { return _id; }
    std::string getName() const { return _name; }
    std::string getSurname() const { return _surname; }
    std::string getPhone() const { return _phone; }

    void setName(const std::string& name) { _name = name; }

    bool operator>(const User& other) const {
        return _id > other._id;
    }

    
    bool operator<(const User& other) const {
        return _id < other._id;
    }
};

class DBServiceUser {
    Vector<User> _users;

public:
    
    void create() {
        _users.push_back(User(1, "Alex", "Smith", "111"));
        _users.push_back(User(5, "Bob", "Brown", "222"));
        _users.push_back(User(3, "Tom", "White", "333"));
        _users.push_back(User(2, "Sam", "Black", "444"));
    }

    
    void read() {
        for (size_t i = 0; i < _users.getSize(); i++) {
            std::cout << "ID: " << _users[i].getId() << std::endl;
            std::cout << "Name: " << _users[i].getName() << std::endl;
            std::cout << "Surname: " << _users[i].getSurname() << std::endl;
            std::cout << "Phone: " << _users[i].getPhone() << std::endl;
            std::cout << "------------------" << std::endl;
        }
    }

    
    void update(int id, const std::string& newName) {
        for (size_t i = 0; i < _users.getSize(); i++) {
            if (_users[i].getId() == id) {
                _users[i].setName(newName);
                break; 
            }
        }
    }

    
    void del(int id) {
        for (size_t i = 0; i < _users.getSize(); i++) {
            if (_users[i].getId() == id) {
                _users.eraseAt(i);
                break;
            }
        }
    }

    
    void sortById() {
        _users = _users.sort();
    }
};

int main() {
    DBServiceUser service;
    service.create();

    std::cout << "Before sort:\n";
    service.read();

    service.sortById();

    std::cout << "\nAfter sort:\n";
    service.read();

    service.update(3, "UpdatedName");

    std::cout << "\nAfter update:\n";
    service.read();

    service.del(2);

    std::cout << "\nAfter delete:\n";
    service.read();

    return 0;
}