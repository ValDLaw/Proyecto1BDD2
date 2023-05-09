#include "../AVL.h"
#include <chrono>

template<typename Record=Payment>
void AVLpaymentTest(vector<Record> data, int size){
    cout << "----------------------------------------" << endl;
    cout << "START TEST " << size << endl;
    AVL<Payment> Arbolito("../TestPayment" + to_string(size) + ".dat");

    // INSERT
    auto start  = chrono::steady_clock::now();
    for(int i = 0; i < size; i++){
        Arbolito.insert(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << "INSERT: " << endl;
    cout << "   EXECUTION TIME: " << duration << " milisegundos" << endl;
    cout << "   SECONDARY ACCESS: " << Arbolito.getNumberAccess() << endl;

    Arbolito.restartNumberAccess();

    // SEARCH
    start  = chrono::steady_clock::now();
    vector<Payment> vec = Arbolito.search((char*)"8a0ecc540156c0f21f25d7bfd4e95dbe");
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout << "SEARCH: " << endl;
    cout << "   EXECUTION TIME: " << duration << " microsegundos" << endl;
    cout << "   SECONDARY ACCESS" << Arbolito.getNumberAccess() << endl;

    cout << "FINISH TEST "<< size << endl;
    cout << "----------------------------------------" << endl;
}
