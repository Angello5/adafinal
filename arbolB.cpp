//
//  arbolB.cpp
//  adaxcode
//
//  Created by Angello Llerena on 19/06/24.
//

#include "arbolB.hpp"
#include <iostream>

BTreeNode::BTreeNode(int t, bool leaf) : t(t), leaf(leaf) {
    keys.reserve(2 * t - 1);
    userData.reserve(2 * t - 1);
    children.reserve(2 * t);
}

BTreeNode::~BTreeNode() {
    for (auto child : children) {
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

shared_ptr<UserData> BTreeNode::search(uint32_t k) {
    int i = 0;
    while (i < keys.size() && k > keys[i]) {
        i++;
    }
    if (keys[i] == k) {
        return userData[i];
    }
    if (leaf) {
        return nullptr;
    }
    return children[i]->search(k);
}

void BTreeNode::insertNonFull(uint32_t k, const shared_ptr<UserData>& data) {
    int i = static_cast<int>(keys.size()) - 1;
    if (leaf) {
        keys.push_back(0);
        userData.push_back(nullptr);
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
    auto z = new BTreeNode(y->t, y->leaf);
    z->keys.reserve(t - 1);
    z->userData.reserve(t - 1);
    z->children.reserve(t);
    for (int j = 0; j < t - 1; j++) {
        z->keys.push_back(y->keys[j + t]);
        z->userData.push_back(y->userData[j + t]);
    }
    if (!y->leaf) {
        for (int j = 0; j < t; j++) {
            z->children.push_back(y->children[j + t]);
        }
    }
    y->keys.resize(t - 1);
    y->userData.resize(t - 1);
    children.insert(children.begin() + i + 1, z);
    keys.insert(keys.begin() + i, y->keys[t - 1]);
    userData.insert(userData.begin() + i, y->userData[t - 1]);
}

void BTreeNode::remove(uint32_t k) {
    int idx = 0;
    while (idx < keys.size() && keys[idx] < k) {
        idx++;
    }
    if (idx < keys.size() && keys[idx] == k) {
        if (leaf) {
            removeNoLeaf(idx);
        } else {
            removeNoLeaf(idx);
        }
    } else {
        if (leaf) {
            cout << "El DNI " << k << " no existe en el árbol.\n";
            return;
        }
        bool flag = ((idx == keys.size()) ? true : false);
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
    for (int i = idx + 1; i < keys.size(); ++i) {
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
        userData[idx] = children[idx]->userData[children[idx]->keys.size() - 1];
        children[idx]->remove(pred);
    } else if (children[idx + 1]->keys.size() >= t) {
        uint32_t succ = getSucc(idx);
        keys[idx] = succ;
        userData[idx] = children[idx + 1]->userData[0];
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
    for (int i = static_cast<int>(child->keys.size()) - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
        child->userData[i + 1] = child->userData[i];
    }
    if (!child->leaf) {
        for (int i = static_cast<int>(child->keys.size()); i >= 0; --i) {
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
    sibling->keys.resize(sibling->keys.size() - 1);
    sibling->userData.resize(sibling->userData.size() - 1);
    child->keys.resize(child->keys.size() + 1);
    child->userData.resize(child->userData.size() + 1);
}

void BTreeNode::borrowNext(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys[child->keys.size()] = keys[idx];
    child->userData[child->userData.size()] = userData[idx];
    if (!child->leaf) {
        child->children[child->keys.size() + 1] = sibling->children[0];
    }
    keys[idx] = sibling->keys[0];
    userData[idx] = sibling->userData[0];
    for (int i = 1; i < sibling->keys.size(); ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
        sibling->userData[i - 1] = sibling->userData[i];
    }
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->keys.size(); ++i) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }
    sibling->keys.resize(sibling->keys.size() - 1);
    sibling->userData.resize(sibling->userData.size() - 1);
    child->keys.resize(child->keys.size() + 1);
    child->userData.resize(child->userData.size() + 1);
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = children[idx];
    BTreeNode* sibling = children[idx + 1];
    child->keys[t - 1] = keys[idx];
    child->userData[t - 1] = userData[idx];
    for (int i = 0; i < sibling->keys.size(); ++i) {
        child->keys[i + t] = sibling->keys[i];
        child->userData[i + t] = sibling->userData[i];
    }
    if (!child->leaf) {
        for (int i = 0; i <= sibling->keys.size(); ++i) {
            child->children[i + t] = sibling->children[i];
        }
    }
    for (int i = idx + 1; i < keys.size(); ++i) {
        keys[i - 1] = keys[i];
        userData[i - 1] = userData[i];
    }
    for (int i = idx + 2; i <= keys.size(); ++i) {
        children[i - 1] = children[i];
    }
    keys.resize(keys.size() - 1);
    userData.resize(userData.size() - 1);
    children.resize(children.size() - 1);
    delete sibling;
}

Btree::Btree(int t) : root(nullptr), t(t) { }

Btree::~Btree() {
    delete root;
}

void Btree::traverse() {
    if (root != nullptr) {
        root->traverse();
    }
}

shared_ptr<UserData> Btree::search(uint32_t k) {
    return (root == nullptr) ? nullptr : root->search(k);
}

void Btree::insert(uint32_t k, const shared_ptr<UserData>& data) {
    if (root == nullptr) {
        root = new BTreeNode(t, true);
        root->keys.push_back(k);
        root->userData.push_back(data);
    } else {
        if (root->keys.size() == 2 * t - 1) {
            auto s = new BTreeNode(t, false);
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
    if (root == nullptr) {
        cout << "El árbol está vacío.\n";
        return false; // Retorna falso si el árbol está vacío
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
    return true; // Retorna verdadero si se realiza la operación, ajustar según la lógica específica de eliminación
}


