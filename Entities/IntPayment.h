//
// Created by Valeria Espinoza Tarazona on 4/05/23.
//

#ifndef PROYECTO1BDD2_INTPAYMENT_H
#define PROYECTO1BDD2_INTPAYMENT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>

using namespace std;

struct IntPayment {
    int ID;
    int payment_sequential;
    char payment_type[11];
    int payment_installments;
    float payment_value;
    int nextDel;
    char reference;
    IntPayment() = default;
    IntPayment(int ID, int payment_sequential, char payment_type[11], int payment_installments, float payment_value){
        this->ID = ID;
        this->payment_sequential = payment_sequential;
        copy_n(payment_type, 11, this->payment_type);
        this->payment_installments = payment_installments;
        this->payment_value = payment_value;
        this->nextDel = 0;
    }
    void showData(){
        cout << "ID      : " << ID << endl;
        cout << "Payment sequential      : " << payment_sequential << endl;
        cout << "Payment type   : " << payment_type << endl;
        cout << "Payment installments     : " << payment_installments << endl;
        cout << "Payment value       : " << payment_value << endl;
    }

    bool operator == (IntPayment& other){
        return this->ID == other.ID;
    }

    int getPrimaryKey() const {
        return ID;
    }

    int getPaymentSequential()const {
        return this->payment_sequential;
    }

    string getPaymentType()const {
        return this->payment_type;
    }

    int getPaymentInstallments()const {
        return this->payment_installments;
    }

    float getPaymentValue()const {
        return this->payment_value;
    }

    void setPaymentSequential(string payment_sequential){
        this->payment_sequential = stoi(payment_sequential);
    }

    void setPaymentType(string payment_type){
        strcmp(this->payment_type, payment_type.c_str());
    }

    void setPaymentInstallments(string payment_installments){
        this->payment_installments = stoi(payment_installments);
    }

    void setPaymentValue(string payment_value){
        this->payment_value = stof(payment_value);
    }

    friend ostream& operator<<(ostream& os, const IntPayment& pago) {
        os << "ID: " << pago.ID << endl;
        os << "Payment Sequential: " << pago.payment_sequential << endl;
        os << "Payment Type: " << pago.payment_type << endl;
        os << "Payment Installments: " << pago.payment_installments << endl;
        os << "Payment Value: " << pago.payment_value << endl;
        return os;
    }


};

#endif //PROYECTO1BDD2_INTPAYMENT_H
