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

El dominio de datos que hemos escogido para nuestro proyecto consiste en dos archivos con registros de Olist: uno que enlista los productos en venta y otro que almacena información sobre los pagos procesados. Tal dataset fue de nuestra elección por la cantidad de registros (superior a 10k) y porque tiene una aplicación interesante y realista en relación al mercado del comercio electrónico.

### Resultados que se esperan obtener

Al desarrollar el gestor de bases de datos, esperamos que sea capaz de seleccionar la estructura más óptima para las operaciones de inserción, eliminación y búsqueda respectivamente, y que las estructuras programadas cumplan con sus funcionalidades sin soltar errores y en tiempos de ejecución aceptables. También esperamos implementar un parser SQL completamente funcional con una GUI decente que acompañe para insertar las consultas.

## Técnicas utilizadas

### Consideraciones iniciales
- Trabajamos con dos clases de Records: Payment.h y Product.h, relativas a cada uno de nuestros datasets.
- Las estructuras implementadas utilizaron templates para distinguir entre ambos tipos de Records.
- Cada Record se ordenó por un atributo que cumplió la función de llave primaria: order_id para la clase Payment y product_id para la clase Product.
```
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
```
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
```
void insert(Record record1);
void insert(fstream &inFile, long NodoActual, long NuevoNodo ,long posParent ,Record record);
```
- Si el archivo de datos se encuentra vacío, se inserta un nuevo nodo en la posición de raíz. Sino, se llama recursivamente a la segunda función ```insert()``` para buscar la posición de inserción.
- Se inserta el nuevo nodo en la posición encontrada (O(logn)).
- Una vez insertado el nodo, se actualiza la altura del árbol con la función ```updateHeight()```.
- Finalmente, se rebalancea el AVL con la función ```balance()``` (O(k\*logn)).

#### Eliminación
```
void remove(string value);
void remove(fstream& File, long NodoActual, long Parent, string value);
```
- Se llama recursivamente a la segunda función ```remove()``` para buscar el valor y eliminarlo.
- Una vez que se encuentra el nodo a eliminar (O(logn)), procedemos a trabajar en los siguientes casos:
    - Si el nodo es una hoja, se elimina.
    - Si el nodo tiene un solo hijo, se hace un swap entre ambos y se elimina el que queda como hoja.
    - Si el nodo tiene dos hijos, se hace un swap entre el padre y el hijo mayor, y se llama nuevamente a ```remove()``` para el padre intercambiado.
- Después de cualquier caso, se actualiza la altura del árbol con la función ```updateHeight()``` y se rebalancea el AVL con la función ```balance()``` (O(k\*logn)).

#### Búsqueda

### Sequential File
#### Inserción
#### Eliminación
#### Búsqueda

### Extensible Hash
#### Inserción
#### Eliminación
#### Búsqueda

### Análisis comparativo teórico

### SQL Parser

## Resultados experimentales
### Cuadros comparativos
#### Inserción
#### Búsqueda

### Discusión y análisis

## Pruebas de uso
### Interfaz gráfica
### Funcionalidad (video)
