# Proyecto 1 - Bases de Datos II
Integrantes:
- Valentín Quezada Amour (202120570)
- Valeria Espinoza Tarazona (202110109)
- Sofía García Quintana (202110567)
- Enzo Camizán Vidal (202110047)
- Diego Guerra Chevarría (202010137)

## Introducción

El comercio eléctrónico es un mercado que se encuentra en auge, especialmente en el último siglo debido al amplio desarrollo de nuevas tecnologías y startups. La empresa Olist es una startup, originaria del país brasileño, segmentada hacia el comercio electrónico con especial enfoque en el sistema de marketplace virtual.

![](https://cdn6.aptoide.com/imgs/c/a/f/caf237ba52e019f3264380ccdf550b1d_fgraphic.jpg)

Así como en otros sectores, las startups enfocadas en el comercio electrónico manejan bases de datos inmensas en relación a los registros de pagos realizados y productos en venta. En el curso de Bases de Datos II, hemos estudiado una variedad de estructuras de datos para la organización de registros en archivos, a las cuales podemos dar uso para el ordenamiento de los datasets de Olist.

### Objetivo del proyecto

El objetivo del presente proyecto consiste en estructurar y programar un gestor funcional de bases de datos con GUI implementada, a fin de administrar los datasets de pagos y productos de Olist. El gestor contará con operaciones de inserción, eliminación y búsqueda (específica y por rango), accediendo a las estructuras del tipo **AVL File**, **Sequential File** y **Extendible Hash** según sea más óptimo.

### Descripción del dominio de datos a utilizar

El dominio de datos que hemos escogido para nuestro proyecto consiste en dos archivos con registros de Olist: uno que enlista los productos en venta y otro que almacena información sobre los pagos procesados. Tal dataset fue de nuestra elección por la cantidad de registros (superior a 100k) y porque tiene una aplicación interesante y realista en relación al mercado del comercio electrónico.

### Resultados que se esperan obtener

Al desarrollar el gestor de bases de datos, esperamos que sea capaz de seleccionar la estructura más óptima para las operaciones de inserción, eliminación y búsqueda respectivamente, y que las estructuras programadas cumplan con sus funcionalidades sin soltar errores y en tiempos de ejecución aceptables. También esperamos implementar un parser SQL completamente funcional con una GUI decente que acompañe para insertar las consultas.

## Técnicas utilizadas

### Consideraciones iniciales
- Trabajamos con dos clases de Records: ```Payment.h``` y ```Product.h```, relativas a cada uno de nuestros datasets.
- Adicionalmente, agregamos dos clases de Records con la llave principal cambiada de tipo ```char``` a tipo ```int``` para facilitar la operación de búsqueda en las estructuras. Estas clases fueron ```intPayment.h``` y ```intProduct.h```.
- Las estructuras implementadas utilizaron templates para distinguir entre ambos tipos de Records.
- Cada Record se ordenó por un atributo que cumplió la función de llave primaria: order_id para la clase Payment y product_id para la clase Product.
```ruby
struct Payment {
    char order_id[32];
    int payment_sequential;
    char payment_type[11];
    int payment_installments;
    float payment_value;
    int nextDel;
    char reference;
};

struct Product {
    char product_id[32];
    char category[50];
    int product_name_length;
    int product_description_length;
    int product_photos;
};
```

### AVL File
En primera instancia, implementamos la estructura del árbol AVL para la organización de registros en forma binaria. Esta clase cuenta con una estructura llamada NodeBT para cada nodo perteneciente al árbol. La característica principal del árbol AVL es que, además de cumplir con las propiedades básicas de un árbol binario, se va rebalanceando con cada inserción u eliminación.
```ruby
template<typename Record>
class AVL {
    struct NodeBT;
    long root;
    string filename;
    int numberAccesMemory = 0;
};
```
Una ventaja evidente de usar el AVL para la organización de registros es la complejidad de sus funciones, siendo la búsqueda de carácter O(logn) y la inserción/eliminación también, en su mejor caso, pues requieren de ubicar el registro con anterioridad. No obstante, la constante reconstrucción del árbol supone un costo adicional que podría suponer una desventaja. 

#### Inserción
```ruby
void insert(Record record1);
void insert(fstream &inFile, long NodoActual, long NuevoNodo ,long posParent ,Record record);
```
- Si el archivo de datos se encuentra vacío, se inserta un nuevo nodo en la posición de raíz. Sino, se llama recursivamente a la segunda función ```insert()``` para buscar la posición de inserción.
- Se inserta el nuevo nodo en la posición encontrada (O(logn)).
- Una vez insertado el nodo, se actualiza la altura del árbol con la función ```updateHeight()```.
- Finalmente, se rebalancea el AVL con la función ```balance()``` (O(k\*logn)).

#### Eliminación
```ruby
void remove(string value);
void remove(fstream& File, long NodoActual, long Parent, string value);
```
- Se llama recursivamente a la segunda función ```remove()``` para buscar el valor y eliminarlo.
- Una vez que se encuentra el nodo a eliminar (O(logn)), procedemos a trabajar en los siguientes casos:
    - Si el nodo es una hoja, se elimina.
    - Si el nodo tiene un solo hijo, se hace un swap entre ambos y se elimina el que queda como hoja.
    - Si el nodo tiene dos hijos, se hace un swap entre el padre y el hijo mayor, y se llama nuevamente a ```remove()``` para el padre intercambiado.
- Después de cualquier caso, se actualiza la altura del árbol con la función ```updateHeight()``` y se rebalancea el AVL con la función ```balance()``` (O(k\*logn), donde k representa el número de hijos debajo).

#### Búsqueda
```ruby
Record search(char value[32]);
vector<Record> rangeSearch(char begin_key[32], char end_key[32]);
```
- **Específica:** Se recorre el árbol hasta encontrar el valor buscado. La complejidad es O(logn) al igual que en un árbol binario.
- **Por rango:** Se reccorre el árbol dos veces para encontrar el valor mínimo y máximo. Todos los nodos entre ambos valores se agregan a un vector de Records. La complejidad es O(logn) al igual que en un árbol binario.

### Sequential File
Por otro lado, implementamos la estructura de Sequential File con apoyo de un archivo auxiliar. La clase cuenta con una estructura llamada SequentialBlock para cada bloque de Record + Índice, además de trabajar con dos archivos a la vez: ```datafile``` y ```auxfile```.
```ruby
template <typename Record>
class SequentialFile{
    struct SequentialBlock;
    string datafile;
    string auxfile;
    int accessMemSec = 0;
    int auxCount = 0;
    int deletedCount = 0;
};
```
Una de las ventajas que ofrece la organización del Sequential File es que mantiene los registros en orden en base a la llave primaria. Su desventaja más clara se encuentra en la complejidad de las operaciones (O(n) en su mayoría). Para la reconstrucción del archivo (rebuild), su complejidad de tiempo es O(nlogn) en el mejor de los casos.

#### Inserción
```ruby
bool add(Record registro);
```
- En caso haya una cantidad k en el auxfile, se hace ```rebuild()```. Caso contrario, empezamos leyendo el último Sequential Block del datafile. Si su key es menor que el key del registro que queremos insertar, colocamos el nuevo registro al final del datafile y actualizamos los punteros correspondientes.
- Si esto no ocurre, se recorre los punteros del archivo tanto en datafile como auxfile hasta encontrar la posición donde debería insertarse. Se modifican los punteros correspondiente e insertamos el nuevo registro al final del auxfile.
- En caso el valor esté repetido, se regresa False.

Para la inserción de un registro en el Sequential File, su complejidad de tiempo es en promedio O(n/2) ya que se debe encontrar la posición de inserción correcta en el archivo y, luego, mover todos los registros siguientes para hacer espacio para el nuevo registro. En el peor de los casos, la complejidad de tiempo puede ser O(n) si el nuevo registro se inserta al final del archivo.

#### Eliminación
```ruby
template<typename T>
remove(T key);
```
- Se recorre el datafile y el auxfile utilizando punteros hasta ubicar el valor exacto. 
- En caso se encuentre el key, cambiamos los punteros de su anterior, y el puntero del record eliminado será -2.
- Luego de eso, se reescribe el nodo eliminado y el nodo previo (en "punteros").

#### Búsqueda
```ruby
template<typename T>
vector<Record> search(T key);
```
- **Específica:** Una opción es recorrer el Sequential File por mitades hasta encontrar el valor buscado. La complejidad de este método es O(logn) + O(k), donde k es el tamaño del archivo auxiliar.
- **Por rango:** Una opción es recorrer el Sequential File por mitades hasta ambos valores, máximo y mínimo.  Se insertan todos los registros entre ambos valores en un vector de Records. La complejidad de este método es O(logn) + O(k), donde k es el tamaño del archivo auxiliar.
Primero recorremos el datafile utilizando binary search. En caso se encuentre el archivo, se regresa, ya que el key es único. Si no, se procede a buscar en el auxfile con búsqueda lineal. Si se encuentra, se regresa. Si no, al final de revisar todo el archivo se regresa un vector vacío.

### Extendible Hash
Por último, el Extendible Hash fue implementado y estructurado por tres clases: 
```Bucket.h``` para almacenar los registros en buckets, ```SMH.h``` para las funcionalidades del Secondary Memory Helper (acceso a registros y modificación del datafile) y ```Hashindex.h``` que usa las otras dos para inicalizar el Hash y realizar las operaciones de inserción, eliminación y búsqueda.

```ruby
template <class Record, class Key>
class Bucket{
    Record records[4];
    int nextBucket;
    int count;
    int D = 4;
    int nextDeleted;
}

template<class Data>
struct SMH {
    string filename;
}

template <class Record, class Key, class Hash = hash<Key> >
class HashIndex{
    int MaxHeight;
    typedef Bucket<Record, Key> Bucketcito;
    SMH<TreeNode> IndexData;
    typedef bitset<50> BitsetData;
    SMH<Bucketcito> BucketData;
    Hash myHash;
    int accessMemSec = 0;
}
```

#### Inserción
```ruby
void insert(Record record);
```
- Se lee la llave y se codifica usando la función hash para obtener el valor del módulo.
- Luego de ubicarse en la posición del array, se lee el registro y se verifica si hay espacio o no al final del bucket.
- Si hay espacio, se añade el registro al final del bucket.
- Si no hay espacio, se llama a la función ```split()```:
```ruby
void split(Record& record, int height, int currentNodePosition, TreeNode& currentNode, int currentBucketPosition, Bucketcito& bucket);
```
La función ```split()``` se encarga de crear el nuevo bucket que estará enlazado al anterior, copia el contenido y añade el registro en el nuevo bucket. Su complejidad es O(logn).

#### Eliminación
```ruby
void remove(Key key);
void removeUtil(int nodePosition, Key key, BitsetData hashKey, int& height);
```
- Se lee la llave y se busca su posición dentro de los buckets.
- Se llama a la función privada ```removeUtil()```.
- Si nos encontramos en un bucket hoja y es el último registro, se elimina el registro y el bucket. Caso contrario, continúa la iteración.
- Sino, borramos el registro del bucket. Para borrar los registros no solo del Hash sino de cada bucket, se llama a la función recursiva ```void deleteRecordInBucket(Key key, TreeNode &currentNode, int nodePosition)```:
```ruby
void deleteRecordInBucket(Key key, TreeNode &currentNode, int nodePosition);
```

#### Búsqueda
```ruby
vector<Record> search(Key searchKey)
```
- Se lee la llave y se codifica usando la función hash para obtener el valor del módulo.
- Se itera dentro del bucket correspondiente hasta que la posición se encuentre.
- En caso no se encuentre el registro, la posición valdrá -1 (fin del bucket).
- Iteramos en cada bucket en cada bucket encadenado a los punteros hasta encontrar el elemento y devolverlo
    
### Análisis comparativo teórico
Teóricamente, entendemos que las complejidades de las operaciones del AVL File son menores a las del Sequential File. Por ende, es posible plantear como hipótesis que los tiempos de ejecución de las operaciones de búsqueda en el AVL serán mucho menores. Por otro lado, el Extendible Hash tiene una menor complejidad en la operación de inserción, pero mayor en la operación de búsqueda (su peor caso depende del tamaño del bucket). No obstante, el uso de un archivo auxiliar para el Sequential File le provee un espacio auxiliar de memoria para que no tenga que realizar tantos accesos a la memoria secundaria, por ende teorizamos que la cantidad de accesos del Sequential File será menor a las del AVL File y del Extendible Hash.

| Operación | AVL File | Sequential File | Hash File |
| ------------- | ------------- | ------------- | ------------- |
| Inserción | O(logn) + O(b) | O(logn) + O(k) | O(logn) + O(B) |
| Eliminación | O(logn) + O(b) | O(logn) | O(logn) + O(B) |
| Búsqueda específica | O(logn) | O(logn) + O(k) | O(B) |
| Búsqueda por rango | O(logn) | O(logn) + O(k) | - |

- b = rebalanceo en AVL
- k = cantidad de enlaces en Sequential
- B = tamaño de bucket en Hash

### SQL Parser
Cuenta con dos ventanas principales: Brazilian e-commerce (autenticación) y Home (registros). En la ventana de registros se muestran las tablas de productos y pagos, además de un recuadro para realizar consultas donde se valida si la consulta realizada es correcta y la envía al código fuente. Ya en el código, se distingue entre las operaciones de inserción, eliminación y búsqueda, clasificado de forma que se invoque a la estructura más adecuada para cada caso:
- Inserción: Hash File o AVL File
- Eliminación: AVL File
- Búsqueda: Sequential File

Para la interfaz gráfica, hicimos uso de QTCreator con apoyo de un kit SQL respectivo.


## Resultados experimentales
### Cuadros comparativos
#### Inserción
| Tiempos de ejecución | AVL File | Sequential File | Hash File |
| ------------- | ------------- | ------------- | ------------- |
| 10k datos  | 4358 ms  | 56172 ms | 69487 ms |
| 50k datos | 28882 ms  | 309010 ms | 277877 ms |
| 100k datos | 56397 ms  | 685572 ms | 341957 ms |

![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/1e3f3416-7c55-4e3a-bdc4-75025ee77df5)

| Memoria secundaria | AVL File | Sequential File | Hash File |
| ------------- | ------------- | ------------- | ------------- |
| 10k datos  | 1525157 accesos  | 155751 accesos | 69970 accesos |
| 50k datos | 10232968 accesos  | 275751 accesos | 349970 accesos |
| 100k datos | 20939560 accesos  | 425751 accesos | 699970 accesos |

![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/2071cc6a-a35d-424a-8a48-02ba3b5198a3)

#### Búsqueda
| Tiempos de ejecución | AVL File | Sequential File | Hash File |
| ------------- | ------------- | ------------- | ------------- |
| 10k datos  | 577 µs  | 131 µs | 74058 µs |
| 50k datos | 514 µs  | 382 µs | 218244 µs |
| 100k datos | 1411 µs  | 185 µs | 386692 µs |

![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/46ca6e6f-10d9-48e4-a6e6-9df3f5ae9c5a)

| Memoria secundaria | AVL File | Sequential File | Hash File |
| ------------- | ------------- | ------------- | ------------- |
| 10k datos  | 1226466 accesos  | 12 accesos | 324 accesos |
| 50k datos | 7211248 accesos  | 15 accesos | 1578 accesos |
| 100k datos | 15304605 accesos  | 14 accesos | 3148 accesos |

![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/757b1a7e-450f-495a-b425-f78b0aa7d1b7)

### Discusión y análisis
El desarrollo de las operaciones en cada una de las estructuras difirió en cierta medida de lo teórico, pues se realizó una implementación propia del AVL File, Sequential File y Extendible Hash, ya que hay varias formas de implementar una misma estructura y variantes de cada tipo de archivo. Adicionalmente, se ha comprobado que la inicialización de la interfaz para la visualización de registros en tablas toma un tiempo adicional, y queda en cuestión si realizar tal incialización de manera individual en cada llamada a las operaciones o en conjunto desde la primera pantalla del programa, dependiendo del sistema de base de datos administrado y de los requerimientos de la empresa. Finalmente, se compararon las estructuras de datos entre ellas y sus complejidades para cada operación, evaluando su efectividad en un escenario de la vida real como es el del mercado de comercio electrónico.

## Pruebas de uso
### Interfaz gráfica

![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/5eeca3cf-49c0-4797-9d61-e580c6d5bb7e)
![image](https://github.com/ValDLaw/Proyecto1BDD2/assets/91209653/7d9b2177-3102-439d-b9d8-0711bb327f12)

### Funcionalidad (video)
https://drive.google.com/drive/folders/18U85sToyU2AuTGPAbGuZ3Tm44WlQZ5rW?usp=sharing

¡Muchas gracias!
