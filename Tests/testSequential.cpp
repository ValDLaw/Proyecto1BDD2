#include "../Sequential.h"
#include <chrono>
#include "tests.cpp"

void demostrationSequential(){
    string data = "../SQ_data.dat";
    string aux = "../SQ_aux.dat";

    SequentialFile<IntPayment> seqFile(data, aux);
    cout << "INSERT" << endl;
    seqFile.add(IntPayment(3,1,"credit_card",6,90.99));
    seqFile.add(IntPayment(9,1,"voucher",1,40.99));
    seqFile.add(IntPayment(4,1,"credit_card",2,99.00));
    seqFile.add(IntPayment(5,1,"boleto",4,19.00));
    seqFile.add(IntPayment(6,1,"credit_card",1,32.99));
    seqFile.add(IntPayment(7,1,"boleto",5,72.99));
    seqFile.add(IntPayment(8,1,"voucher",2,16.20));
    seqFile.add(IntPayment(12,1,"credit_card",2,10.99));
    seqFile.add(IntPayment(13,1,"boleto",1,15.99));
    seqFile.add(IntPayment(14,1,"credit_card",2,20.00));
    seqFile.read();

    cout << "SEARCH 4" << endl;
    auto find5 = seqFile.search(4);
    if (find5.empty()){cout << "WRONG: 4 not found" << endl;}
    else{cout << "CORRECT: 4 found" << endl;}
    cout << "SEARCH 10" << endl;
    find5 = seqFile.search(10);
    if (find5.empty()){cout << "CORRECT: 10 not found" << endl;}
    else{cout << "WRONG: 10 found" << endl;}
    auto getvalues = seqFile.rangeSearch(2,10);
    cout << "SEARCH FROM 2 TO 10" << endl;
    for (auto item : getvalues){
        cout << item.getPrimaryKey() << ",";
    }
    cout << endl;

    for (auto i=110; i!=0; i--){
        seqFile.add(IntPayment(i, 2, "boleto", 1, 99.99));
    }
    cout << "REBUILD" << endl;
    seqFile.read();
    cout << endl;
};

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