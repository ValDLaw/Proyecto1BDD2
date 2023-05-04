#include <iostream>
#include <vector>
#include <sstream>
#include "../Entities/Product.h"
#include "../Entities/Payment.h"
#include "../AVL.h"
#include <chrono>


using namespace	std;

void TestProduct(){
  AVL<Product> Arbolito("TestProduct.dat");
  string direc = "/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_products_dataset.csv";
  ifstream archivo(direc);
  string linea;

  if(!archivo.is_open()){
      cout << "No abrio" << endl;
  }


  while (getline(archivo, linea)) {

      istringstream ss(linea);
      string order_id, payment_type;
      int payment_sequential, payment_installments;
      int payment_value;
      char cstr[32], ctr[50];

      getline(ss, order_id, ',');
      std::strcpy(cstr, order_id.c_str());
      getline(ss, payment_type, ',');
      std::strcpy(ctr, payment_type.c_str());
      ss >> payment_sequential;
      ss.ignore();
      ss >> payment_installments;
      ss.ignore();
      ss >> payment_value;


      Product pago(cstr ,ctr , payment_sequential, payment_installments, payment_value);

      Arbolito.insert(pago);
  }

  Arbolito.scanAll();

  archivo.close();

}

void TestPayment(){
    AVL<Payment> Arbolito10("TestPayment10.dat");
    string direc = "/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_products_dataset.csv";
    ifstream archivo(direc);
    string linea;
    cout << "----------------------------AVL TEST---------------------------" << endl;

    if(!archivo.is_open()){
        cout << "No abrio" << endl;

    }

    cout << "\nINICIANDO TEST CON 10K" << endl;
    cout << "************************" << endl;
    int count = 0;
    auto start  = chrono::steady_clock::now();
    while (getline(archivo, linea) && count != 10001) {
        count++;
        istringstream ss(linea);
        string order_id, payment_type;
        int payment_sequential, payment_installments;
        float payment_value;
        char cstr[32], ctr[11];

        getline(ss, order_id, ',');
        std::strcpy(cstr, order_id.c_str());
        ss >> payment_sequential;
        ss.ignore();
        getline(ss, payment_type, ',');
        std::strcpy(ctr, payment_type.c_str());
        ss >> payment_installments;
        ss.ignore();
        ss >> payment_value;

        Payment pago(cstr, payment_sequential, ctr, payment_installments, payment_value);
        Arbolito10.insert(pago);
    }
    auto end = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration: " <<duration << endl;
    cout << "Number of Memory Accesses:  " <<Arbolito10.getNumberAccess() << endl;


    AVL<Payment> Arbolito50("TestPayment50.dat");

    cout << "\nINICIANDO TEST CON 50K" << endl;
    cout << "************************" << endl;
    count = 0;
    start  = chrono::steady_clock::now();

    while (getline(archivo, linea) && count != 50001) {
        count++;
        istringstream ss(linea);
        string order_id, payment_type;
        int payment_sequential, payment_installments;
        float payment_value;
        char cstr[32], ctr[11];

        getline(ss, order_id, ',');
        std::strcpy(cstr, order_id.c_str());
        ss >> payment_sequential;
        ss.ignore();
        getline(ss, payment_type, ',');
        std::strcpy(ctr, payment_type.c_str());
        ss >> payment_installments;
        ss.ignore();
        ss >> payment_value;

        Payment pago(cstr, payment_sequential, ctr, payment_installments, payment_value);
        Arbolito50.insert(pago);
    }
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration: " <<duration << endl;
    cout << "Number of Memory Accesses:  " <<Arbolito50.getNumberAccess() << endl;

    AVL<Payment> Arbolito100("TestPayment100.dat");
    cout << "\nINICIANDO TEST CON 100K" << endl;
    cout << "************************" << endl;
    count = 0;
    start  = chrono::steady_clock::now();
    while (getline(archivo, linea)) {
        count++;
        istringstream ss(linea);
        string order_id, payment_type;
        int payment_sequential, payment_installments;
        float payment_value;
        char cstr[32], ctr[11];

        getline(ss, order_id, ',');
        std::strcpy(cstr, order_id.c_str());
        ss >> payment_sequential;
        ss.ignore();
        getline(ss, payment_type, ',');
        std::strcpy(ctr, payment_type.c_str());
        ss >> payment_installments;
        ss.ignore();
        ss >> payment_value;

        Payment pago(cstr, payment_sequential, ctr, payment_installments, payment_value);
        Arbolito100.insert(pago);
    }
    end = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(end-start).count();
    cout << count <<" Records - Insert Duration: " <<duration << endl;
    cout << "Number of Memory Accesses:  " <<Arbolito100.getNumberAccess() << endl;

    archivo.close();

}
