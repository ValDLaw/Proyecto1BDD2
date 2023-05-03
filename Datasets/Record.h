#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Record {
protected:
    string id;
public:
    //Record() = default;
    Record(string recordID){
        id = recordID;
    }
    virtual istream& read(istream& input) {
        input >> id;
        return input;
    }
    string getID(){
        return id;
    }
    virtual void setData() = 0;
    virtual void showData() = 0;

    virtual ~Record() {

    }
};

class Payment : public Record {
    short payment_sequential;
    string payment_type;
    short payment_installments;
    float payment_value;
public:
    Payment(string id, short ps, string pt, short pi, float pv) : Record(id) {
        payment_sequential = ps;
        payment_type = pt;
        payment_installments = pi;
        payment_value = pv;
    }
    friend istream & operator >> (istream & stream, Payment& pay) {
        string S, T;
        getline(stream, S);
        stringstream X(S); // leer string tipo stream
        short count = 1;

        while (getline(X, T, ',')) {
            if(count == 1) {pay.id = T;}
            else if(count == 2) {pay.payment_sequential = stoi(T);}
            else if(count == 3) pay.payment_type = stoi(T);
            else if(count == 4) pay.payment_installments = stoi(T);
            else pay.payment_value = stof(T);
            // cout << T << endl;
            count++;
        }
        return stream;
    }
    
    virtual istream& read(istream& input) override {
        string line;
        getline(input, line); // Skipeamos header del csv
        stringstream ss(line);
        ss >> *this;
        return input;
    }
    /*
    virtual istream& read(istream& stream) override {
        Record::read(stream);  // Llama a la implementación de leer de la clase base
        stream >> *this;  // Llama a la sobrecarga del operador >> de istream de la clase Payment
        return stream;
    }*/

    friend ostream & operator << (ostream & stream, const Payment& pay)
    {
        stream << "\n";
        stream << pay.id;
        stream << ",";
        stream << pay.payment_sequential;
        stream << ",";
        stream << pay.payment_type;
        stream << ",";
        stream << pay.payment_installments;
        stream << ",";
        stream << pay.payment_value;
        stream << flush;
        return stream;
    }

    void setData() override {
        cout << "Order ID:";
        cin >> id;
        cout << "Payment sequential: ";
        cin >> payment_sequential;
        cout << "Payment type: ";
        cin >> payment_type;
        cout << "Payment installments: ";
        cin >> payment_installments;
        cout << "Payment value: ";
        cin >> payment_value;
    }
    void showData() override{
        cout << "\nOrder ID: " << id;
        cout << "\nPayment sequential: " << payment_sequential;
        cout << "\nPayment type: " << payment_type;
        cout << "\nPayment installments: " << payment_installments;
        cout << "\nPayment value: " << payment_value;
    }

    ~Payment() {}
};

struct Product : public Record {
    string category;
    short product_name_lenght;
    string product_description;
    short product_photos;
public:
    Product(string id): Record(id){};
    Product(string id, string c, short pnl, string pd, short pp) : Record(id) {
            category = c;
            product_name_lenght = pnl;
            product_description = pd;
            product_photos = pp;
    }
    virtual istream& read(istream& input) override {
        string line;
        getline(input, line); // Skipeamos header del csv
        stringstream ss(line);
        ss >> *this;
        return input;
    }

    friend istream & operator >> (istream & stream, Product & prod) {
        string S, T;
        getline(stream, S);
        stringstream X(S); // leer string tipo stream
        short count = 1;

        while (getline(X, T, ',')) {
            if(count == 1) {prod.id = T;}
            else if(count == 2) {prod.category = T;}
            else if(count == 3) prod.product_name_lenght = stoi(T);
            else if(count == 4) prod.product_description = T;
            else  prod.product_photos = stoi(T);
            // cout << T << endl;
            count++;
        }
        return stream;
    }
    /*
    //Anterior istream
    friend istream & operator >> (istream & stream, Product & prod) {
        string S, T;
        //getline(stream, S); // Skipeamos la primera fila de nombres de atributos
        getline(stream, S);
        stringstream X(S); // leer string tipo stream
        short count = 1;

        while (getline(X, T, ',')) {
            if(count == 1) {prod.id = T;}
            else if(count == 2) {prod.category = T;}
            else if(count == 3) prod.product_name_lenght = stoi(T);
            else if(count == 4) prod.product_description = T;
            else  prod.product_photos = stoi(T);
            // cout << T << endl;
            count++;
        }
        return stream;
    }
    */

    friend ostream & operator << (ostream & stream, const Product& prod)
    {
        stream << "\n";
        stream << prod.id;
        stream << ",";
        stream << prod.category;
        stream << ",";
        stream << prod.product_name_lenght;
        stream << ",";
        stream << prod.product_description;
        stream << ",";
        stream << prod.product_photos;
        stream << flush;
        return stream;
    }

    void setData() override {
        cout << "Product ID:";
        cin >> id;
        cout << "Category: ";
        cin >> category;
        cout << "Product name lenght: ";
        cin >> product_name_lenght;
        cout << "Product description: ";
        cin >> product_description;
        cout << "Product number of photos: ";
        cin >> product_photos;
    }
    void showData() override {
        cout << "\nProduct ID: " << id;
        cout << "\nCategory: " << category;
        cout << "\nProduct name lenght : " << product_name_lenght;
        cout << "\nProduct description: " << product_description;
        cout << "\nProduct number of photos : " << product_photos;
    }

    ~Product() {}
};