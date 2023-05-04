//
// Created by Valeria Espinoza Tarazona on 3/05/23.
//

#ifndef PROYECTO1BDD2_PRODUCT_H
#define PROYECTO1BDD2_PRODUCT_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>

using namespace std;

struct IntProduct {
private:
    int ID;
    char product_id[32];
    char category[50];
    int product_name_length;
    int product_description_length;
    int product_photos;
public:
    int nextDel;
    char reference;
    IntProduct() = default;
    IntProduct(int ID, char product_id[32], char category[50], int product_name_lenght, int product_description_lenght, int product_photos){

        copy_n(product_id, 32, this->product_id);
        copy_n(category, 50, this->category);
        this->ID = ID;
        this->product_name_length = product_name_lenght;
        this->product_description_length = product_description_lenght;
        this->product_photos = product_photos;
        this->nextDel = 0;
    }
    void showData(){
        cout << "ID      : " << ID << endl;
        cout << "Product ID      : " << product_id << endl;
        cout << "Category      : " << category << endl;
        cout << "Product name lenght   : " << product_name_length << endl;
        cout << "Product description     : " << product_description_length << endl;
        cout << "Product photos       : " << product_photos << endl;
    }
    bool operator < (IntProduct& other){
        return strcmp(this->product_id, other.product_id) < 0;
    }

    bool operator == (IntProduct& other){
        return strcmp(this->product_id, other.product_id) == 0;
    }

    int getPrimaryKey() const {
        return ID;
    }

    bool equalToKey(const char* key){
        return strcmp(this->product_id, key) == 0;
    }

    string getProductID()const {
        return this->product_id;
    }

    string getCategory()const {
        return this->category;
    }

    int getNameLenght()const {
        return this->product_name_length;
    }

    int getDescriptionLenght()const {
        return this->product_description_length;
    }

    int getPhotosLenght()const {
        return this->product_photos;
    }

    void setID(string id){
        strcmp(this->product_id, id.c_str());
    }

    void setCategory(string category){
        strcmp(this->category, category.c_str());
    }

    void setNameLenght(string name_length){
        this->product_name_length = stoi(name_length);
    }

    void setDescriptionLength(string description_length){
        this->product_description_length = stoi(description_length);
    }

    void setPhotosLength(string photos_length){
        this->product_photos = stoi(photos_length);
    }

};

struct ProductHash{
    int operator()(const char* key){
        string str(key);
        hash<string> myHash;
        return myHash(str);
    }
};

#endif //PROYECTO1BDD2_INTPRODUCT_H
