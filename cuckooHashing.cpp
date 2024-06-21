//
//  cuckooHashing.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#include "cuckooHashing.hpp"
//#include <boost/make_shared.hpp>

using namespace std;

cuckooHashing::cuckooHashing(size_t size) : table1(size, nullptr), table2(size, nullptr), tableSize(size) {}

bool cuckooHashing::insert(uint32_t key) {
    shared_ptr<UserData> data = make_shared<UserData>();
    data->dni = key;
    size_t pos1 = hash1(key);
    size_t pos2 = hash2(key);

    if (!table1[pos1]) {
        table1[pos1] = data;
        return true;
    } else if (!table2[pos2]) {
        table2[pos2] = data;
        return true;
    }

    size_t i = 0;
    while (i < tableSize) {
        std::swap(data, table1[pos1]);
        if (!data) return true;
        pos2 = hash2(data->dni);
        std::swap(data, table2[pos2]);
        if (!data) return true;
        pos1 = hash1(data->dni);
        ++i;
    }

    cerr << "Error: Tablas hash estan llenas, no se pudo insertar la clave" << key << "\n";
    return false;
}

bool cuckooHashing::remove(uint32_t key) {
    size_t pos1 = hash1(key);
    size_t pos2 = hash2(key);

    if (table1[pos1] && table1[pos1]->dni == key) {
        table1[pos1] = nullptr;
        return true;
    } else if (table2[pos2] && table2[pos2]->dni == key) {
        table2[pos2] = nullptr;
        return true;
    }

    return false;
}

shared_ptr<UserData> cuckooHashing::search(uint32_t key) const {
    size_t pos1 = hash1(key);
    size_t pos2 = hash2(key);

    if (table1[pos1] && table1[pos1]->dni == key) {
        return table1[pos1];
    } else if (table2[pos2] && table2[pos2]->dni == key) {
        return table2[pos2];
    }

    return nullptr;
}

size_t cuckooHashing::hash1(uint32_t key) const {
    return key % tableSize;
}

size_t cuckooHashing::hash2(uint32_t key) const {
    return (key / tableSize) % tableSize;
}

