#include "../ExtendibleHash/Hashindex.h"
#include <chrono>

template<typename Record>
void paymentTest(vector<Record> data, int size){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << size << endl;

    HashIndex<Record, const char *, PaymentHash> hashFile("TestPaymentDATA" + to_string(size) + ".dat", "TestPaymentAux" + to_string(size) + ".dat");

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
    cout << "   SECONDARY ACCESS: " << hashFile.getNumberAccess() << endl;

    hashFile.restartNumberAccess();

    // SEARCH
    start  = chrono::steady_clock::now();
    vector<Payment> vec = hashFile.search("0573b5e23cbd798006520e1d5b4c6714");
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << "SEARCH: " << endl;
    cout << "   EXECUTION TIME: " << duration << " microsegundos" << endl;
    cout << "   SECONDARY ACCESS: " << hashFile.getNumberAccess() << endl;

    cout << "FINISH TEST "<< size << endl;
    cout << "----------------------------------------" << endl;
}