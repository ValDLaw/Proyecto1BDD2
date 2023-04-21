//
// Created by ValDLaw on 4/20/23.
//

#include "Records/Product.h"
#include "Records/Payment.h"
#include <vector>

using namespace std;

#ifndef PROYECTO1BDD2_EXTENDIBLEHASH_H
#define PROYECTO1BDD2_EXTENDIBLEHASH_H

struct Bucket{ // corresponde a una página (bloque de memoria) en donde se almacena uno o mas registros
    short id;
    short d; // profundidad local
    vector<long> keys;
};

struct HashIndex { // espacio de valores de la función hash y sus direcciones a los buckets de datos.
    short D = 3; // profundidad global

public:
    HashIndex() = default;
    short hashFunction(string key){
        size_t hash_value = hash<string>{}(key);
        return hash_value%(2^D);
    }
};

class ExtendibleHashing {
    HashIndex hashIndex;
    string filename;
public:
    // in process
    ExtendibleHashing(string f){
        this->filename = f;
    }

    bool fileIsEmpty(string f){
        ifstream file(f);
        if (file.is_open() && file.peek() != ifstream::traits_type::eof()) {
            file.close(); // The text file has records.
            return false;
        }
        file.close(); // The file is empty
        return true;
    }
};

#endif //PROYECTO1BDD2_EXTENDIBLEHASH_H
