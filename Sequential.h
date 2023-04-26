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
    string datafile;
    string auxfile;
    int accessMemSec;
    struct SequentialBlock{
        long next;
        char file; //D or A
    };

    vector<Registro> search(T key);
    vector<Registro> rangeSearch(T begin-key, T end-key);
    bool add(Registro registro);
    bool remove(T key);
};