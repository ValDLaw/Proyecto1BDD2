//
// Created by hola on 2/5/2023.
//

#ifndef PROJECTBD_SMH_H
#define PROJECTBD_SMH_H

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

template<class Data>
struct SMH {
    string filename;

    void readHeader(int &header, fstream &file) {
        file.seekg(0, ios::beg);
        file.read((char *) &header, sizeof(int));
    }

    void writeHeader(int &header, fstream &file) {
        file.seekp(0, ios::beg);
        file.write((char *) &header, sizeof(int));
    }

    void writeData(Data &record, int position, fstream &file) {
        file.seekp(position * sizeof(Data) + sizeof(int), ios::beg);
        file.write((char *) &record, sizeof(Data));
    }

    int numRecords(fstream &file) {
        if (file.is_open()) {
            file.seekg(0, ios::end);
            int n = ((int) file.tellg() - sizeof(int)) / sizeof(Data);
            file.seekg(0, ios::beg);
            return n;
        }
        return 0;
    }

    void readData(Data &record, int position, fstream &file) {
        file.seekg(position * sizeof(Data) + sizeof(int), ios::beg);
        file.read((char *) &record, sizeof(Data));
    }

    SMH() = default;

    void setFilename(string file) {
        filename = file;
    }

    int getNumRecords() {
        fstream file(filename.c_str(), ios::binary | ios::in);
        return numRecords(file);
    }

    int add(Data record) {
        fstream file(filename.c_str(), ios::binary | ios::in);
        if (file.is_open()) {
            file.close();
            file.open(filename.c_str(), ios::binary | ios::in | ios::out);
            int header;
            readHeader(header, file);
            int recordPosition;
            if (header == -1) {
                recordPosition = numRecords(file);
                writeData(record, recordPosition, file);
            } else {
                Data recordDeleted;
                readData(recordDeleted, header, file);
                recordPosition = header;
                header = recordDeleted.nextDeleted;
                writeHeader(header, file);
                writeData(record, recordPosition, file);
            }
            file.close();
            return recordPosition;
        } else {
            file.close();
            file.open(this->filename.c_str(), ios::binary | ios::out);
            int header = -1;
            writeHeader(header, file);
            writeData(record, 0, file);
            return 0;
        }
    }

    Data readRecord(int position) {
        fstream file(filename.c_str(), ios::binary | ios::in);
        Data record;
        if (numRecords(file) == 0) {
            return record;
        }
        if (position >= 0 && position < numRecords(file)) {
            readData(record, position, file);
            file.close();
            return record;
        }
        return record;
    }
    void writeRecord(int position, Data& record){
        fstream file(filename.c_str(), ios::binary | ios::in);
        if(numRecords(file) == 0){
            return;
        }
        if(position >= 0 && position < numRecords(file)){
            file.close();
            file.open(filename.c_str(), ios::binary | ios::out | ios::in);
            writeData(record, position, file);
            file.close();
        }else{
            return;
        }
    }

    void deleteRecord(int position){
        fstream file(filename.c_str(), ios::binary | ios::in | ios::out);
        if(numRecords(file) == 0) {
            return;
        }
        if(position >= 0 && position < numRecords(file)) {
            Data record;
            int header;
            readHeader(header, file);
            writeHeader(position, file);
            readData(record, position, file);
            record.nextDeleted = header;
            writeData(record, position, file);
            file.close();
        }
    }

};

#endif //PROJECTBD_SMH_H
