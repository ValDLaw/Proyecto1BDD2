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
        long next; //pos*sizeof(SequentialBlock) = pos_física
        char next_file; //D or A
        Registro record;
    };
    string datafile;    
    string auxfile;
    int accessMemSec;
    int deletedCount;

    SequentialFile(){
        SequentialBlock header;
        SequentialBlock header.next = -1;
        SequentialBlock header.next_file = 'D';
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
        SequentialBlock temp;
        long pos = 0;
        data.seekg(0, ios::beg);
        data.read((char*)&temp, sizeof(SequentialBlock));
        
        //Buscar el bloque donde se debe insertar
        SequentialBlock prev = temp;
        while(temp.next != -1){
            //Leer el siguiente bloque
            if (temp.next_file == 'D'){
                data.seekg(temp.next, ios::beg);
                data.read((char*)&temp, sizeof(SequentialBlock));
            }
            else if (temp.next_file == 'A'){
                aux.seekg(temp.next, ios::beg);
                aux.read((char*)&temp, sizeof(SequentialBlock));
            }

            if (temp.record.key == registro.key){ //si se encuentra, add = false
                data.close();
                aux.close();
                return false;
            }
            else if (temp.record.key > registro.key){ //si el key es mayor, insertar en el bloque anterior
                temp = prev;
                break;
            }
            else{ //ir al siguiente bloque
                prev = temp;
                temp = temp.next;
            }
        }

        //Write block
        SequentialBlock block;
        block->next = temp->next;
        block->next_file = temp->next_file;
        block->record = registro;
        block->deleted = false;
       
        //En caso que sea el último key de todos los files
        if (temp->next_file == 'D' and temp->next == -1){
            data.seekg(0, ios::end);
            temp->next = data.tellg();
            temp->next_file = 'D'
            data.write((char*)&block, sizeof(SequentialBlock));
        }
        //Cualquier otra posición
        else if{
            aux.seekg(temp->next, ios::end);
            temp->next = data.tellg();
            temp->next_file = 'A'
            aux.write((char*)&block, sizeof(SequentialBlock));
        } 
    }
    bool remove(T key);
};