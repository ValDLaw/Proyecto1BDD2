//
// Created by hola on 2/5/2023.
//

#ifndef PROJECTBD_NODE_H
#define PROJECTBD_NODE_H

#include <functional>

struct HashNode{
    int left;
    int right;
    int bucketPosition;
    bool isLeaf;
    int nextDel;

    HashNode() = default;;
    HashNode(int left, int right){
        this->left = left;
        this->right = right;
        this->isLeaf = false;
        this->nextDel = 0;
    }
    HashNode(int bucketPosition){
        this->bucketPosition = bucketPosition;
        this->isLeaf = true;
        this->nextDel = 0;
    }

    int getBucketPosition() const {
        if(isLeaf)
            return bucketPosition;
//        else throw invalid_argument("No es un nodo hoja");
        return -1;
    }

    void setBucketPosition(int  position){
        if(isLeaf)
            this->bucketPosition = position;
//        else throw invalid_argument("No es un nodo hoja");
        return;
    }
};


#endif //PROJECTBD_NODE_H
