//
//  cuckooHashing.hpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#ifndef cuckooHashing_hpp
#define cuckooHashing_hpp
#include <stdio.h>
//#include <boost/shared_ptr.hpp>
#include "userData.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>
#include <cstdint>

using namespace std;

class cuckooHashing {

public:
    explicit cuckooHashing(size_t size);
    bool insert(uint32_t key);
    shared_ptr<UserData> search(uint32_t key) const;
    bool remove(uint32_t key);
    void rehash( );
    
private:
    vector<shared_ptr<UserData>> table1;
    vector<shared_ptr<UserData>> table2;
    size_t hash1 (uint32_t key) const;
    size_t hash2 (uint32_t key) const;
    size_t tableSize;

        
};



#endif /* cuckooHashing_hpp */
