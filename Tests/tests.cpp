#include "../Entities/IntPayment.h"
#include <vector>

vector<IntPayment> readIntPayments(string filename){
    vector<IntPayment> payments;
    ifstream archivo(filename);
    string linea;

    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo." << endl;
    }

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        int id;
        string payment_type;
        int payment_sequential, payment_installments;
        float payment_value;
        char ctr[11];

        ss >> id;
        ss >> payment_sequential;
        ss.ignore();
        getline(ss, payment_type, ',');
        std::strcpy(ctr, payment_type.c_str());
        ss >> payment_installments;
        ss.ignore();
        ss >> payment_value;

        IntPayment pago(id, payment_sequential, ctr, payment_installments, payment_value);
        payments.push_back(pago);
    }
    return payments;
}
