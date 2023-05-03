//
// Created by hola on 2/5/2023.
//

#ifndef PROJECTBD_HASHINDEX_H
#define PROJECTBD_HASHINDEX_H

#include <iostream>
#include <functional>
#include <bitset>
#include "SMH.h"
#include "Node.h"
#include "Bucket.h"
using namespace std;

template <class Register, class Key, class Hash = hash<Key> >
class HashIndex{
private:
    int MAXHEIGHT;
    typedef  bitset<50> myBitset;
    SMH<Bucket<Register, Key>> bucketFile;
    SMH<HashNode> indexFile;
    Hash myHash;
public:
    HashIndex()= default;
    HashIndex(const string& indexFilePath, const string& bucketFilePath, int MAXHEIGHT_ = 3){
        this->bucketFile.setPath(bucketFilePath);
        this->indexFile.setPath(indexFilePath);
        if(MAXHEIGHT_ > 50 - 1) {
            throw invalid_argument("Invalid MAXHEIGHT, must to be < " + to_string(50)); }
        this->MAXHEIGHT = MAXHEIGHT_;
    }
    void initializeFirstValues(){
        HashNode root(1, 2), v1(0), v2(1);
        Bucket<Register, Key> b1, b2;
        this->bucketFile.add(b1);
        this->bucketFile.add(b2);
        this->indexFile.add(root);
        this->indexFile.add(v1);
        this->indexFile.add(v2);
    }

    void insertAll(vector<Register> records){
        if(bucketFile.getNumberOfRecords() == 0)
            initializeFirstValues();

        for(auto& record : records){
            insert(record);
        }
    }

    int searchNode(int nodePosition, myBitset key, int& height){
        HashNode node = indexFile.readRecord(nodePosition);
        if(node.isLeaf)
            return nodePosition;
        else{
            if(key[height] == 0) return searchNode(node.left, key, ++height);
            else return searchNode(node.right, key, ++height);
        }
    }
    vector<Register> search(Key searchKey){
        myBitset hashKey = myHash(searchKey);
        int height = 0;
        int currentNodePosition = searchNode(0, hashKey, height);
        HashNode currentNode = indexFile.readRecord(currentNodePosition);
        int currentBucketPosition = currentNode.getBucketPosition();
        vector<Register> output;
        while(currentBucketPosition != -1){
            Bucket bucket = bucketFile.readRecord(currentBucketPosition);
            for(auto& r: bucket.getRecords()){
                if( strcmp(r.getPrimaryKey(),searchKey)) // AQUIIII
                    output.push_back(r);
            }
            currentBucketPosition = bucket.nextDel;
        }
        return output;
    }

    vector<Register> searchInRange(Key beginKey, Key endKey){
        int bucketsNumber = bucketFile.getNumberOfRecords();
        vector<Register> output;
        for(int i = 0; i < bucketsNumber; ++i){
            Bucket bucket = bucketFile.readRecord(i);
            for(auto& r : bucket.getRecords()){
                if(strcmp(r->getID(),endKey) <= 0 && strcmp(r->getID(),beginKey) >= 0)
                    output.push_back(r);
            }
        }
        sort(output.begin(), output.end());
        return output;
    }

    void insert(Register record){
        if(bucketFile.getNumberOfRecords() == 0)
            initializeFirstValues();
        int height = 0;
        myBitset hashKey = myHash(record.getPrimaryKey());
        int currentNodePosition = searchNode(0, hashKey, height);
        HashNode currentNode = indexFile.readRecord(currentNodePosition);
        int currentBucketPosition = currentNode.getBucketPosition();
        Bucket bucket = bucketFile.readRecord(currentBucketPosition);
        if(!bucket.isFull()){
            bucket.add(record);
            bucket.sortBucket();
            bucketFile.writeRecord(currentBucketPosition, bucket);
        }else
            split(record, height, currentNodePosition, currentNode, currentBucketPosition, bucket);
    }

