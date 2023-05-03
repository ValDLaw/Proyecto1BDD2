//
// Created by Valeria Espinoza Tarazona on 3/05/23.
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
private:
    char order_id[32];
    int payment_sequential;
    char payment_type[11];
    int payment_installments;
    float payment_value;
public:
    int nextDel;
    char reference;
    Payment() = default;
    Payment(string order_id, int payment_sequential, string payment_type, int payment_installments, float payment_value){
        if(order_id.size() > 32 ) order_id = order_id.substr(0, 32);
        this->payment_sequential = payment_sequential;
        if(payment_type.size() > 11) payment_type = payment_type.substr(0, 11);
        this->payment_installments = payment_installments;
        this->payment_value = payment_value;
        this->nextDel = 0;
    }
    void showData(){
        cout << "Order ID      : " << order_id << endl;
        cout << "Payment sequential      : " << payment_sequential << endl;
        cout << "Payment type   : " << payment_type << endl;
        cout << "Payment installments     : " << payment_installments << endl;
        cout << "Payment value       : " << payment_value << endl;
    }
    bool operator < (Payment& other){
        return strcmp(this->order_id, other.order_id) < 0;
    }

    bool operator == (Payment& other){
        return strcmp(this->order_id, other.order_id) == 0;
    }

    const char *getPrimaryKey() const {
        return order_id;
    }
    bool equalToKey(const char* key){
        return strcmp(this->order_id, key) == 0;
    }

    bool lessThanToKey(const char* key){
        return strcmp(this->order_id, key) < 0;
    }

    bool greaterThanToKey(const char* key){
        return strcmp(this->order_id, key) > 0;
    }

    bool lessThanOrEqualToKey(const char* key){
        return strcmp(this->order_id, key) <= 0;
    }

    bool greaterThanOrEqualToKey(const char* key){
        return strcmp(this->order_id, key) >= 0;
    }

    string getID()const {
        return this->order_id;
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

    void setID(string id){
        strcmp(this->order_id, id.c_str());
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

};

struct PaymentHash{
    int operator()(const char* key){
        string str(key);
        hash<string> myHash;
        return myHash(str);
    }
};

#endif //PROYECTO1BDD2_PAYMENT_H
