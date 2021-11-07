#include "HashTable.h"
#include <iostream>
#include <list>

using namespace std;


int hash_func(const Key& s, int table_size) {
    int sum = 0;
    for (int k = 0; k < s.length(); k++)
        sum = (int(s[k])*sum + int(s[k])) % table_size;
    return  sum % table_size;
}


//constructor
HashTable::HashTable() {
    size_table = default_size;      // a size of table
    records = 0;                    // quantity of records in a table
    list<pair<Key, Value>>* null_vec = nullptr;
    arr.resize(size_table, null_vec);
}

//destructor
HashTable::~HashTable() {
    vector<list<pair<Key, Value>>*>().swap(arr);
    records = 0;
}

//constructor (HashTable a(b);)
HashTable::HashTable(const HashTable& b) {
    arr = b.arr;
    size_table = b.size_table;
    records = b.records;
}


HashTable::HashTable(HashTable&& b) {
    arr = b.arr;
    size_table = b.size_table;
    records = b.records;
    for (auto& it : b.arr) {
        it = nullptr;
    }
}


HashTable& HashTable::operator=(const HashTable& b) {
    if (this == &b) {
        return *this;
    }
    size_table = b.size_table;
    records = b.records;
    arr.clear();
    arr = b.arr;

    return *this;
}


HashTable& HashTable::operator=(HashTable&& b) {
    if (this == &b) {
        return *this;
    }
    size_table = b.size_table;
    records = b.records;
    arr.clear();
    arr = b.arr;
    for (auto it : b.arr) {
        it = nullptr;
    }
    return *this;
}


void HashTable::resize() {
    vector<list<pair<Key, Value>>*> new_arr = arr;
    arr.clear();
    size_table *= 2;

    list<pair<Key, Value>>* null_vec = nullptr;
    arr.resize(size_table, null_vec);
    for (int i = 0; i < size_table / 2; i++) {
        //if list is not empty
        if (!new_arr[i]->empty()) {
            for (auto& it : *new_arr[i])
                //insert pairs(key ,value)
                insert(it.first, it.second);
        }
    }
}


void HashTable::swap(HashTable& b) {
    std::swap(records, b.records);
    std::swap(size_table, b.size_table);
    arr.swap(b.arr);
}


void HashTable::clear() {
    list<pair<Key, Value>>* null_list = nullptr;
    arr.resize(size_table, null_list);
}


bool HashTable::erase(const Key& k) {
    if (!contains(k))
        return false;

    int hash = hash_func(k, size_table);
    for (auto it = arr[hash]->begin(); it != arr[hash]->end(); it++) {
        if (it->first == k) {
            arr[hash]->erase(it);
            records--;
            return true;
        }
    }
}


bool HashTable::insert(const Key& k, const Value& v) {
    if (contains(k))
        return false;

    int hash = hash_func(k, size_table);

    if (arr[hash] == nullptr) {
        arr[hash] = new list<pair<Key, Value>>;
    }

    arr[hash]->push_front(make_pair(k, v));

    if (arr[hash]->size() > max_size) {
        resize();
    }
    records++;
    return true;
}


bool HashTable::contains(const Key& k) const {
    int hash = hash_func(k, size_table);
    if (arr[hash] == nullptr) {
        return false;
    }

    for (const auto& it : *arr[hash]) {
        if (it.first == k) {
            return true;
        }
    }
    return false;
}


Value& HashTable::operator[](const Key& k) {
    if (contains(k)) {
        int hash = hash_func(k, size_table);
        for (auto& it : *arr[hash]) {
            if (it.first == k) {
                return it.second;
            }
        }
    }
    insert(k, default_student);
    return default_student;
}


Value& HashTable::at(const Key& k) {
    if (contains(k)) {
        int hash = hash_func(k, size_table);
        for (auto& it : *arr[hash]) {
            if (it.first == k) {
                return it.second;
            }
        }
    }
    else {
        throw "It not exist";
    }
}


size_t HashTable::size() const {
    return records;
}


bool HashTable::empty() const {
    if (!records)
        return true;
    else
        return false;
}


bool operator==(const HashTable& a, const HashTable& b) {
    bool size = a.size_table == b.size_table;
    bool arrs = a.arr == b.arr;
    return size && arrs;
}


bool operator!=(const HashTable& a, const HashTable& b) {
    bool size = a.size_table == b.size_table;
    bool arrs = a.arr == b.arr;
    bool r = size && arrs;
    return !r;
}
