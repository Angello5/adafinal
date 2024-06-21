//
//  arbolB.hpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#ifndef arbolB_hpp
#define arbolB_hpp


#pragma once
#include <stdio.h>
#include "userData.h"
//#include <boost/shared_ptr.hpp>
//#include <boost/make_shared.hpp>
#include <vector>
#include <cstdint>
#include <memory>

using namespace std;

class BTreeNode
{
    private:

        vector <uint32_t> keys;
        vector <shared_ptr<UserData>> userData;
        vector <BTreeNode*> children;
        bool leaf;
        int t;

    public:
    
        BTreeNode(int t, bool leaf);
        ~BTreeNode();
        void traverse();
        void insertNonFull(uint32_t k, const shared_ptr<UserData>& data);
        void splitChild(int i, BTreeNode* y);
        shared_ptr<UserData> search(uint32_t k);
        void remove(uint32_t k);
        void removeLeaf(int idx);
        void removeNoLeaf(int idx);
        uint32_t getPred(int idx);
        uint32_t getSucc(int idx);
        void fill(int idx);
        void borrowPrev(int idx);
        void borrowNext(int idx);
        void merge(int idx);
    
        friend class Btree;
};

class Btree
{
    BTreeNode* root;
    int t;
    
    public:
        Btree(int t);
        ~Btree();
        void traverse();
        void insert(uint32_t k, const shared_ptr<UserData>& data);
        shared_ptr<UserData> search(uint32_t k);
        void remove(uint32_t k);
};



#endif /* arbolB_hpp */
