#include <iostream>
#include <vector>
#include "../AVL.h"

#include <chrono>


using namespace	std;

template<typename Record=Payment>
void paymentTest(vector<Record> data, int size){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << size << endl;
    AVL<Payment> Arbolito("TestPayment" + to_string(size) + ".dat");

    // INSERT
    int count = 0;
    auto start  = chrono::steady_clock::now();
    for(int i = 0; i < size; i++){
        count++;
        Arbolito.insert(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "INSERT: " << duration << "milisegundos" << endl;

    // SEARCH
    start  = chrono::steady_clock::now();
    vector<Payment> vec = Arbolito.search((char*)"8a0ecc540156c0f21f25d7bfd4e95dbe");
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::nanoseconds>(end-start).count();
    cout <<"SEARCH: " << duration << "nanosegundos" << endl;

    cout << "SECONDARY ACCESS" << Arbolito.getNumberAccess() << endl;
    cout << "FINISH TEST "<< size << endl;
    cout << "----------------------------------------" << endl;
}
