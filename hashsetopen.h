#ifndef HASHSETOPEN_H
#define HASHSETOPEN_H

#include <list>
#include <string>
#include <QString>

class HashSetOpen {
private:
    struct Entry {
        int key;
        std::string value;
        Entry(int k, const std::string& v) : key(k), value(v) {}
    };
    std::list<Entry>* table;
    int capacity;
    int count;

public:
    explicit HashSetOpen(int cap);
    ~HashSetOpen();

    HashSetOpen(const HashSetOpen&) = delete;
    HashSetOpen& operator=(const HashSetOpen&) = delete;

    void insert(int key, const std::string& value);
    bool get(int key, std::string& outvalue) const;
    bool contains(int key) const;
    bool remove(int key);
    int size() const { return count; }
    QString display() const;
};

#endif