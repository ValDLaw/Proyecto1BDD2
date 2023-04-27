#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include "Records/Product.h"
#include "Records/Payment.h"

using namespace std;
const int K = 3;

template <class Registro>
class SequentialFile{
private:
    struct SequentialBlock{
        long next; //pos*sizeof(SequentialBlock) = pos_física del siguiente
        char next_file; //D or A
        Registro record;
    };
    string datafile;    
    string auxfile;
    int accessMemSec;
    int deletedCount;

    SequentialFile(){
        SequentialBlock header;
        header.next = -1;
        header.next_file = 'D';
        header.record = Registro();
        fstream data(datafile, ios::in | ios::out | ios::binary);
        data.write((char*)&header, sizeof(SequentialBlock));
        data.close();
    }

    vector<Registro> search(T key);
    vector<Registro> rangeSearch(T begin-key, T end-key);
    
    void rebuild();

    bool add(Registro registro){
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);
        long current_pos = 0;
        char current_file = 'D';
        SequentialBlock current;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock)); //current = header
        
        SequentialBlock next;
        while(current.next != -1){
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&next, sizeof(SequentialBlock));
            }
            else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&next, sizeof(SequentialBlock));
            }

            if (next.record.key == registro.key){
                data.close();
                aux.close();
                return false; //Ya existe, no se puede agregar
            }
            else if (next.record.key > registro.key){
                break;
            }
            else if (next.record.key < registro.key){
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
            //Si el siguiente es menor, sólo continúa
        }

        //"reemplazamos" el next con el nuevo bloque
        SequentialBlock block;
        long pos;
        block.next = current.next; 
        block.next_file = current.next_file; //reemplazamos al next
        block.record = registro;
        block.deleted = false;
        
        //if current.next = -1 -> último
        if (current.next == -1){
            data.seekg(0, ios::end);
            pos = data.tellg();
            data.write((char*)&block, sizeof(SequentialBlock));
            current.next = pos;
            current.next_file = 'D';
        }
        else{
            aux.seekg(0, ios::end);
            pos = data.tellg();
            aux.write((char*)&block, sizeof(SequentialBlock));
            current.next = pos;
            current.next_file = 'A';
        }

        if (current_file == 'D'){
            data.seekg(current_pos, ios::begin);
            data.write((char*)&current, sizeof(SequentialBlock));
        }
        else if (current_file == 'A'){
            aux.seekg(current_pos, ios::begin);
            aux.write((char*)&current, sizeof(SequentialBlock));
        }

        data.close();
        aux.close();
    }
    
    bool remove(T key);
};