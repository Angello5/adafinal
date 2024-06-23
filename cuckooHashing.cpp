/*#include "cuckooHashing.hpp"
#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

cuckooHashing::cuckooHashing(size_t size) : tableSize(size) {
    table1.resize(tableSize);
    table2.resize(tableSize);
    // Initialize tables with empty UserData entries
    for (auto &entry : table1) {
        entry.dni = 0;
    }
    for (auto &entry : table2) {
        entry.dni = 0;
    }
}

bool cuckooHashing::insert(uint32_t key) {
    if (search(key) != nullptr) {
        std::cerr << "Error: La clave " << key << " ya está insertada." << std::endl;
        return false;
    }

    UserData newNode;
    newNode.dni = key;

    int limit = 50; // Limita el número de ciclos para evitar ciclos infinitos
    for (int count = 0; count < limit; count++) {
        // Intenta insertar en Table1
        size_t pos1 = hash1(key);
        if (table1[pos1].dni == 0) {
            table1[pos1] = newNode;
            return true;
        }
        std::swap(newNode, table1[pos1]);

        // Intenta insertar en Table2
        size_t pos2 = hash2(newNode.dni);
        if (table2[pos2].dni == 0) {
            table2[pos2] = newNode;
            return true;
        }
        std::swap(newNode, table2[pos2]);
    }

    // Rehash si no se pudo insertar después de varios intentos
    rehash();
    return insert(newNode.dni);
}

bool cuckooHashing::remove(uint32_t key) {
    size_t pos1 = hash1(key);
    if (table1[pos1].dni == key) {
        table1[pos1].dni = 0;
        return true;
    }

    size_t pos2 = hash2(key);
    if (table2[pos2].dni == key) {
        table2[pos2].dni = 0;
        return true;
    }

    return false;
}

UserData* cuckooHashing::search(uint32_t key) const {
    size_t pos1 = hash1(key);
    if (table1[pos1].dni == key) {
        return const_cast<UserData*>(&table1[pos1]);
    }

    size_t pos2 = hash2(key);
    if (table2[pos2].dni == key) {
        return const_cast<UserData*>(&table2[pos2]);
    }

    return nullptr;
}

void cuckooHashing::rehash() {
    std::vector<UserData> oldTable1 = table1, oldTable2 = table2;
    tableSize *= 2; // Duplica el tamaño de las tablas
    table1.clear();
    table1.resize(tableSize);
    table2.clear();
    table2.resize(tableSize);

    // Initialize tables with empty UserData entries
    for (auto &entry : table1) {
        entry.dni = 0;
    }
    for (auto &entry : table2) {
        entry.dni = 0;
    }

    for (auto& entry : oldTable1) {
        if (entry.dni != 0) {
            insert(entry.dni);
        }
    }
    for (auto& entry : oldTable2) {
        if (entry.dni != 0) {
            insert(entry.dni);
        }
    }
}

size_t cuckooHashing::hash1(uint32_t key) const {
    return key % tableSize;
}

size_t cuckooHashing::hash2(uint32_t key) const {
    return (key / tableSize) % tableSize;
}

*/
#include "cuckooHashing.hpp"
#include <iostream>

CuckooHashing::CuckooHashing(size_t size) : tableSize(size) {
    table1.resize(tableSize, nullptr);
    table2.resize(tableSize, nullptr);
}

bool CuckooHashing::insert(uint32_t key) {
    if (search(key) != nullptr) {
        std::cerr << "Error: La clave " << key << " ya está insertada." << std::endl;
        return false;
    }

    UserData* newNode = new UserData();
    newNode->dni = key;

    int limit = 50; // Limita el número de ciclos para evitar ciclos infinitos
    for (int count = 0; count < limit; count++) {
        size_t pos1 = hash1(key);
        if (table1[pos1] == nullptr) {
            table1[pos1] = newNode;
            return true;
        }
        std::swap(newNode, table1[pos1]);

        size_t pos2 = hash2(newNode->dni);
        if (table2[pos2] == nullptr) {
            table2[pos2] = newNode;
            return true;
        }
        std::swap(newNode, table2[pos2]);
    }

    rehash();
    return insert(newNode->dni);
}

bool CuckooHashing::remove(uint32_t key) {
    size_t pos1 = hash1(key);
    if (table1[pos1] != nullptr && table1[pos1]->dni == key) {
        delete table1[pos1];
        table1[pos1] = nullptr;
        return true;
    }

    size_t pos2 = hash2(key);
    if (table2[pos2] != nullptr && table2[pos2]->dni == key) {
        delete table2[pos2];
        table2[pos2] = nullptr;
        return true;
    }

    return false;
}

UserData* CuckooHashing::search(uint32_t key) const {
    size_t pos1 = hash1(key);
    if (table1[pos1] != nullptr && table1[pos1]->dni == key) {
        return table1[pos1];
    }

    size_t pos2 = hash2(key);
    if (table2[pos2] != nullptr && table2[pos2]->dni == key) {
        return table2[pos2];
    }

    return nullptr;
}

void CuckooHashing::rehash() {
    std::vector<UserData*> oldTable1 = table1;
    std::vector<UserData*> oldTable2 = table2;
    tableSize *= 2;
    table1.clear();
    table1.resize(tableSize, nullptr);
    table2.clear();
    table2.resize(tableSize, nullptr);

    for (auto& entry : oldTable1) {
        if (entry != nullptr) {
            insert(entry->dni);
            delete entry;
        }
    }
    for (auto& entry : oldTable2) {
        if (entry != nullptr) {
            insert(entry->dni);
            delete entry;
        }
    }
}

void CuckooHashing::clear() {
    for (auto& entry : table1) {
        delete entry;
        entry = nullptr;
    }
    for (auto& entry : table2) {
        delete entry;
        entry = nullptr;
    }
}

size_t CuckooHashing::hash1(uint32_t key) const {
    return key % tableSize;
}

size_t CuckooHashing::hash2(uint32_t key) const {
    return (key / tableSize) % tableSize;
}

