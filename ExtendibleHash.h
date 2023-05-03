//
// Created by ValDLaw on 4/20/23.
//

#include "./Datasets/Record.h"
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
    // factor de bloque 4 o 5, pasarlo como parámetro
    short size = 0;
    short d = 1; // profundidad local
    vector<string> keys;
    short next = -1;
};

struct HashIndex { // espacio de valores de la función hash y sus direcciones a los buckets de datos.
    unordered_map<string, short> reference; // alamacenamos el string de inicio con el id del bucket
    // buscar nueva forma de guardarlo en forma secundaria
    // el indice debe etrar en la ram, de la frma mas eficientemente posibe
    // la cadeba binaria se saca del hashcode
    // de izqueirda a derecha
    // usar size_t para la funcion hashing
public:
    HashIndex(){
        reference["0"] = 0; reference["10"] = 0; reference["110"] = 0; reference["100"] = 0;
        reference["1"] = 1; reference["11"] = 1; reference["101"] = 1; reference["111"] = 1;
    }
};

// recomendacion, usar el index.dat  para no perder los cambios
// el nodo interno guarda el bucket, la referencia a las keys
// no crear bucket si es que no tiene elementos, entonces el nodo aputnaría a -1
// la estrategia del RANDOM, cada vez que hay actualizaciones en la RAM, se debe escribir en la meoria secundaria, se envían todas las página del índice

class ExtendibleHashing {
    short D = 8; // profundidad global
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
                /*Record temp("nada");
                file.seekg(0, ios::beg); // regresamos al inicio
                while(temp.read(cin)){
                    insert(temp); // in progress
                }*/
                file.close();
            }
        }
        else{
            cout << "No se pudo abrir el archivo." << endl;
        }
    }

    // crear un extendible hash no agrupado

    int hashFunction(string key){
        size_t hashValue = hash<string>{}(key); // ASCI tmb puede ser
        return hashValue%(2^D); // de frente devuelva el binario en string
    }

    void insert(Record record){
        ofstream file;
        file.open(filename,ios::app);
        if(file.is_open()){
            record.read(cin);
            file.close();
        }
        else{
            cout << "No se pudo abrir el archivo." << endl;
        }
        string key = record.getID();
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
