#include "../AVL.h"
#include "../Sequential.h"
#include "../ExtendibleHash.h"
#include <chrono>
#include <Entities/Payment.h>
#include <Entities/Product.h>

string dataFilePath = "datafile.dat";
string auxFilePath = "auxfile.dat";
string indexFilePath = "indexFile.dat";
string bucketFilePath = "bucketFile.dat";

template<typename Record>
void AVLTest(vector<Record>& data){
    cout << "----------------------------------------" << endl;
    cout << "Start Test " << data.size() <<" - AVL" << endl;
    AVLFile avl(dataFilePath);
    vector<Record> vr = data;

    // Insert
    auto start  = chrono::steady_clock::now();
    for(int i = 0; i < vr.size(); i++){
        avl.insert(vr[i]);
    }
    auto end    = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end-start).count();

    // Search
    start  = chrono::steady_clock::now();
    const char* codigo = vr[100].getPrimaryKey();
    avl.search(codigo);
    end    = chrono::steady_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(end-start).count();
    cout <<"Search duration: " <<duration << endl;
    cout << "Finish Test "<< data.size() <<" - Hash" << endl;
    cout << "----------------------------------------" << endl;
}

template <typename Record=Payment>
vector<Record> getData(string& path, int amount){

}

template <typename Record=Payment>
void PARSERTEST(){
    string filename = "olist_order_payments_dataset.csv";
    vector<int> amount = {10000, 50000, 100000};
    for(int i = 0; i < amount.size(); i++){
        cout <<"-------------------------------------------" << endl;
        vector<Record> data = getData(filename, amount[i]);
        AVLTest(data);
    }
}

int main(){
    PARSERTEST();
    return 0;
}

