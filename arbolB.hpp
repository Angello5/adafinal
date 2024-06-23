#ifndef arbolB_hpp
#define arbolB_hpp

#pragma once
#include <stdio.h>
#include "userData.h"
#include <vector>
#include <cstdint>

using namespace std;

class BTreeNode {
private:
    vector<uint32_t> keys;
    vector<UserData> userData;
    vector<BTreeNode*> children;
    bool leaf;
    int t;

public:
    BTreeNode(int t, bool leaf);
    ~BTreeNode();
    void traverse();
    void insertNonFull(uint32_t k, const UserData& data);
    void splitChild(int i, BTreeNode* y);
    UserData* search(uint32_t k);
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


class Btree {
    BTreeNode* root;
    int t;

public:
    Btree(int t);
    ~Btree();
    void traverse();
    void insert(uint32_t k, const UserData& data);
    UserData* search(uint32_t k);
    bool remove(uint32_t k);
};

#endif /* arbolB_hpp */
