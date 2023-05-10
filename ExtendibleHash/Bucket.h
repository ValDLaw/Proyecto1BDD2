//
// Created by hola on 2/5/2023.
//

#ifndef PROJECTBD_BUCKET_H
#define PROJECTBD_BUCKET_H
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

template <class Record, class Key>
class Bucket{
private:
    Record records[4];
    int nextBucket;
    int count;
    int D = 4;
public:
    int nextDeleted;
    Bucket(): nextBucket(-1), nextDeleted(0), count(0) {}

    bool isFull(){
        return count == D;
    }

    void setNextBucket(int nextPosition){
        nextBucket = nextPosition;
    }

    int getNextBucket() const {
        return nextBucket;
    }

    void add(Record record){
        if(count < D) {
            records[count] = record;
            ++count;
        }
    }

    void sortBucket(){
        sort(records, records+count, compareRecords);
    }

    static bool compareRecords(Record& a, Record& b){
        return a.getID() < b.getID();
    }

    vector<Record> getRecords(){
        return vector<Record>(records, records+count);
    }

    void setRecords(vector<Record> newRecords){
        count = 0;
        for(auto& record : newRecords){
            records[count] = record;
            count++;
        }
    }

    bool empty(){
        return count == 0;
    }

    vector<Record> getAllDifferentRecords(Key key){
        vector<Record> output;
        for(auto& record : this->getRecords()){
            if(!record.equalToKey(key))
                output.push_back(record);
        }
        return output;
    }
};


#endif //PROJECTBD_BUCKET_H
