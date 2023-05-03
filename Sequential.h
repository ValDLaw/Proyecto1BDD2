#ifndef PROYECTO1BDD2_SEQ_H
#define PROYECTO1BDD2_SEQ_H
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>
#include "./Datasets/Record.h"


using namespace std;
const int K = 4;
//Dado que rebuild es O(n)
//Para version final, conviene hacer tests con K = 100, 500, 1000, 2000, 5000, 10 000
//dado que nuestra data es de alrededor 100 000 registros para Payments y 30 000 para Products
//Siempre haciendo que k sea <= log(n)

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

    void rebuild(){ //auxCount = 0; deletedCount = 0
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        ifstream data(datafile, ios::in | ios::binary);
        fstream newData("../datanew.dat", ios::in | ios::out | ios::binary | ios::app);

        //Header
        SequentialBlock current;
        SequentialBlock block;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock));

        long pos_block = 0;
        char current_file = 'D';

        data.seekg(0, ios::beg);
        data.read((char*)&block, sizeof(SequentialBlock));
        block.next_file = current_file;
        block.next = (pos_block+1)*sizeof(SequentialBlock);

        //Escribimos header
        newData.seekg(0, ios::beg);
        newData.write((char*)&block, sizeof(SequentialBlock));
        while (current.next != -1){
            //Si es A o D, vas al nuevo bloque, lo lees,
            //te ubicas en newData y lo escribes
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&block, sizeof(SequentialBlock));
            } else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&block, sizeof(SequentialBlock));
            }

            pos_block = pos_block + 1;
            current = block;

            block.next_file = 'D';
            if (block.next != -1){
                block.next = (pos_block+1)*sizeof(SequentialBlock);
            }
            newData.seekg(pos_block*sizeof(SequentialBlock), ios::beg);
            newData.write((char*)&block, sizeof(SequentialBlock));
        }

        this->auxCount = 0;
        this->deletedCount = 0;

        data.close();
        aux.close();

        //Con trunc se borra todo lo del file
        fstream auxNew(auxfile, ios::in | ios::out | ios::binary | ios::trunc);
        auxNew.close();

        //Borrar oldFile
        std::remove(datafile.c_str());

        //Renombrar newFile
        newData.close();
        std::rename("../datanew.dat", datafile.c_str());

    };

public:
    SequentialFile(string d, string a){
        datafile = move(d);
        auxfile = move(a);
        fstream data(datafile, ios::app);
        data.seekg(0, ios::end);
        if (data.tellg() == 0){ //if file empty
            SequentialBlock header;
            header.next = -1;
            header.next_file = 'D';
            header.record = Registro();
            data.write((char*)&header, sizeof(SequentialBlock));
            fstream aux(auxfile, ios::app);
            aux.close();
        }
        data.close();
    }

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
        if (this->auxCount == K){
            rebuild();
        };

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
            auxCount++;
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
        }
        data.close();
        aux.close();

        return true;
    }

    bool remove(T key){
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

            if (next.record.key == key){//Si se encuentra el key
                current.next = next.next;
                current.next_file = next.next_file;
                if (current_file == 'D'){
                    data.seekg(current_pos, ios::beg);
                    data.write((char*)&current, sizeof(SequentialBlock));
                }
                else if (current_file == 'A'){
                    aux.seekg(current_pos, ios::beg);
                    aux.write((char*)&current, sizeof(SequentialBlock));
                }
                data.close();
                aux.close();
                deletedCount++;
                return true; //Key eliminada
            }
            else if (next.record.key > key){//si el siguiente es mayor, no existe el key
                return false;
            }
            else if (next.record.key < key){//si el siguiente es menor, avanzamos
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
        }

        if (this->deletedCount == K){
            rebuild();
        }
    }

    vector<Registro> search(T key){
        vector<Registro> res;
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);
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

            if (next.record.key == key){//si se encuentra el key
                res.push_back(next.record);
            }
            else if (next.record.key > key){//si el siguiente es mayor, ya no se va a encontrar el key
                data.close();
                aux.close();
                break;
            }
            current = next; //si es menor sigue
        };
        return res;
    };

    vector<Registro> rangeSearch(T begin_key, T end_key){
        vector<Registro> res;
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);
        SequentialBlock current;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock)); //current = header

        SequentialBlock next;
        while(current.next != -1){
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&next, sizeof(SequentialBlock));
            }else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&next, sizeof(SequentialBlock));
            };

            if (next.record.key >= begin_key and next.record.key <= end_key){//Si se encuentra el key
                res.push_back(next.record);
            }else if (next.record.key > end_key){//si el siguiente es mayor, ya no se va a encontrar el key
                data.close();
                aux.close();
                break;
            };
            current = next; //si es menor sigue
        };
        return res;
    };
};

#endif //PROYECTO1BDD2_SEQ_H