#include "hashsetopen.h"
#include <climits>
#include <stdexcept>

static bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

static int nextPrime(int n) {
    if (n < 2) return 2;
    while (!isPrime(n)) {
        if (n == INT_MAX) throw std::overflow_error("Overflow");
        n++;
    }
    return n;
}

static int calculateHash(int key, int capacity) {
    long long k = key;
    long long index = k % capacity;
    if (index < 0) index += capacity;
    return static_cast<int>(index);
}

HashSetOpen::HashSetOpen(int cap) : table(nullptr), capacity(cap), count(0) {
    if (cap <= 0) throw std::invalid_argument("Capacity must be positive");
    capacity = nextPrime(cap);
    table = new std::list<Entry>[capacity];
}

HashSetOpen::~HashSetOpen() { delete[] table; }

void HashSetOpen::insert(int key, const std::string& value) {
    int index = calculateHash(key, capacity);
    for (auto& entry : table[index]) {
        if (entry.key == key) { entry.value = value; return; }
    }
    table[index].emplace_back(key, value);
    count++;
}

bool HashSetOpen::get(int key, std::string& outvalue) const {
    int index = calculateHash(key, capacity);
    for (const auto& entry : table[index]) {
        if (entry.key == key) { outvalue = entry.value; return true; }
    }
    return false;
}

bool HashSetOpen::contains(int key) const {
    std::string dummy;
    return get(key, dummy);
}

bool HashSetOpen::remove(int key) {
    int index = calculateHash(key, capacity);
    auto& bucket = table[index];
    for (auto it = bucket.begin(); it != bucket.end(); ++it) {
        if (it->key == key) { bucket.erase(it); count--; return true; }
    }
    return false;
}

QString HashSetOpen::display() const {
    QString out;
    out += QString("Hash Table Capacity: %1\n").arg(capacity);
    for (int i = 0; i < capacity; i++) {
        out += QString("[%1] ").arg(i);
        if (table[i].empty()) out += "Empty!";
        else {
            for (const auto& entry : table[i]) {
                out += QString("(%1:%2) -> ").arg(entry.key).arg(QString::fromStdString(entry.value));
            }
            out += "null";
        }
        out += "\n";
    }
    return out;
}