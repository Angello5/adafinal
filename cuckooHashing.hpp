//
//  cuckooHashing.hpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

/*#ifndef CUCKOOHASHING_HPP
#define CUCKOOHASHING_HPP

#include <vector>
#include <cstdint>
#include "userData.h"
using namespace std;

class cuckooHashing {
public:
    explicit cuckooHashing(size_t size);
    bool insert(uint32_t key);
    UserData* search(uint32_t key) const;
    bool remove(uint32_t key);
    void rehash();

private:
    vector<UserData> table1;
    vector<UserData> table2;
    size_t hash1(uint32_t key) const;
    size_t hash2(uint32_t key) const;
    size_t tableSize;
};

#endif // CUCKOOHASHING_HPP
*/
#ifndef CUCKOO_HASHING_HPP
#define CUCKOO_HASHING_HPP

#include <vector>
#include <cstdint>
#include "userData.h"

class CuckooHashing {
public:
    explicit CuckooHashing(size_t size);
    bool insert(uint32_t key);
    UserData* search(uint32_t key) const;
    bool remove(uint32_t key);
    void rehash();
    void clear(); // Nuevo método para limpiar las tablas

private:
    std::vector<UserData*> table1;
    std::vector<UserData*> table2;
    size_t hash1(uint32_t key) const;
    size_t hash2(uint32_t key) const;
    size_t tableSize;
};

#endif // CUCKOO_HASHING_HPP

