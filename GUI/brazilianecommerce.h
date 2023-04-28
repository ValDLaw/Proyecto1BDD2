#ifndef BRAZILIANECOMMERCE_H
#define BRAZILIANECOMMERCE_H

#include <QMainWindow>
#include "home.h"

QT_BEGIN_NAMESPACE
namespace Ui { class BrazilianECommerce; }
QT_END_NAMESPACE

class BrazilianECommerce : public QMainWindow
{
    Q_OBJECT

public:
    BrazilianECommerce(QWidget *parent = nullptr);
    ~BrazilianECommerce();

private slots:
    void on_pushButton_login_clicked();

private:
    Ui::BrazilianECommerce *ui;
    Home *home;
};
#endif // BRAZILIANECOMMERCE_H
