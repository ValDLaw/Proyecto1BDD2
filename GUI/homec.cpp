#include "home.h"
#include "ui_home.h"
#include "tests.cpp"
#include "Payment.h"
#include "Product.h"
#include "AVL.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <string>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

Home::Home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    ui->productsTable->setColumnCount(5);
    ui->productsTable->setHorizontalHeaderLabels({"OrderID","Sequential","Type","Installments","Value"});
    ui->productsTable->setSelectionBehavior(QTableView::SelectRows);

    ui->paymentsTable->setColumnCount(5);
    ui->paymentsTable->setHorizontalHeaderLabels({"ProductID","Category","Name length","Description length","Photos",});
    ui->paymentsTable->setSelectionBehavior(QTableView::SelectRows);
}

Home::~Home()
{
    delete ui;
}

void Home::on_pushButton_clicked()
{
    QString sql = ui->sentenciaSQL->text();
    AVL<Payment> Arbolito("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_payments_dataset.csv");
    string ss = sql.toStdString();


    if (sql.startsWith("SELECT")) {
        std::istringstream iss(ss);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }
        string column = "";
        string table = "";
        string keyToSearch = "";
        string keyValue = "";
        for (int i = 0; i<tokens.size(); i++) {
            if (tokens[i] == "SELECT"){
                column = tokens[i+1];
                i += 1;
            } else if (tokens[i] == "FROM"){
                table = tokens[i+1];
                i+=1;
            } else if (tokens[i] == "WHERE"){
                keyToSearch = tokens[i+1];
                i+=1;
            } else if (tokens[i] == "=="){
                keyValue = tokens[i+1];
                i+=1;
            }
        }
        qDebug() << column;
        qDebug() << table;
        qDebug() << keyToSearch;
        qDebug() << keyValue;
        cout << endl << "columns " << column << endl;
        cout << endl << "table " << table << endl;
        cout << endl << "keyToSearch " << keyToSearch << endl;
        cout << endl << "keyValue " << keyValue << endl;

        // Consulta de selección
        QString tableName = QString::fromStdString(table);// Obtener el nombre de la tabla de la consulta
        vector<Payment> payments_result;
        char buf[32] = {0};
        //payments_result = Arbolito.search((char*)"8a0ecc540156c0f21f25d7bfd4e95dbe");
        //payments_result = Arbolito.search(std::strncpy(buf, keyValue.c_str(), sizeof(buf)-1));
        ui->paymentsTable->clear();
        for (const Payment& payment : payments_result) {
            int row = ui->paymentsTable->rowCount();
            ui->paymentsTable->insertRow(row);
            ui->paymentsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(payment.getID())));
            ui->paymentsTable->setItem(row,1, new QTableWidgetItem(QString::number(payment.getPaymentSequential())));
            ui->paymentsTable->setItem(row,2, new QTableWidgetItem(QString::fromStdString(payment.getPaymentType())));
            ui->paymentsTable->setItem(row,3, new QTableWidgetItem(QString::number(payment.getPaymentInstallments())));
            ui->paymentsTable->setItem(row,4, new QTableWidgetItem(QString::number(payment.getPaymentValue())));
        }

        //QVector<MyData> results = myClass.search(tableName, condition);
        // Mostrar los resultados en la interfaz de usuario
    }
    else if (sql.startsWith("INSERT")) {
        std::istringstream iss(ss);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(iss, token, ' ')) {
            tokens.push_back(token);
        }
        string table = "";
        char order_id[32];
        int payment_sequential;
        char payment_type[11];
        int payment_installments;
        float payment_value;
        if (tokens[0] == "INSERT" && tokens[1] == "INTO"){
            table = tokens[3];
        }
        qDebug() << order_id;
        /*
        if (tokens[11] == "VALUES"){
            copy_n(tokens[13], 32, order_id[32]);
            payment_sequential = stoi(tokens[15]);
            copy_n(tokens[17], 11, payment_type);
            payment_installments = stoi(tokens[19]);
            payment_value = stof(tokens[21]);
        }*/

        qDebug() << order_id;
        qDebug() << payment_sequential;
        qDebug() << payment_type;
        qDebug() << payment_installments;
        qDebug() << payment_value;
        cout << endl << "order_id " << order_id << endl;
        cout << endl << "payment_sequential " << payment_sequential << endl;
        cout << endl << "payment_type " << payment_type << endl;
        cout << endl << "payment_installments " << payment_installments << endl;
        cout << endl << "payment_value " << payment_value << endl;
    } /*else if (sql.startsWith("DELETE")) {
        // Consulta de eliminación
        // Eliminar datos de la estructura de datos
    } else {
        // Consulta no válida
    }*/

}


void Home::on_pushButton_3_clicked()
{
    //QList<Payment> payments = readPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv");
    vector<Payment> payments;
    ifstream archivo("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv");
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
        std::strncpy(cstr, order_id.c_str(), sizeof(cstr));
        ss >> payment_sequential;
        ss.ignore();
        getline(ss, payment_type, ',');
        std::strncpy(ctr, payment_type.c_str(), sizeof(ctr));
        ss >> payment_installments;
        ss.ignore();
        ss >> payment_value;

        Payment pago(cstr, payment_sequential, ctr, payment_installments, payment_value);
        payments.push_back(pago);
    }
    for (const Payment& payment : payments) {
        int row = ui->paymentsTable->rowCount();
        ui->paymentsTable->insertRow(row);
        ui->paymentsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(payment.getID())));
        ui->paymentsTable->setItem(row,1, new QTableWidgetItem(QString::number(payment.getPaymentSequential())));
        ui->paymentsTable->setItem(row,2, new QTableWidgetItem(QString::fromStdString(payment.getPaymentType())));
        ui->paymentsTable->setItem(row,3, new QTableWidgetItem(QString::number(payment.getPaymentInstallments())));
        ui->paymentsTable->setItem(row,4, new QTableWidgetItem(QString::number(payment.getPaymentValue())));
    }
}


void Home::on_pushButton_2_clicked()
{
    //QList<Payment> payments = readPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_products_dataset.csv");
    vector<Product> products;
    ifstream archivo("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_products_dataset.csv");
    string linea;

    if(!archivo.is_open()){
        cout << "No se pudo abrir el archivo." << endl;
    }

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string product_id, category;
        int name_length, description_length, photos;
        char cstr[32], ctr[11];

        getline(ss, product_id, ',');
        std::strncpy(cstr, product_id.c_str(), sizeof(cstr));
        getline(ss, category, ',');
        std::strncpy(ctr, category.c_str(), sizeof(ctr));
        ss >> name_length;
        ss.ignore();
        ss >> description_length;
        ss.ignore();
        ss >> photos;

        Product producto(cstr, ctr, name_length, description_length, photos);
        products.push_back(producto);
    }
    for (const Product& p : products) {
        int row = ui->productsTable->rowCount();
        ui->productsTable->insertRow(row);
        ui->productsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(p.getID())));
        ui->productsTable->setItem(row,1, new QTableWidgetItem(QString::fromStdString(p.getCategory())));
        ui->productsTable->setItem(row,2, new QTableWidgetItem(QString::number(p.getNameLenght())));
        ui->productsTable->setItem(row,3, new QTableWidgetItem(QString::number(p.getDescriptionLenght())));
        ui->productsTable->setItem(row,4, new QTableWidgetItem(QString::number(p.getPhotosLenght())));
    }
}

