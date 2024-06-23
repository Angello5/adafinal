//
//  arbolB.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#include "arbolB.hpp"
#include <iostream>

using namespace std;

BTreeNode::BTreeNode(int t, bool leaf) : t(t), leaf(leaf) {
    keys.resize(2 * t - 1);
    userData.resize(2 * t - 1);
    children.resize(2 * t);
}

BTreeNode::~BTreeNode() {
    for (BTreeNode* child : children) {
        delete child;
    }
}

void BTreeNode::traverse() {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!leaf) {
            children[i]->traverse();
        }
        cout << " " << keys[i];
    }
    if (!leaf) {
        children[i]->traverse();
    }
}

UserData* BTreeNode::search(uint32_t k) {
    int i = 0;
    while (i < keys.size() && k > keys[i]) {
        i++;
    }
    if (keys[i] == k) {
        return &userData[i];
    }
    if (leaf) {
        return nullptr;
    }
    return children[i]->search(k);
}

void BTreeNode::insertNonFull(uint32_t k, const UserData& data) {
    /*int i = static_cast<int>(keys.size()) - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            userData[i + 1] = userData[i];
            i--;
        }
        keys[i + 1] = k;
        userData[i + 1] = data;
    } else {
        while (i >= 0 && keys[i] > k) {
            i--;
        }
        if (children[i + 1]->keys.size() == 2 * t - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k) {
                i++;
            }
        }
        children[i + 1]->insertNonFull(k, data);
    }*/
    int i =static_cast<int>(keys.size())  - 1;

        if (leaf) {
            keys.push_back(0); // Incrementa el tamaño del vector
            userData.push_back(UserData());

            while (i >= 0 && keys[i] > k) {
                keys[i + 1] = keys[i];
                userData[i + 1] = userData[i];
                i--;
            }

            keys[i + 1] = k;
            userData[i + 1] = data;
        } else {
            while (i >= 0 && keys[i] > k) {
                i--;
            }

            if (children[i + 1]->keys.size() == 2 * t - 1) {
                splitChild(i + 1, children[i + 1]);

                if (keys[i + 1] < k) {
                    i++;
                }
            }
            children[i + 1]->insertNonFull(k, data);
        }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    
    /*BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->keys.resize(t - 1);
    z->userData.resize(t - 1);
    for (int j = 0; j < t - 1; j++) {
        z->keys[j] = y->keys[j + t];
        z->userData[j] = y->userData[j + t];
    }
    if (!y->leaf) {
        z->children.resize(t);
        for (int j = 0; j < t; j++) {
            z->children[j] = y->children[j + t];
        }
    }
    y->keys.resize(t - 1);
    y->userData.resize(t - 1);
    for (int j = static_cast<int>(keys.size()); j >= i + 1; j--) {
        children[j + 1] = children[j];
    }
    children[i + 1] = z;
    for (int j = static_cast<int>(keys.size()) - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
        userData[j + 1] = userData[j];
    }
    keys[i] = y->keys[t - 1];
    userData[i] = y->userData[t - 1];*/
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
        z->keys.resize(t - 1);
        z->userData.resize(t - 1);

        for (int j = 0; j < t - 1; j++) {
            z->keys[j] = y->keys[j + t];
            z->userData[j] = y->userData[j + t];
        }

        if (!y->leaf) {
            z->children.resize(t);
            for (int j = 0; j < t; j++) {
                z->children[j] = y->children[j + t];
            }
        }

        y->keys.resize(t - 1);
        y->userData.resize(t - 1);

        children.resize(keys.size() + 1);
        for (int j = static_cast<int>(keys.size()); j >= i + 1; j--) {
            children[j + 1] = children[j];
        }

        children[i + 1] = z;

        keys.resize(keys.size() + 1);
        userData.resize(userData.size() + 1);
        for (int j = static_cast<int>(keys.size()) - 1; j >= i; j--) {
            keys[j + 1] = keys[j];
            userData[j + 1] = userData[j];
        }

        keys[i] = y->keys[t - 1];
        userData[i] = y->userData[t - 1];
}

void BTreeNode::remove(uint32_t k) {
    int idx = 0;
    while (idx < keys.size() && keys[idx] < k) {
        idx++;
    }
    if (idx < keys.size() && keys[idx] == k) {
        if (leaf) {
            removeLeaf(idx);
        } else {
            removeNoLeaf(idx);
        }
    } else {
        if (leaf) {
            cout << "The key " << k << " is not present in the tree\n";
            return;
        }
        bool flag = (idx == keys.size());
        if (children[idx]->keys.size() < t) {
            fill(idx);
        }
        if (flag && idx > keys.size()) {
            children[idx - 1]->remove(k);
        } else {
            children[idx]->remove(k);
        }
    }
}

void BTreeNode::removeLeaf(int idx) {
    for (int i = idx + 1; i < keys.size(); i++) {
        keys[i - 1] = keys[i];
        userData[i - 1] = userData[i];
    }
    keys.resize(keys.size() - 1);
    userData.resize(userData.size() - 1);
}

