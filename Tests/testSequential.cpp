#include "../Sequential.h"
#include <chrono>
#include "tests.cpp"

template<typename Record=Payment>
void paymentTest(vector<Record> data){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << data.size() << "- Sequential" << endl;
    SequentialFile<string> seqFile("../SQ_test_data.dat", "../SQ_test_aux.dat");

    // INSERT
    auto start  = chrono::steady_clock::now();
    for(int i = 0; i < data.size(); i++){
        seqFile.add(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "INSERT: " << duration << endl;

    // SEARCH
    start  = chrono::steady_clock::now();
    const char* codigo = data[100].getPrimaryKey();
    seqFile.search(codigo);
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout <<"SEARCH: " << duration << endl;

    cout << "FINISH TEST "<< data.size() <<" - Sequential" << endl;
    cout << "----------------------------------------" << endl;
}

void tests(){
    vector<Payment> payments = readPayments("../Datasets/olist_order_payments_dataset.csv");
    for (auto p:payments){
        cout << p.getID() << endl;
    }
}