#include "home.h"
#include "Sequential.h"
#include "ui_home.h"
#include "tests.cpp"
#include "Product.h"
#include "Payment.h"
#include "AVL.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
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
    ui(new Ui::Home),
    seqFileIntPayment("../SQ_test_data1.dat", "../SQ_test_aux1.dat"),
    seqFileIntProduct("../SQ_test_data2.dat", "../SQ_test_aux2.dat"),
    paymentHash("../indexFile.dat", "../bucketFile.dat"),
    productHash("../indexFile2.dat", "../bucketFile2.dat"),
    paymentAVL("../AVLdata.dat"),
    productAVL("../AVLdata2.dat")
{
    ui->setupUi(this);

    ui->productsTable->setColumnCount(5);
    ui->productsTable->setHorizontalHeaderLabels({"OrderID","Category","Name length","Description length","Photos"});
    ui->productsTable->setSelectionBehavior(QTableView::SelectRows);

    ui->paymentsTable->setColumnCount(5);
    ui->paymentsTable->setHorizontalHeaderLabels({"ProductID","Sequential","Type","Installments","Value"});
    ui->paymentsTable->setSelectionBehavior(QTableView::SelectRows);

    payments = readPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv");
    qDebug() << "payments";
    products = readProducts("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_products_dataset.csv");
    qDebug() << "products";
    intpayments = readIntPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_int_payments_dataset.csv");
    qDebug() << "intpayments";
    intproducts = readIntProducts("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_int_products_dataset.csv");
    qDebug() << "intproducts";
    //int count = 0;

    for(int i = 0; i < 500; i++){
        seqFileIntPayment.add(intpayments[i]);
        //cout << count << endl;
        //count ++;
    }
    //count = 0;
    qDebug() << "SFIPayment creada";
    for(int i = 0; i < 500; i++){
        seqFileIntProduct.add(intproducts[i]);
        cout << intproducts[i].getPrimaryKey() << " " << intproducts[i].getCategory()<<endl;
        //cout << count << endl;
        //count ++;
    }
    qDebug() << "SFIProduct creada";

    /*
    for(int i = 0; i < 10000; i++){
        productHash.insert(products[i]);
        //cout << count << endl;
        //count ++;
    }
    qDebug() << "HashProduct creado";

    for(int i = 0; i < 10000; i++){
        paymentHash.insert(payments[i]);
        //cout << count << endl;
        //count ++;
    }
    qDebug() << "HashPayment creado";*/

    for(int i = 0; i < 10000; i++){
        paymentAVL.insert(payments[i]);
        //cout << count << endl;
        //count ++;
    }
    qDebug() << "AVLPayment creado";

    for(int i = 0; i < 10000; i++){
        productAVL.insert(products[i]);
        //cout << count << endl;
        //count ++;
    }
    qDebug() << "AVLProduct creado";
}

Home::~Home()
{
    delete ui;
}

