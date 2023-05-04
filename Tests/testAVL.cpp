#include <iostream>
#include <vector>
#include <sstream>
#include "../Entities/Product.h"
#include "../Entities/Payment.h"
#include "../AVL.h"
#include "../Tests/tests.cpp"
#include <chrono>


using namespace	std;



void TestPayment(){
    AVL<Payment> Arbolito10("TestPayment10.dat");
    string direc = "/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv";
    cout << "----------------------------AVL TEST---------------------------" << endl;

    auto data = readPayments(direc);
    cout << "\nINICIANDO TEST CON 10K" << endl;
    cout << "************************" << endl;
    int count = 0;
    auto start  = chrono::steady_clock::now();
    for(auto i = 0; i<= data.size()/10; i++) {
        count++;
        Arbolito10.insert(data[i]);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration in miliseconds: " <<duration << endl;
    cout << "Number of Memory Accesses:  " << Arbolito10.getNumberAccess() << endl;

    cout << "\nINICIANDO TEST CON 50K" << endl;
    cout << "************************" << endl;
    count = 0;
    AVL<Payment> Arbolito50("TestPayment.dat");
    start  = chrono::steady_clock::now();
    for(auto i = 0; i<= data.size()/2; i++) {
        count++;
        Arbolito50.insert(data[i]);
    }
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration in miliseconds: " <<duration << endl;
    cout << "Number of Memory Accesses:  " << Arbolito50.getNumberAccess() << endl;

    cout << "\nINICIANDO TEST CON 100K" << endl;
    cout << "************************" << endl;
    count = 0;
    AVL<Payment> Arbolito100("TestPayment.dat");
    start  = chrono::steady_clock::now();
    for(auto i = 0; i<= data.size(); i++) {
        count++;
        Arbolito100.insert(data[i]);
    }
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration in miliseconds: " <<duration << endl;
    cout << "Number of Memory Accesses:  " << Arbolito100.getNumberAccess() << endl;

}
