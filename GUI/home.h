#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include "Payment.h"
#include "IntPayment.h"
#include "IntProduct.h"
#include "Product.h"
#include "Sequential.h"
#include "AVL.h"
#include "HashIndex.h"

namespace Ui {
class Home;
}

class QStandardItemModel;

class Home : public QDialog
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = nullptr);
    ~Home();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Home *ui;
    QStandardItemModel *mModel;
    vector<Payment> payments;
    vector<IntPayment> intpayments;
    vector<Product> products;
    vector<IntProduct> intproducts;
    SequentialFile<IntPayment> seqFileIntPayment;
    SequentialFile<IntProduct> seqFileIntProduct;
    HashIndex<Product, const char *, ProductHash> productHash;
    HashIndex<Payment, const char *, PaymentHash> paymentHash;
    AVL<Payment> paymentAVL;
    AVL<Product> productAVL;
};

#endif // HOME_H
