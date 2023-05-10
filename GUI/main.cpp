#include "brazilianecommerce.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    BrazilianECommerce w;
    w.show();
    return a.exec();
}
