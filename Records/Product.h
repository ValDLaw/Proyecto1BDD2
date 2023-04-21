//
// Created by ValDLaw on 4/21/23.
//

#ifndef PROYECTO1BDD2_PRODUCT_H
#define PROYECTO1BDD2_PRODUCT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Product {
    string product_id;
    string category;
    short product_name_lenght;
    string product_description;
    short product_photos;

    void setData() {
        cout << "Product ID:";
        cin >> product_id;
        cout << "Category: ";
        cin >> category;
        cout << "Product name lenght: ";
        cin >> product_name_lenght;
        cout << "Product description: ";
        cin >> product_description;
        cout << "Product number of photos: ";
        cin >> product_photos;
    }
    void showData() {
        cout << "\nProduct ID: " << product_id;
        cout << "\nCategory: " << category;
        cout << "\nProduct name lenght : " << product_name_lenght;
        cout << "\nProduct description: " << product_description;
        cout << "\nProduct number of photos : " << product_photos;
    }
};

istream & operator >> (istream & stream, Product & p){
    string S, T;
    getline(stream, S); // Skipeamos la primera fila de nombres de atributos
    getline(stream, S);
    stringstream X(S); // leer string tipo stream
    short count = 1;

    while (getline(X, T, ',')) {
        if(count == 1) {p.product_id = T;}
        else if(count == 2) {p.category = T;}
        else if(count == 3) p.product_name_lenght = stoi(T);
        else if(count == 4) p.product_description = T;
        else  p.product_photos = stoi(T);
        // cout << T << endl;
        count++;
    }
    return stream;
}

ostream & operator << (ostream & stream, Product & p)
{
    stream << "\n";
    stream << p.product_id;
    stream << ",";
    stream << p.category;
    stream << ",";
    stream << p.product_name_lenght;
    stream << ",";
    stream << p.product_description;
    stream << ",";
    stream << p.product_photos;
    stream << flush;
    return stream;
}

#endif //PROYECTO1BDD2_PRODUCT_H
