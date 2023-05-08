#include "../Sequential.h"
#include <chrono>
#include "tests.cpp"

template<typename Record=IntPayment>
void paymentTest(vector<Record> data, int size){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << size << endl;
    SequentialFile<Record> seqFile("../SQ_test_data.dat", "../SQ_test_aux.dat");

    // INSERT
    auto start  = chrono::steady_clock::now();
    for(int i = 500; i < size; i++){
        seqFile.add(data[i]);
    }
    for(int i = 0; i < 500; i++){
        seqFile.add(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "INSERT: " << endl;
    cout << "   EXECUTION TIME: " << duration << " milisegundos" << endl;
    cout << "   SECONDARY ACCESS: " << seqFile.getNumberAccess() << endl;

    seqFile.restartNumberAccess();

    // SEARCH
    start  = chrono::steady_clock::now();
    vector<IntPayment> vec = seqFile.search(3041);
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << "SEARCH: " << endl;
    cout << "   EXECUTION TIME: " << duration << " microsegundos" << endl;
    cout << "   SECONDARY ACCESS: " << seqFile.getNumberAccess() << endl;

    cout << "FINISH TEST "<< size << endl;
    cout << "----------------------------------------" << endl;
}

void tests(){
    vector<IntPayment> payments = readIntPayments("../Datasets/olist_order_int_payments_dataset.csv");
    paymentTest(payments, 10000);
    paymentTest(payments, 50000);
    paymentTest(payments, 100000);
}