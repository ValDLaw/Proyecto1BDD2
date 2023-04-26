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
        long next;
        char file; //D or A
        bool deleted;
        Registro record;
    };
    string datafile;
    string auxfile;
    int accessMemSec;
    SequentialBlock header;

    SequentialFile(){
        SequentialBlock header.next = -1;
        SequentialBlock header.file = 'D';
    }

    vector<Registro> search(T key);
    vector<Registro> rangeSearch(T begin-key, T end-key);
    
    bool add(Registro registro){
        SequentialBlock temp;
        if header.file = 'D'{
            fstream datafile(datafile, ios::ate);
            datafile.seekg(header.next);
            datafile.read((char*)&temp, sizeof(SequentialBlock));
            
        }
        



    }
    bool remove(T key);
};