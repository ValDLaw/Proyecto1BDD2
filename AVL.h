//
// ENZO, ponlo aquí <3
//

#ifndef PROYECTO1BDD2_AVL_H
#define PROYECTO1BDD2_AVL_H
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>

using namespace std;

/// SINO FUNCIONA
// CAMBIAR STOI con ATOI

/*
struct Record
{
    char cod[5] ;
    char nombre[20];
    int ciclo;

    Record() = default;
    Record(char n[5]  , char c[20] , int semester){
        std::copy_n(n,5,cod);
        std::copy_n(c, 20, nombre);
        ciclo = semester;
    }
    void setData(){
        cout << "Codigo:  "; cin >> cod ;
        cout << "Nombre: "; cin >> nombre ;
        cout << "Ciclo: "; cin >> ciclo;
    }
    string getKey(){
        return cod ;
    }
};

ostream& operator<<(ostream& res, Record& record){
    res << "\n";
    res << "Codigo: " << record.cod << ",  ";
    res << "Nombre: " << record.nombre << ",  ";
    res << "Ciclo: " << record.ciclo;
    res << endl;
    return res;
}

class AVLFile {
private:

    struct NodeBT {
        Record data;
        long left;
        long right;
        int height=0;
        long pos = 0;
        bool del = false;
        NodeBT() {
            left = right = -1;
            height = 0;
        }

        NodeBT(Record &record) {
            data = record;
            left = right = -1;
            height = 0;
        }
    };

    long root;
    string filename;

public:
    AVLFile(std::string ifilename){
        filename =  ifilename;
        fstream inFile;
        inFile.open(filename,ios::in | ios::binary);
        inFile.seekp(0, std::ios_base::end);
        int L  = inFile.tellp();
         root = (L ==-1)?-1:0;
        inFile.close();
    }

    NodeBT getNode( fstream& inFile , long NodeTTemp){
        inFile.seekg(NodeTTemp);
        NodeBT nodeBt;
        inFile.read((char*)&nodeBt, sizeof (nodeBt));
        return nodeBt;
    }

    void insert(Record record1) {
        ofstream (filename, std::ofstream::app | std::fstream::binary);
        fstream inFile(filename,  std::ios::binary |  ios::in | ios::out);
        if(!inFile.is_open()){
            cout << "NO ABIERTO" << endl;
        }
        inFile.seekp(0, std::ios_base::end);
        long pos = inFile.tellp();
        NodeBT nodeBt(record1);
        nodeBt.pos = pos;
        inFile.write((char *) &nodeBt, sizeof(nodeBt));
        if (root == -1) {
            root = 0;
        }else {
            insert(inFile, root, pos, root, record1);
        }

        inFile.close();
    }

    void insert(fstream &inFile, long NodoActual, long NuevoNodo ,long posParent ,Record &record ){
        if(NodoActual == -1 ){
            NodeBT temp1 = getNode( inFile ,posParent );
            (stoi(record.cod) < stoi(temp1.data.cod) ? temp1.left : temp1.right) = NuevoNodo;
            inFile.seekp(posParent);
            inFile.write((char*)& temp1, sizeof (temp1));
            return;
        }
        else {
            NodeBT temp = getNode(inFile ,NodoActual);
            if(stoi(record.cod) < stoi(temp.data.cod)){
                insert(inFile, temp.left , NuevoNodo, NodoActual, record);
            }else{
                insert(inFile, temp.right, NuevoNodo, NodoActual , record);
            }
        }
        updateHeight( inFile,  NodoActual);
        balance(inFile, NodoActual);
    };

    int height( fstream& File , long &node) {
        if (node == -1) {
            return -1;
        }

        NodeBT temp = getNode( File ,node);
        return temp.height;
    }

    int balancingFactor(fstream &File , long Node){
        NodeBT temp = getNode( File , Node);
        return (height(File , temp.left) - height(  File , temp.right));
    }

    void change(fstream& File,  long posNode, long right_node, bool flag, bool doubleRotate){
        // true if is leftRotate
        NodeBT h2 = getNode(File, right_node);
        if(this->root == posNode){
            NodeBT h1 = getNode(File, root);

            long al2 = h2.height;
            long pos1 = h1.pos;
            long pos2 = h2.pos;

            File.seekp(pos2); // pos 0
            h1.pos = h2.pos; // pos 48 = 0
            if(doubleRotate) { h1.height += 1; }
            (flag ? h1.left : h1.right) = pos1;
            File.write((char*)& h1 , sizeof(h1));

            File.seekp(pos1); // pos 48
            h2.height = al2; // pos 1 = 0
            h2.pos = pos1; // pos 0 =
            File.write((char*)& h2 , sizeof(h2));
        }else{
            NodeBT h1 = getNode(File, posNode);
            long pos1 = h1.pos;
            long pos2 = h2.pos;

            File.seekp(pos2); // pos 96
            h1.pos = h2.pos; // pos 48 = 0
            if(doubleRotate) { h1.height += 1; }
            (flag ? h1.left : h1.right) = pos1;
            File.write((char*)& h1 , sizeof(h1));
            File.seekp(pos1); // pos 48
            h2.pos = pos1; // pos 0 =
            File.write((char*)& h2 , sizeof(h2));
        }

    }

    void leftRota(fstream &File, long  posNode, bool doubleRotate){
        NodeBT Node = getNode(File, posNode);
        long right_node = Node.right;
        NodeBT NodeRight = getNode( File , right_node);
        Node.right = NodeRight.left;
        NodeRight.left = Node.pos;

        File.seekg(NodeRight.pos);
        File.write((char*)&Node, sizeof(Node));

        updateHeight( File , posNode);
        updateHeight( File , NodeRight.pos);

        File.seekp(Node.pos);
        File.write((char *) &NodeRight, sizeof(NodeRight));

        change(File, posNode, right_node, true, doubleRotate);

    }

    void rightRota( fstream &File ,long posNode, bool doubleRotate){

        NodeBT Node = getNode(File, posNode);
        long left_node = Node.left;
        NodeBT NodeLeft = getNode( File , left_node);
        Node.left = NodeLeft.right;
        NodeLeft.right = Node.pos;

        File.seekg(NodeLeft.pos);
        File.write((char*)&Node, sizeof(Node));

        updateHeight( File , posNode);
        updateHeight( File , NodeLeft.pos);

        File.seekp(Node.pos);
        File.write((char *) &NodeLeft, sizeof(NodeLeft));

        change(File, posNode, left_node, false,  doubleRotate);

    }

    void balance( fstream &File, long &Node){
        int  i = balancingFactor(File, Node);
        NodeBT dataNode = getNode( File , Node);
        if(i >= 2){
            if(balancingFactor( File , dataNode.left) <= -1){
                leftRota(File,dataNode.left, true);
            }
            rightRota(File, Node, false);
        }else if(i <= -2){
            if(balancingFactor(File,dataNode.right) >= 1){
                rightRota(File ,dataNode.right, true);
            }
            leftRota(File, Node, false);
        }
    }


    void updateHeight( fstream &inFile , long pos) {
        if (pos == -1) {
            return ;
        }
        else{
            NodeBT temp1 = getNode(inFile, pos);
            temp1.height = max(height(inFile, temp1.left), height(inFile, temp1.right) ) + 1;
            inFile.seekp(pos);
            inFile.write((char *) &temp1, sizeof(temp1));
        }
    }


    void remove(string value){
        ofstream (filename, std::ofstream::app | std::fstream::binary);
        fstream inFile(filename,  std::ios::binary |  ios::in | ios::out);
        if(!inFile.is_open()){
            cout << "NO ABIERTO" << endl;
        }
        inFile.seekg(0, std::ios_base::beg);
        remove(inFile, root, root , value);
    }

    void remove(fstream& File, long NodoActual, long Parent, string value) {
        NodeBT node = getNode(File, NodoActual);
        if(NodoActual == -1){
            return;
        }
        else if(stoi(value) < stoi(node.data.cod) ){

            remove(File, node.left , NodoActual , value);
            updateHeight(File, NodoActual);
            balance(File, NodoActual);

        }else if(stoi(value) > stoi(node.data.cod) ){

            remove(File, node.right , NodoActual, value);
            updateHeight(File, NodoActual);
            balance(File, NodoActual);
        }
        else{
            NodeBT nodeParent = getNode(File, Parent );
            if(node.left == -1 && node.right == -1){
                if(NodoActual == nodeParent.left){
                    nodeParent.left = -1;
                } else {
                    nodeParent.right = -1;
                }
                node.del = true;
                File.seekg(Parent);
                File.write((char*)&nodeParent, sizeof(nodeParent));
                File.seekg(NodoActual);
                File.write((char*)&node, sizeof(node));

            }else if(node.left == -1){
                NodeBT nodoHijo = getNode(File, node.right);
                node = nodoHijo;
                node.pos = NodoActual;
                nodoHijo.del = true;
//                nodeParent.height -= 1;
                File.seekg(NodoActual);
                File.write((char*)&node, sizeof(node));
                File.seekp(nodoHijo.pos);
                File.write((char*)&nodoHijo, sizeof(nodoHijo));
            }else if(node.right == -1){
                NodeBT nodoHijo = getNode(File, node.left);
                node = nodoHijo;
                node.pos = NodoActual;
                nodoHijo.del = true;
//                nodeParent.height -= 1;
                File.seekg(NodoActual);
                File.write((char*)&node, sizeof(node));
                File.seekp(nodoHijo.pos);
                File.write((char*)&nodoHijo, sizeof(nodoHijo));
            }else{
                string temp = maxValue(File,node.left);
                temp.resize(5);
                char dat[5];
                strcpy(dat, temp.c_str());
                copy_n(dat,5,node.data.cod);
                File.seekp(NodoActual);
                File.write((char*)& node, sizeof(node) );
                remove(File, node.left, NodoActual, dat);
            }
        }
    }

    string maxValue(fstream &File, long NodoActual ){
        NodeBT nodo = getNode(File, NodoActual);
        if(NodoActual == -1){
            throw invalid_argument("Esta Vacio");
        }else if(nodo.right == -1){
            return nodo.data.cod;
        }else{
            return maxValue(File, nodo.right);
        }
    }

    void scanAll(){
        fstream inFile(filename, std::ios::in  | std::ios::binary);
        inFile.seekg(0);
        if(!inFile.is_open()){ cout << "NO ABIERTO" << endl;}
        while( inFile.peek()  ,  !inFile.eof()){
            NodeBT a1{};
            inFile.read( (char *)& a1, sizeof (a1));
            inFile.clear();
            if(!a1.del) {
                cout << "POS " << a1.pos;
                cout << a1.data;
                cout << " Pos Left : " << a1.left << "  Pos right : " << " " << a1.right << " Heigh : " << a1.height;
                cout << endl << endl;
            }
        }
        inFile.close();
    }

    Record search(string value)
    {
        std::ifstream file(this->filename, std::ios::binary);
        value.resize(5);
        Record result = search(file, root, value);
        file.close();
        return result;
    }

    static Record search(std::ifstream &file, long record_pos,string value)
    {
        if (record_pos == -1)
            throw "Archivo Vacio";
        else {
            NodeBT temp;
            file.seekg(record_pos);
            file.read((char*)&temp, sizeof(NodeBT));
            if ( stoi(value) < stoi(temp.data.cod))
                return search(file, temp.left, value);
            else if (stoi(value) > stoi(temp.data.cod))
                return search(file, temp.right, value);
            else
                return temp.data;
        }
    }

};

void writeFile(string filename){
    AVLFile file(filename);

    file.insert({"5","Julia",7});
    file.insert({"6","Azul",2});
    file.insert({"7","Marco",2});
    file.insert({"1","Arturo",3});
    file.insert({"2","ENZO",5});
    file.insert({"62","ENZO",5});
    file.insert({"89","Olga",8});
    file.insert({"3","Olga",8});
    file.insert({"4","Andres",6});
    file.insert({"15","Arturo",3});
    file.insert({"33","Andres",6});



}

void readFile(string filename)
{
    AVLFile file(filename);
    cout<< endl << "--------SHOW DATA------------\n"<<endl;
    file.scanAll();
    cout << endl <<"--------SEARCH------------\n " << endl;
    Record record = file.search("15");
    cout  << record << endl;
    cout << endl <<"--------REMOVE6------------\n " << endl;
    file.remove("6");
    file.scanAll();
    cout << endl <<"--------REMOVE1------------\n " << endl;
    file.remove("1");
    file.scanAll();
    cout << endl <<"--------REMOVE15------------\n " << endl;
    file.remove("15");
    file.scanAll();
    cout << endl <<"--------REMOVE4------------\n " << endl;
    file.remove("4");
    file.scanAll();

}

int main(){

    writeFile("data.dat");
    readFile("data.dat");
}
 */


#endif //PROYECTO1BDD2_AVL_H
