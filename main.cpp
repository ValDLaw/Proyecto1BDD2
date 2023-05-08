#include "Tests/tests.cpp"
#include "Tests/testAVL.cpp"

using namespace std;

int main(){
    auto data = readPayments("/Users/ValDLaw/Documents/GitHub/Proyecto1BDD2/Datasets/olist_order_payments_dataset.csv");
    paymentTest(data,10000);
    paymentTest(data,50000);
    paymentTest(data,100000);
};