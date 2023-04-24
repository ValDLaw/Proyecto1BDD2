//
// Created by ValDLaw on 4/20/23.
//

#include "Records/Product.h"
#include "Records/Payment.h"
#include <vector>
#include <unordered_map>

using namespace std;

#ifndef PROYECTO1BDD2_EXTENDIBLEHASH_H
#define PROYECTO1BDD2_EXTENDIBLEHASH_H

string decToBin(short dec) {
    string bin = "";

    while (dec != 0) {
        bin = to_string(dec % 2) + bin;
        dec /= 2;
    }

    return bin;
}

struct Bucket{ // corresponde a una página (bloque de memoria) en donde se almacena uno o mas registros
    // short id;
    short size = 0;
    short d = 1; // profundidad local
    vector<string> keys;
    short next = -1;
};

struct HashIndex { // espacio de valores de la función hash y sus direcciones a los buckets de datos.
    unordered_map<string, short> reference; // alamacenamos el string de inicio con el id del bucket
public:
    HashIndex(){
        reference["0"] = 0; reference["10"] = 0; reference["110"] = 0; reference["100"] = 0;
        reference["1"] = 1; reference["11"] = 1; reference["101"] = 1; reference["111"] = 1;
    }
};

class ExtendibleHashing {
    short D = 3; // profundidad global
    int size = 1; // indica el último índice ocupado en el vector de Buckets
    HashIndex hashIndex;
    string filename;
    vector<Bucket> buckets;
public:
    ExtendibleHashing(string f){
        this->filename = f;
        fstream file;
        file.open(filename,ios::in | ios::out);
        if(file.is_open()){
            hashIndex = HashIndex(); // no sé si es necesario dejarlo
            Bucket b1, b2; // buckets iniciales 0 y 1
            buckets.push_back(b1); buckets.push_back(b2);
            // chequeamos si hay registros
            file.seekg(0, ios::end); // nos posicionamos al final
            int size_file = file.tellg();
            size_file = size_file / ((int)sizeof(Product));
            if(size_file != 0){
                Product temp;
                file.seekg(0, ios::beg); // regresamos al inicio
                while(file >> temp){
                    insert(temp); // in progress
                }
                file.close();
            }
        }
        else{
            cout << "No se pudo abrir el archivo." << endl;
        }
    }

    short hashFunction(string key){
        size_t hashValue = hash<string>{}(key);
        return hashValue%(2^D);
    }

    void insert(Product p){
        ofstream file;
        file.open(filename,ios::app);
        if(file.is_open()){
            file << p;
            file.close();
        }
        else{
            cout << "No se pudo abrir el archivo." << endl;
        }
        string key = p.product_id;
        short hashValue = hashFunction(key);
        string binCode = decToBin(hashValue);
        short bucketID = hashIndex.reference[binCode]; // obteniendo el bucket que corresponde
        if(buckets[bucketID].size <= D){
            buckets[bucketID].keys.push_back(key);
            buckets[bucketID].size++;
        }
        else if(buckets[bucketID].size == D){
            Bucket b;
            buckets.push_back(b);
            string last_n_chars = binCode.substr(4 - buckets[bucketID].d, buckets[bucketID].d);
            // planeando LÓGICA: SUFRIENDOOO
        }
    }
};

#endif //PROYECTO1BDD2_EXTENDIBLEHASH_H
