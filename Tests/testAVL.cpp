#include <iostream>
#include <vector>
#include <sstream>
#include "../Entities/Product.h"
#include "../Entities/Payment.h"
#include "../AVL.h"


using namespace	std;

void TestProduct(){
  AVL<Product> Arbolito("TestProduct.dat");
  string direc = "C:\\Users\\hola\\CLionProjects\\ProjectBD\\Datasets\\olist_products_dataset.csv";
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
  AVL<Payment> Arbolito("TestPayment.dat");
    string direc = "C:\\Users\\hola\\CLionProjects\\ProjectBD\\Datasets\\olist_order_payments_dataset.csv";
    ifstream archivo(direc);
    string linea;

    if(!archivo.is_open()){
        cout << "No abrio" << endl;
    }

    while (getline(archivo, linea)) {
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

        Arbolito.insert(pago);
    }

    Arbolito.scanAll();

    archivo.close();
}