void Home::on_pushButton_clicked()
{
    QString sql = ui->sentenciaSQL->text();
    string ss = sql.toStdString();

    // SELECT * FROM Payments WHERE order_id = cf55509ea8edaaac1d28fdb16e48fc22
    if (sql.startsWith("SELECT")) {
        int pos1 = ss.find("FROM ") + 5;
        int pos2 = ss.find(" ", pos1);
        string tabla = ss.substr(pos1, pos2 - pos1);

        // Extraer los valores de búsqueda de la sentencia SQL
        pos1 = ss.find("WHERE ") + 6;
        if(ss.find("BETWEEN ") != string::npos){
            // SELECT * FROM Payments WHERE order_id BETWEEN 4 AND 12
            pos2 = ss.find(" BETWEEN ");
            string columna = ss.substr(pos1, pos2 - pos1);

            pos1 = pos2 + 9;
            pos2 = ss.find(" AND ", pos1);
            string valor_inicial = ss.substr(pos1, pos2 - pos1);

            pos1 = pos2 + 5;
            pos2 = sql.length();
            string valor_final = ss.substr(pos1, pos2 - pos1);

            qDebug() << tabla;
            qDebug() << columna;
            qDebug() << valor_inicial;
            qDebug() << valor_final;

            if(tabla == "Payments"){
                ui->paymentsTable->setRowCount(0);
                ui->paymentsTable->clearContents();
                vector<Payment> payments_result;
                payments_result = paymentAVL.rangeSearch(valor_inicial,valor_final);
                cout << "A VER AVER" << payments_result[0].getPaymentValue() << endl;
                for (const Payment& payment : payments_result) {
                    int row = ui->paymentsTable->rowCount();
                    ui->paymentsTable->insertRow(row);
                    ui->paymentsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(payment.getPrimaryKey())));
                    ui->paymentsTable->setItem(row,1, new QTableWidgetItem(QString::number(payment.getPaymentSequential())));
                    ui->paymentsTable->setItem(row,2, new QTableWidgetItem(QString::fromStdString(payment.getPaymentType())));
                    ui->paymentsTable->setItem(row,3, new QTableWidgetItem(QString::number(payment.getPaymentInstallments())));
                    ui->paymentsTable->setItem(row,4, new QTableWidgetItem(QString::number(payment.getPaymentValue())));
                }
            }
            else {
                ui->productsTable->setRowCount(0);
                ui->productsTable->clearContents();
                vector<Product> products_result;
                products_result = productAVL.rangeSearch(valor_inicial,valor_final);
                for (const Product& product : products_result) {
                    int row = ui->productsTable->rowCount();
                    ui->productsTable->insertRow(row);
                    ui->productsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(product.getID())));
                    ui->productsTable->setItem(row,1, new QTableWidgetItem(QString::fromStdString(product.getCategory())));
                    ui->productsTable->setItem(row,2, new QTableWidgetItem(QString::number(product.getNameLenght())));
                    ui->productsTable->setItem(row,3, new QTableWidgetItem(QString::number(product.getDescriptionLenght())));
                    ui->productsTable->setItem(row,4, new QTableWidgetItem(QString::number(product.getPhotosLenght())));
                }
            }

        }
        else{
            pos2 = ss.find(" = ", pos1);
            string atributo = ss.substr(pos1, pos2 - pos1);
            string valor = ss.substr(pos2 + 3);
            qDebug() << tabla;
            qDebug() << atributo;
            qDebug() << valor;

            if(tabla == "Payments"){
                ui->paymentsTable->setRowCount(0);
                ui->paymentsTable->clearContents();
                try {
                    Payment payment= paymentAVL.search(valor);
                    int row = ui->paymentsTable->rowCount();
                    ui->paymentsTable->insertRow(row);
                    ui->paymentsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(payment.getPrimaryKey())));
                    ui->paymentsTable->setItem(row,1, new QTableWidgetItem(QString::number(payment.getPaymentSequential())));
                    ui->paymentsTable->setItem(row,2, new QTableWidgetItem(QString::fromStdString(payment.getPaymentType())));
                    ui->paymentsTable->setItem(row,3, new QTableWidgetItem(QString::number(payment.getPaymentInstallments())));
                    ui->paymentsTable->setItem(row,4, new QTableWidgetItem(QString::number(payment.getPaymentValue())));
                }
                catch (const std::runtime_error& e) {
                    QMessageBox::information(this,"SEARCH","El id no existe");
                    Payment payment = Payment();
                }

            }
            else {
                ui->productsTable->setRowCount(0);
                ui->productsTable->clearContents();
                try {
                    Product product_result = productAVL.search(valor);
                    int row = ui->productsTable->rowCount();
                    ui->productsTable->insertRow(row);
                    ui->productsTable->setItem(row,0, new QTableWidgetItem(QString::fromStdString(product_result.getPrimaryKey())));
                    ui->productsTable->setItem(row,1, new QTableWidgetItem(QString::fromStdString(product_result.getCategory())));
                    ui->productsTable->setItem(row,2, new QTableWidgetItem(QString::number(product_result.getNameLenght())));
                    ui->productsTable->setItem(row,3, new QTableWidgetItem(QString::number(product_result.getDescriptionLenght())));
                    ui->productsTable->setItem(row,4, new QTableWidgetItem(QString::number(product_result.getPhotosLenght())));
                }
                catch (const std::runtime_error& e) {
                    QMessageBox::information(this,"SEARCH","El id no existe");
                    Product product = Product();
                }
            }
        }

    }
    else if (sql.startsWith("INSERT")) {
        // INSERT INTO Payments VALUES(769214146682788a92201d8907fa1b50,1,credit_card,4,24.98,);
        QString sql = ui->sentenciaSQL->text();
        // Extraer el nombre de la tabla de la sentencia SQL
        int pos1 = ss.find("INTO ") + 5;
        int pos2 = ss.find(" V", pos1);
        string tabla = ss.substr(pos1, pos2 - pos1);
        string s = "";

        // Extraer los valores de la sentencia SQL
        pos1 = ss.find("(") + 1;
        pos2 = ss.find(")", pos1);
        string valores_str = ss.substr(pos1, pos2 - pos1);

        // Convertir los valores a un vector de strings
        vector<string> valores;
        pos1 = 0;
        pos2 = valores_str.find(",");
        while (pos2 != string::npos) {
            s += valores_str.substr(pos1, pos2 - pos1);
            s += ",";
            pos1 = pos2 + 1;
            pos2 = valores_str.find(",", pos1);
        }

        if(tabla == "Payments"){
            istringstream ss(s);
            string order_id, payment_type;
            int payment_sequential, payment_installments;
            float payment_value;
            char cstr[33], ctr[11];

            getline(ss >> std::ws, order_id, ',');
            cout << order_id << endl;
            std::strncpy(cstr, order_id.c_str(), sizeof(cstr) - 1);
            cstr[sizeof(cstr) - 1] = '\0';
            ss >> payment_sequential;
            ss.ignore();
            getline(ss, payment_type, ',');
            std::strncpy(ctr, payment_type.c_str(), sizeof(ctr));
            ss >> payment_installments;
            ss.ignore();
            ss >> payment_value;

            Payment newPayment(cstr, payment_sequential, ctr, payment_installments, payment_value);
            qDebug() << "esta es la  llave que inserte" << newPayment.getPrimaryKey();
            IntPayment newIntPayment(intpayments.size(),payment_sequential, ctr, payment_installments, payment_value);
            qDebug() << "esta es la  llave que inserte" << newIntPayment.getPrimaryKey();
            payments.push_back(newPayment);
            intpayments.push_back(newIntPayment);
            seqFileIntPayment.add(newIntPayment);
            paymentAVL.insert(newPayment);
        }

        else{
            istringstream ss(s);
            string product_id, category;
            float name_length, description_length, photos;
            char cstr[33], ctr[50];

            getline(ss, product_id, ',');
            std::strncpy(cstr, product_id.c_str(), sizeof(cstr) - 1);
            cstr[sizeof(cstr) - 1] = '\0';
            getline(ss, category, ',');
            std::strncpy(ctr, category.c_str(), sizeof(ctr));
            ss >> name_length;
            ss.ignore();
            ss >> description_length;
            ss.ignore();
            ss >> photos;

            Product newProduct(cstr, ctr, name_length, description_length, photos);
            IntProduct newIntProduct(intproducts.size(), ctr, name_length, description_length, photos);
            products.push_back(newProduct);
            intproducts.push_back(newIntProduct);
            seqFileIntProduct.add(newIntProduct);
        }

    }
    else if (sql.startsWith("DELETE")) {
        // Extraer el nombre de la tabla de la sentencia SQL
        int pos1 = ss.find("FROM ") + 5;
        int pos2 = ss.find(" ", pos1);
        string tabla = ss.substr(pos1, pos2 - pos1);
        // Extraer los valores a eliminar de la sentencia SQL
        pos1 = ss.find("= ") + 2;
        pos2 = sql.length();
        string id = ss.substr(pos1, pos2 - pos1);

        qDebug() << tabla;
        qDebug() << id;
        if(tabla == "Payments"){
            paymentAVL.remove((char*)&id);
            for(auto p:payments) if(p.getPrimaryKey() == id) p = Payment();
            //for(auto p:intpayments) if(p.getPrimaryKey() == id) p = IntPayment();
        }
        else {
            productAVL.remove((char*)&id);
            for(auto p:products) if(p.getPrimaryKey() == id) p = Product();
            //for(auto p:intproducts) if(p.getPrimaryKey() == id) p = IntProduct();
        }
    } else {
        QMessageBox::information(this,"SQL","Sentencia invalida");
    }
}


void Home::on_pushButton_3_clicked()
{
    //vector<Payment> payments = readPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv");
    ui->paymentsTable->setRowCount(0);
    ui->paymentsTable->clearContents();
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
    ui->productsTable->setRowCount(0);
    ui->productsTable->clearContents();
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

