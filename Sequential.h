#ifndef PROYECTO1BDD2_SEQ_H
#define PROYECTO1BDD2_SEQ_H
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;
const int K = 3;

struct Registro{
    int key;

    Registro() = default;

    Registro(int key){
        this->key = key;
    };
};

template <typename T>
class SequentialFile{
private:
    struct SequentialBlock{
        long next; //pos*sizeof(SequentialBlock) = pos_física del siguiente
        char next_file; //D or A
        Registro record;
    };
    string datafile;
    string auxfile;
    int accessMemSec = 0;
    int auxCount = 0;
    int deletedCount = 0;

public:
    SequentialFile(string d, string a){
        datafile = move(d);
        auxfile = move(a);

        SequentialBlock header;
        header.next = -1;
        header.next_file = 'D';
        header.record = Registro();
        fstream aux(auxfile, ios::app);
        aux.close();
        fstream data(datafile, ios::app);
        data.write((char*)&header, sizeof(SequentialBlock));
        data.close();
    }

    vector<Registro> search(T key);
    vector<Registro> rangeSearch(T begin_key, T end_key);

    void rebuild(); //auxCount = 0; deletedCount = 0;


    void read(){
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);

        SequentialBlock block;
        data.seekg(0, ios::beg);
        data.read((char*)&block, sizeof(SequentialBlock));
        int pos_actual = 0;
        char current_file = 'D';
        int pos_block;

        cout << "POS|" << "KEY|" << "NEXT" << endl;

        while (block.next != -1){
            if (current_file == 'D'){
                data.seekg(pos_actual);
                pos_block = data.tellg()/sizeof(SequentialBlock);
            } else{
                aux.seekg(pos_actual);
                pos_block = aux.tellg()/sizeof(SequentialBlock);
            }

            cout << pos_block << current_file << " | ";
            cout << block.record.key << " | ";
            cout << block.next/sizeof(SequentialBlock) << block.next_file << endl;

            if (block.next_file == 'A'){
                pos_actual = block.next;
                aux.seekg(block.next, ios::beg);
                aux.read((char*)&block, sizeof(SequentialBlock));
                current_file = 'A';
            } else {
                pos_actual = block.next;
                data.seekg(block.next, ios::beg);
                data.read((char*)&block, sizeof(SequentialBlock));
                current_file = 'D';
            }
        }

        if (current_file == 'D'){
            data.seekg(pos_actual);
            pos_block = data.tellg()/sizeof(SequentialBlock);
        } else{
            aux.seekg(pos_actual);
            pos_block = aux.tellg()/sizeof(SequentialBlock);
        }

        cout << pos_block << current_file << " | ";
        cout << block.record.key << " | ";
        cout << block.next << block.next_file << endl;

        aux.close();
        data.close();
    };

    bool add(Registro registro){
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary | ios::ate);
        long current_pos = 0;
        char current_file = 'D';
        SequentialBlock current;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock)); //current = header

        SequentialBlock next;
        while(current.next != -1){
            cout << current_pos/sizeof(SequentialBlock) << current_file << " ";
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&next, sizeof(SequentialBlock));
            }
            else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&next, sizeof(SequentialBlock));
            }

            cout << current.record.key << " " << (current.next)/sizeof(SequentialBlock) << current.next_file << endl;

            if (next.record.key == registro.key){//Si se encuentra el key
                data.close();
                aux.close();
                return false; //Ya existe, no se puede agregar
            }
            else if (next.record.key > registro.key){//si el siguiente es mayor, stop
                break;
            }
            else if (next.record.key < registro.key){//si el siguiente es menor, avanzamos
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
        }

        //"reemplazamos" el next con el nuevo bloque
        SequentialBlock block;
        long pos;
        block.next = current.next;
        block.next_file = current.next_file; //reemplazamos al next
        block.record = registro;

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
            pos = aux.tellg();
            aux.write((char*)&block, sizeof(SequentialBlock));
            current.next = pos;
            current.next_file = 'A';
        }

        if (current_file == 'D'){
            data.seekg(current_pos, ios::beg);
            data.write((char*)&current, sizeof(SequentialBlock));
        }
        else if (current_file == 'A'){
            aux.seekg(current_pos, ios::beg);
            aux.write((char*)&current, sizeof(SequentialBlock));
            auxCount++;
        }
        data.close();
        aux.close();

        /*if (auxCount == K){
            rebuild();
        }*/
        return true;
    }

    bool remove(T key);
};

#endif //PROYECTO1BDD2_SEQ_H