void BTreeNode::removeNoLeaf(int idx) {
    uint32_t k = keys[idx];
    if (children[idx]->keys.size() >= t) {
        uint32_t pred = getPred(idx);
        keys[idx] = pred;
        children[idx]->remove(pred);
    } else if (children[idx + 1]->keys.size() >= t) {
        uint32_t succ = getSucc(idx);
        keys[idx] = succ;
        children[idx + 1]->remove(succ);
    } else {
        merge(idx);
        children[idx]->remove(k);
    }
}

uint32_t BTreeNode::getPred(int idx) {
    BTreeNode* cur = children[idx];
    while (!cur->leaf) {
        cur = cur->children[cur->keys.size()];
    }
    return cur->keys[cur->keys.size() - 1];
}

uint32_t BTreeNode::getSucc(int idx) {
    BTreeNode* cur = children[idx + 1];
    while (!cur->leaf) {
        cur = cur->children[0];
    }
    return cur->keys[0];
}

void BTreeNode::fill(int idx) {
    if (idx != 0 && children[idx - 1]->keys.size() >= t) {
        borrowPrev(idx);
    } else if (idx != keys.size() && children[idx + 1]->keys.size() >= t) {
        borrowNext(idx);
    } else {
        if (idx != keys.size()) {
            merge(idx);
        } else {
            merge(idx - 1);
        }
    }
}

void BTreeNode::borrowPrev(int idx) {
    
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx - 1];
    for ( int i = static_cast<int>(child->keys.size()) - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
        child->userData[i + 1] = child->userData[i];
    }
    if (!child->leaf) {
        for (int i = static_cast<int>(child->keys.size()); i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }
    child->keys[0] = keys[idx - 1];
    child->userData[0] = userData[idx - 1];
    if (!child->leaf) {
        child->children[0] = sibling->children[sibling->keys.size()];
    }
    keys[idx - 1] = sibling->keys[sibling->keys.size() - 1];
    userData[idx - 1] = sibling->userData[sibling->keys.size() - 1];
    child->keys.resize(child->keys.size() + 1);
    sibling->keys.resize(sibling->keys.size() - 1);
}

void BTreeNode::borrowNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys[child->keys.size()] = keys[idx];
    child->userData[child->keys.size()] = userData[idx];
    if (!child->leaf) {
        child->children[child->keys.size() + 1] = sibling ->children[0];}
    keys[idx] = sibling ->keys[0];
    userData[idx] = sibling -> userData[0];
    for(int i = 1;i < sibling->keys.size();i++){
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->userData[i - 1] = sibling->userData[i];
    }
    if (!sibling->leaf) {
    for (int i = 1; i <= sibling->keys.size(); i++) {
    sibling->children[i - 1] = sibling->children[i];
        }
    }
    child->keys.resize(child->keys.size() + 1);
    sibling->keys.resize(sibling->keys.size() - 1);
    
}

void BTreeNode::merge(int idx) {
BTreeNode* child = children[idx];
BTreeNode* sibling = children[idx + 1];
child->keys[t - 1] = keys[idx];
child->userData[t - 1] = userData[idx];
for (int i = 0; i < sibling->keys.size(); i++) {
child->keys[i + t] = sibling->keys[i];
child->userData[i + t] = sibling->userData[i];
}
if (!child->leaf) {
for (int i = 0; i <= sibling->keys.size(); i++) {
child->children[i + t] = sibling->children[i];
}
}
for (int i = idx + 1; i < keys.size(); i++) {
keys[i - 1] = keys[i];
userData[i - 1] = userData[i];
}
for (int i = idx + 2; i <= keys.size(); i++) {
children[i - 1] = children[i];
}
child->keys.resize(child->keys.size() + sibling->keys.size() + 1);
keys.resize(keys.size() - 1);
delete sibling;
}

Btree::Btree(int t) : t(t), root(nullptr) {}

Btree::~Btree() {
    delete root;
}

void Btree::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}

UserData* Btree::search(uint32_t k) {
    return (root == nullptr) ? nullptr : root->search(k);
}

void Btree::insert(uint32_t k, const UserData& data) {
    if (root == nullptr) {
            root = new BTreeNode(t, true);
            root->keys.push_back(k);
            root->userData.push_back(data);
        } else {
            if (root->keys.size() == 2 * t - 1) {
                BTreeNode* s = new BTreeNode(t, false);
                s->children.push_back(root);
                s->splitChild(0, root);

                int i = 0;
                if (s->keys[0] < k) {
                    i++;
                }
                s->children[i]->insertNonFull(k, data);

                root = s;
            } else {
                root->insertNonFull(k, data);
            }
        }
}

bool Btree::remove(uint32_t k) {
    if (!root) {
        cout <<"El arbol esta lleno"<<endl;
        return false;
    }
    root->remove(k);
    if (root->keys.size() == 0) {
        BTreeNode* tmp = root;
        if (root->leaf) {
            root = nullptr;
        } else {
            root = root->children[0];
        }
        delete tmp;
    }
return true;
}
