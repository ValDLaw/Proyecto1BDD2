#include "../Entities/Payment.h"
#include "../Entities/Product.h"


vector<Payment> readPayments(string filename){
    vector<Payment> payments;
    ifstream archivo(filename);
    string linea;

    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo." << endl;
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
        payments.push_back(pago);
    }
    return payments;
}
