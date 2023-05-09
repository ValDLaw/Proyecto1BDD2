#include "../ExtendibleHash/Hashindex.h"
#include <chrono>
#include "tests.cpp"

template<typename Record=IntPayment>
void paymentTest(vector<Record> data, int size){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << size << endl;
    HashIndex<Record, char> hashFile("../Hash_data.dat", "../Hash_aux.dat");

    // INSERT
    auto start  = chrono::steady_clock::now();
    for(int i = 500; i < size; i++){
        hashFile.insert(data[i]);
    }
    for(int i = 0; i < 500; i++){
        hashFile.insert(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "INSERT: " << endl;
    cout << "   EXECUTION TIME: " << duration << " milisegundos" << endl;
    cout << "   SECONDARY ACCESS: " << "hashFile.getNumberAccess()" << endl;

    hashFile.restartNumberAccess();

    // SEARCH
    start  = chrono::steady_clock::now();
    vector<IntPayment> vec = hashFile.search(3041);
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << "SEARCH: " << endl;
    cout << "   EXECUTION TIME: " << duration << " microsegundos" << endl;
    cout << "   SECONDARY ACCESS: " << "hashFile.getNumberAccess()" << endl;

    cout << "FINISH TEST "<< size << endl;
    cout << "----------------------------------------" << endl;
}

void tests(){
    vector<IntPayment> payments = readIntPayments("../Datasets/olist_order_int_payments_dataset.csv");
    paymentTest(payments, 10000);
    paymentTest(payments, 50000);
    paymentTest(payments, 100000);
}