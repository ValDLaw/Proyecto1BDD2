#include "brazilianecommerce.h"
#include "./ui_brazilianecommerce.h"
#include <QMessageBox>
#include <QPixmap>

BrazilianECommerce::BrazilianECommerce(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::BrazilianECommerce)
{
    ui->setupUi(this);
    QPixmap pix("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/images/brazil-e-commerce.png"); // cambiar segun la ubicacion de la imagen
    ui->ecommercebrazillogo->setPixmap(pix.scaled(160,160,Qt::KeepAspectRatio));
}

BrazilianECommerce::~BrazilianECommerce()
{
    delete ui;
}

void BrazilianECommerce::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username == "admin" && password == "1234"){
        QMessageBox::information(this,"Login","Login succeded!");
        hide();
        home = new Home(this);
        home->show();
    }
    else {
        QMessageBox::information(this,"Login","Login failed!");
    }
}
