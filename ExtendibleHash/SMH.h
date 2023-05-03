//
// Created by hola on 2/5/2023.
//

#ifndef PROJECTBD_SMH_H
#define PROJECTBD_SMH_H

#include <iostream>
#include <vector>
#include "Hashindex.h"
#include <fstream>

using namespace std;

template<typename Record>
class SMH{
private:
    string path;

    void readHeader(int& header, fstream& file){
        file.seekg(0, ios::beg);
        file.read((char *)& header, sizeof(int));
    }

    void writeHeader(int& header, fstream& file){
        file.seekp(0, ios::beg);
        file.write((char *)& header, sizeof(int));
    }

    void readRegister(Record& record, int  position, fstream& file){
        file.seekg(position * sizeof(Record) + sizeof(int), ios::beg);
        file.read((char*)& record, sizeof(Record));
    }

    void writeRegister( fstream& file,  Record& record, int position){
        file.seekp(position*sizeof(Record)+sizeof(int), ios::beg);
        file.write((char*)& record, sizeof(Record));
    }

    int numberOfRecords(fstream& file){
        if(file.is_open()){
            file.seekg(0, ios::end);
            int n = ((int) file.tellg() - sizeof(int)) / sizeof(Record);
            file.seekg(0, ios::beg);
            return n;
        }return 0;
    }
public:
    SMH() = default;
    SMH(const string& path) {
        this->path = path;
    }
    void setPath(string path){
        this->path = path;
    }
    int getNumberOfRecords(){
        fstream file(this->path.c_str(), ios::binary | ios::in);
        return numberOfRecords(file);
    }
    vector<Record*> load(){
        vector<Record*> records;
        fstream file(this->path.c_str(), ios::binary | ios::in);
        if(file.is_open()) {
            Record *record;
            for(int i = 0; i < numberOfRecords(file); ++i){
                readRegister(*record, i, file);
//                if(record->nextDel == 0)
                records.push_back(record);
            }
        }else cout << "No se pudo abrir el archivo - LOAD\n";
        file.close();
        return records;
    }

    int add(Record record){
        fstream file(this->path.c_str(), ios::binary | ios::in);
        if(file.is_open()){
            file.close();
            file.open(this->path.c_str(), ios::binary | ios::in | ios::out);
            int header;
            readHeader(header, file);
            int recordPosition;
            if(header == -1) {
                recordPosition = numberOfRecords(file);
                writeRegister(file ,record, recordPosition); // AQUIIII
            }
            else{ // ESTRATEGIA LIFO
                Record recordDeleted;
                readRegister(recordDeleted, header, file); // AQUIIII
                recordPosition = header;
                header = recordDeleted.nextDel;
                writeHeader(header, file);
                writeRegister(file, record, recordPosition); // AQUIIII
            }
            file.close();
            return recordPosition;
        }
        else{
            file.close();
            file.open(this->path.c_str(), ios::binary | ios::out);
            int header = -1;
            writeHeader(header, file);
            writeRegister(file , record, 0); // AQUIIII
            return 0;
        }
    }

    Record readRecord(int position){
        fstream file(this->path.c_str(), ios::binary | ios::in);
        Record record;
        if(numberOfRecords(file) == 0) {
            cout << "ARCHIVO VACIO - readRecord()" << endl;
            return record;
        }
        if(position >= 0 && position < numberOfRecords(file)){
            readRegister(record, position, file);
            file.close();
            return record;
        }else throw out_of_range("Indice incorrecto");
    }

    void writeRecord(int position, Record& record){
        fstream file(this->path.c_str(), ios::binary | ios::in);
        if(numberOfRecords(file) == 0){
            cout << "archivo vacio - writeRecord()" << endl;
            return;
        }
        if(position >= 0 && position < numberOfRecords(file)){
            file.close();
            file.open(this->path.c_str(), ios::binary | ios::out | ios::in);
            writeRegister( file, record, position);
            file.close();
        }else{
            cout << "indice incorrecto - writeRecord()" << endl;
            return;
        }
    }

    void deleteRecord(int position){
        fstream file(this->path.c_str(), ios::binary | ios::in | ios::out);
        if(numberOfRecords(file) == 0) {
            cout << "Archivo vacio - deleteRecord()" << endl;
            return;
        }

        if(position >= 0 && position < numberOfRecords(file)) {
            Record record;
            int header;
            readHeader(header, file);
            writeHeader(position, file);
            readRegister( record, position, file);
            record.nextDel = header;
            writeRegister(file, record, position);
            file.close();
        }else throw out_of_range("Indice incorrecto");
    }


};

#endif //PROJECTBD_SMH_H
