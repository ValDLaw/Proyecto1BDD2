//
// Created by ValDLaw on 4/21/23.
//

#ifndef PROYECTO1BDD2_PAYMENT_H
#define PROYECTO1BDD2_PAYMENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Payment {
    string order_id;
    short payment_sequential;
    string payment_type;
    short payment_installments;
    float payment_value;

    void setData() {
        cout << "Order ID:";
        cin >> order_id;
        cout << "Payment sequential: ";
        cin >> payment_sequential;
        cout << "Payment type: ";
        cin >> payment_type;
        cout << "Payment installments: ";
        cin >> payment_installments;
        cout << "Payment value: ";
        cin >> payment_value;
    }
    void showData() {
        cout << "\nOrder ID: " << order_id;
        cout << "\nPayment sequential: " << payment_sequential;
        cout << "\nPayment type: " << payment_type;
        cout << "\nPayment installments: " << payment_installments;
        cout << "\nPayment value: " << payment_value;
    }
};

istream & operator >> (istream & stream, Payment & p){
    string S, T;
    getline(stream, S); // Skipeamos la primera fila de nombres de atributos
    getline(stream, S);
    stringstream X(S); // leer string tipo stream
    short count = 1;

    while (getline(X, T, ',')) {
        if(count == 1) {p.order_id = T;}
        else if(count == 2) {p.payment_sequential = stoi(T);}
        else if(count == 3) p.payment_type = stoi(T);
        else if(count == 4) p.payment_installments = stoi(T);
        else  p.payment_value = stof(T);
        // cout << T << endl;
        count++;
    }
    return stream;
}

ostream & operator << (ostream & stream, Payment & p)
{
    stream << "\n";
    stream << p.order_id;
    stream << ",";
    stream << p.payment_sequential;
    stream << ",";
    stream << p.payment_type;
    stream << ",";
    stream << p.payment_installments;
    stream << ",";
    stream << p.payment_value;
    stream << flush;
    return stream;
}

#endif //PROYECTO1BDD2_PAYMENT_H