    void split(Register& record, int height, int currentNodePosition, HashNode& currentNode, int currentBucketPosition,
    Bucket<Register, Key>& bucket) {
        if(height == MAXHEIGHT){
            Bucket<Register, Key> bucketToLink;
            bucketToLink.add(record);
            bucketToLink.nextDel = currentBucketPosition; // .next
            currentNode.bucketPosition = bucketFile.add(bucketToLink);
            indexFile.writeRecord(currentNodePosition, currentNode);
            return;
        }
        vector<Register> records = bucket.getRecords();
        records.push_back(record);
        Bucket<Register, Key> bucket1, bucket2;
        bool splitAgain = false;
        bool splitToLeft = false;
        for(auto& r : records){
            myBitset hashKey = myHash(r.getPrimaryKey());
            if(hashKey[height] == 0){
                if(!bucket1.isFull()) bucket1.add(r);
                else{
                    splitAgain = true;
                    splitToLeft = true;
                }
            }
            else{
                if(!bucket2.isFull()) bucket2.add(r);
                else splitAgain = true;
            }
        }
        bucketFile.deleteRecord(currentBucketPosition);
        bucket1.sortBucket();
        bucket2.sortBucket();
        int bucketPosition1 = bucketFile.add(bucket1);
        int bucketPosition2 = bucketFile.add(bucket2);
        HashNode leftNode(bucketPosition1), rightNode(bucketPosition2);
        currentNode.left = indexFile.add(leftNode);
        currentNode.right = indexFile.add(rightNode);
        currentNode.isLeaf = false;
        indexFile.writeRecord(currentNodePosition, currentNode);
        if(splitAgain){
            int splitNodePosition, splitBucketPosition;
            HashNode splitNode;
            if(splitToLeft) {
                splitNodePosition = currentNode.left;
                splitNode = indexFile.readRecord(splitNodePosition);
                split(record, height+1, splitNodePosition, splitNode, bucketPosition1, bucket1);
            }else{
                splitNodePosition = currentNode.right;
                splitNode = indexFile.readRecord(splitNodePosition);
                split(record, height+1, splitNodePosition, splitNode, bucketPosition2, bucket2);
            }
        }
    }
    void remove(Key key){
        int height = 0;
        removeUtil(0, key, myHash(key), height);
    }
    void removeUtil(int nodePosition, Key key, myBitset hashKey, int& height) {
        HashNode currentNode = indexFile.readRecord(nodePosition);
        if (currentNode.isLeaf) {
            deleteRecordInBucket(key, currentNode, nodePosition);
            return;
        }
        if (hashKey[height] == 0) removeUtil(currentNode.left, key, hashKey, ++height);
        else removeUtil(currentNode.right, key, hashKey, ++height);

        if(nodePosition == 0) return;
        HashNode left = indexFile.readRecord(currentNode.left);
        HashNode right = indexFile.readRecord(currentNode.right);

        if (left.isLeaf && right.isLeaf) {
            Bucket bucket1 = bucketFile.readRecord(left.bucketPosition);
            Bucket bucket2 = bucketFile.readRecord(right.bucketPosition);
            if (bucket1.isEmpty() && bucket2.isEmpty()) {
                bucketFile.deleteRecord(left.bucketPosition);
                bucketFile.deleteRecord(right.bucketPosition);
                indexFile.deleteRecord(currentNode.left);
                indexFile.deleteRecord(currentNode.right);
                Bucket<Register, Key> newBucket;
                currentNode.isLeaf = true;
                currentNode.bucketPosition = bucketFile.add(newBucket);
                indexFile.writeRecord(nodePosition, currentNode);
            }
        }
    }

    void deleteRecordInBucket(Key key, HashNode &currentNode, int nodePosition) {
        int currentBucketPosition = currentNode.bucketPosition;
        Bucket bucket = bucketFile.readRecord(currentBucketPosition);
        auto differentRecords = bucket.getAllDifferentRecords(key);
        bucket.size = 0 ;
        for(auto &i : differentRecords){
            bucket.keys[bucket.size] = i;
            bucket.size++;
        }
        bucketFile.writeRecord(currentBucketPosition, bucket);
        if(bucket.isEmpty() && bucket.next != -1){
            currentNode.bucketPosition = bucket.next;
            bucketFile.deleteRecord(currentBucketPosition);
            indexFile.writeRecord(nodePosition, currentNode);
            deleteRecordInBucket(key, currentNode, nodePosition);
        }
    }

    int getMaxHeight(){
        return MAXHEIGHT;
    }

};

#endif //PROJECTBD_HASHINDEX_H
