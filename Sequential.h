#ifndef PROYECTO1BDD2_SEQ_H
#define PROYECTO1BDD2_SEQ_H
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>
#include "Entities/IntProduct.h"
#include "Entities/IntPayment.h"

using namespace std;
const int K = 100;
//Dado que rebuild es O(n)
//Para version final, conviene hacer tests con K = 100, 500, 1000, 2000, 5000, 10 000
//dado que nuestra data es de alrededor 100 000 registros para Payments y 30 000 para Products
//Siempre haciendo que k sea <= log(n)

template <typename Record>
class SequentialFile{
private:
    struct SequentialBlock{
        long next; //pos*sizeof(SequentialBlock) = pos_física del siguiente
        char next_file; //D or A
        Record record;

        SequentialBlock(){
            next = -1;
            next_file = 'D';
        }
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
        accessMemSec++;

        long pos_block = 0;
        char current_file = 'D';

        data.seekg(0, ios::beg);
        data.read((char*)&block, sizeof(SequentialBlock));
        accessMemSec++;
        block.next_file = current_file;
        block.next = (pos_block+1)*sizeof(SequentialBlock);

        //Escribimos header
        newData.seekg(0, ios::beg);
        newData.write((char*)&block, sizeof(SequentialBlock));
        accessMemSec++;
        while (current.next != -1){
            //Si es A o D, vas al nuevo bloque, lo lees,
            //te ubicas en newData y lo escribes
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&block, sizeof(SequentialBlock));
                accessMemSec++;
            } else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&block, sizeof(SequentialBlock));
                accessMemSec++;
            }

            pos_block = pos_block + 1;
            current = block;

            block.next_file = 'D';
            if (block.next != -1){
                block.next = (pos_block+1)*sizeof(SequentialBlock);
            }
            newData.seekg(pos_block*sizeof(SequentialBlock), ios::beg);
            newData.write((char*)&block, sizeof(SequentialBlock));
            accessMemSec++;
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
        datafile = std::move(d);
        auxfile = std::move(a);
        fstream data(datafile, ios::app);
        data.seekg(0, ios::end);
        if (data.tellg() == 0){ //if file empty
            SequentialBlock header;
            //se puede borrar
            header.next = -1;
            header.next_file = 'D';
            header.record = Record();
            data.write((char*)&header, sizeof(SequentialBlock));
            accessMemSec++;
            fstream aux(auxfile, ios::app);
            aux.close();
        }
        data.close();
    }

    int getNumberAccess(){
        return accessMemSec;
    }

    void restartNumberAccess(){
        accessMemSec = 0;
    }

    void read(){
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);

        SequentialBlock block;
        data.seekg(0, ios::beg);
        data.read((char*)&block, sizeof(SequentialBlock));
        accessMemSec++;
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
            cout << block.record.getPrimaryKey() << " | ";
            cout << block.next/sizeof(SequentialBlock) << block.next_file << endl;

            if (block.next_file == 'A'){
                pos_actual = block.next;
                aux.seekg(block.next, ios::beg);
                aux.read((char*)&block, sizeof(SequentialBlock));
                accessMemSec++;
                current_file = 'A';
            } else {
                pos_actual = block.next;
                data.seekg(block.next, ios::beg);
                data.read((char*)&block, sizeof(SequentialBlock));
                accessMemSec++;
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
        cout << block.record.getPrimaryKey() << " | ";
        cout << block.next << block.next_file << endl;

        aux.close();
        data.close();
    };

    bool add(Record registro){
        if (this->auxCount == K){
            rebuild();
        };

        fstream temp_data(datafile, ios::ate | ios::in | ios::out | ios::binary);
        SequentialBlock last;
        long temp_pos = temp_data.tellg();
        temp_pos = temp_pos - sizeof(SequentialBlock);

        temp_data.seekg(temp_pos);
        temp_data.read((char*)&last, sizeof(SequentialBlock));
        accessMemSec++;

        //AÑADIR A DATA
        if (last.record.getPrimaryKey() < registro.getPrimaryKey()) {
            SequentialBlock new_block;
            new_block.next = last.next;
            new_block.next_file = last.next_file;
            new_block.record = registro;
            temp_data.write((char*)&new_block, sizeof(SequentialBlock));
            accessMemSec++;

            last.next = temp_pos + sizeof(SequentialBlock);
            last.next_file = 'D';
            temp_data.seekp(temp_pos);
            temp_data.write((char*)&last, sizeof(SequentialBlock));
            accessMemSec++;
            temp_data.close();
            return true;
        }
        else{
            temp_data.close();
        }

        //AÑADIR A AUX
        fstream data(datafile, ios::in | ios::out | ios::binary);
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        long current_pos = 0;
        char current_file = 'D';

        SequentialBlock current;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock)); //current = header
        accessMemSec++;

        SequentialBlock next;
        while(current.next != -1){
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&next, sizeof(SequentialBlock));
                accessMemSec++;
            }
            else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&next, sizeof(SequentialBlock));
                accessMemSec++;
            }

            if (next.record.getPrimaryKey() == registro.getPrimaryKey()){//Si se encuentra el key
                data.close();
                aux.close();
                return false; //Ya existe, no se puede agregar
            }
            else if (next.record.getPrimaryKey() > registro.getPrimaryKey()){//si el siguiente es mayor, stop
                break;
            }
            else if (next.record.getPrimaryKey() < registro.getPrimaryKey()){//si el siguiente es menor, avanzamos
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

        aux.seekg(0, ios::end);
        pos = aux.tellg();
        aux.write((char*)&block, sizeof(SequentialBlock));
        accessMemSec++;
        auxCount++;
        current.next = pos;
        current.next_file = 'A';

        if (current_file == 'D'){
            data.seekg(current_pos, ios::beg);
            data.write((char*)&current, sizeof(SequentialBlock));
            accessMemSec++;
        }
        else if (current_file == 'A'){
            aux.seekg(current_pos, ios::beg);
            aux.write((char*)&current, sizeof(SequentialBlock));
            accessMemSec++;
        }
        data.close();
        aux.close();

        return true;
    }

    template<typename T>
    bool remove(T key){
        fstream aux(auxfile, ios::in | ios::out | ios::binary);
        fstream data(datafile, ios::in | ios::out | ios::binary);
        long current_pos = 0;
        long temp_pos = 0;
        char current_file = 'D';
        SequentialBlock current;
        data.seekg(0, ios::beg);
        data.read((char*)&current, sizeof(SequentialBlock)); //current = header
        accessMemSec++;

        SequentialBlock next;
        while(current.next != -1){
            if (current.next_file == 'D'){
                data.seekg(current.next, ios::beg);
                data.read((char*)&next, sizeof(SequentialBlock));
                accessMemSec++;
                if (next.record.getPrimaryKey() == key){
                    temp_pos = next.next;
                    next.next = -2;
                    data.seekg(current.next, ios::beg);
                    data.write((char*)&next, sizeof(SequentialBlock));
                    accessMemSec++;
                }
            }
            else if (current.next_file == 'A'){
                aux.seekg(current.next, ios::beg);
                aux.read((char*)&next, sizeof(SequentialBlock));
                accessMemSec++;
                if (next.record.getPrimaryKey() == key){
                    temp_pos = next.next;
                    next.next = -2;
                    aux.seekg(current.next, ios::beg);
                    aux.write((char*)&next, sizeof(SequentialBlock));
                    accessMemSec++;
                }
            }

            if (next.record.getPrimaryKey() == key){//Si se encuentra el key
                current.next = temp_pos;
                current.next_file = next.next_file;
                if (current_file == 'D'){
                    data.seekg(current_pos, ios::beg);
                    data.write((char*)&current, sizeof(SequentialBlock));
                    accessMemSec++;
                }
                else if (current_file == 'A'){
                    aux.seekg(current_pos, ios::beg);
                    aux.write((char*)&current, sizeof(SequentialBlock));
                    accessMemSec++;
                }

                data.close();
                aux.close();
                deletedCount++;
                return true; //Key eliminada
            }
            else if (next.record.getPrimaryKey() > key){//si el siguiente es mayor, no existe el key
                return false;
            }
            else if (next.record.getPrimaryKey() < key){//si el siguiente es menor, avanzamos
                current_pos = current.next;
                current_file = current.next_file;
                current = next;
            }
        }

        if (this->deletedCount == K){
            rebuild();
        }
        return false;
    }

    template<typename T>
    vector<Record> search(T key){
        vector<Record> res;
        fstream data(datafile, ios::in | ios::binary);
        SequentialBlock current;
        data.seekg(0, ios::end);
        long long fileSize = data.tellg();
        int low = 0, high = fileSize/sizeof(SequentialBlock) - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            data.seekg(mid * sizeof(SequentialBlock),ios::beg);
            data.read((char*)&current, sizeof(SequentialBlock));
            accessMemSec++;
            if (current.record.getPrimaryKey() == key and current.next != -2) {
                res.push_back(current.record);
                data.close();
                return res;
                //codigo en caso el key se repita
            } else if (current.record.getPrimaryKey() < key) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        fstream aux(auxfile, ios::in | ios::binary);
        data.seekg(0, ios::end);
        while (aux.read((char*)(&current), sizeof(SequentialBlock))) {
            accessMemSec++;
            if (current.record.getPrimaryKey() == key and current.next != -2) {
                res.push_back(current.record);
            }
        }

        return res;
    };

    template<typename T>
    vector<Record> rangeSearch(T begin_key, T end_key){
        vector<Record> res;
        fstream data(datafile, ios::in | ios::binary);
        SequentialBlock current;
        data.seekg(0, ios::end);
        long long n = data.tellg()/sizeof(SequentialBlock);
        int low = 0, high = n - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            data.seekg(mid * sizeof(SequentialBlock),ios::beg);
            data.read((char*)&current, sizeof(SequentialBlock));
            accessMemSec++;

            if (current.record.getPrimaryKey() < begin_key) {
                low = mid + 1;
            }
            else if (current.record.getPrimaryKey() > end_key){
                high = mid - 1;
            }
            else{
                int i = mid;
                while (current.record.getPrimaryKey() <= end_key) {
                    if (current.next != -2 and current.record.getPrimaryKey() >= begin_key) {
                        res.push_back(current.record);
                    }
                    i++;
                    data.seekg(i*sizeof(SequentialBlock),ios::beg);
                    data.read((char*)&current, sizeof(SequentialBlock));
                    accessMemSec++;
                }
                i = mid;
                while (current.record.getPrimaryKey() >= begin_key) {
                    if (current.next != -2 and current.record.getPrimaryKey() <= end_key) {
                        res.push_back(current.record);
                    }
                    i--;
                    data.seekg(i*sizeof(SequentialBlock),ios::beg);
                    data.read((char*)&current, sizeof(SequentialBlock));
                    accessMemSec++;
                }
                low = mid + 1;
                high = mid - 1;
            }
        }

        data.close();

        fstream aux(auxfile, ios::in | ios::binary);
        aux.seekg(0, ios::beg);
        while (aux.read((char*)(&current), sizeof(SequentialBlock))) {
            accessMemSec++;
            if (current.record.getPrimaryKey() >= begin_key and current.record.getPrimaryKey() <= end_key) {
                if (current.next != -2) {
                    res.push_back(current.record);
                }
            }
        }

        aux.close();

        return res;
    };
};

#endif //PROYECTO1BDD2_SEQ_H