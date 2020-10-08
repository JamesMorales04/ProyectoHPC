**Que algoritmo decidimos utilizar**

Para resolver el problema de la busqueda de camino mas corto decidimos utilizar el algoritmo de djikstra debido a que algorimos como prim o kruskal debido a que nos genereraban varias limitaciones.

![](https://i.imgur.com/nWLftyu.png)


**Dijsktra Algoritmo secuencial**

Diseño de algoritmo basico para Dijsktra secuencial, para esto utilizamos el algoritmo basico teorico, y adema utilizaremos como metodo de ingreso una matriz de adyasencia que nos facilitara la paralelizacion mas basica, la vectorial.

```cpp
método Dijkstra(Grafo,origen):
    creamos una cola de prioridad Q
    agregamos origen a la cola de prioridad Q
    mientras Q no este vacío:
        sacamos un elemento de la cola Q llamado u
        si u ya fue visitado continuo sacando elementos de Q    
        marcamos como visitado u
        para cada vértice v adyacente a u en el Grafo:
            sea w el peso entre vértices ( u , v )  
            si v no ah sido visitado:
                Verificacion( u , v , w )

método Verificacion( actual , adyacente , peso ):
    si distancia[ actual ] + peso < distancia[ adyacente ]
        distancia[ adyacente ] = distancia[ actual ] + peso
        agregamos adyacente a la cola de prioridad Q
```


***Implementacion del algoritmo de manera secuencial***

```cpp
#include <limits.h> 
#include <stdio.h> 
#include <fstream>
#include <string>
#include <iostream>
#include <omp.h>

#define Tamaño 5

void saveSolution(int distancia[])
{
	std::ofstream out("archivo.txt");
	out << "Vertex \t\t Distance from Source\n";

	for (int i = 0; i < Tamaño; i++) {
    
		out << i;
		out << "\t\t";
		out << distancia[i];
		out << "\n";
	}
	out.close();
}

int distanciaMinima(int distancia[], bool sptSet[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < Tamaño; v++) {

		if (sptSet[v] == false && distancia[v] <= min) {
			min = distancia[v];
			min_index = v;
		}
	}
	return min_index;
}

void dijkstra(int grafo[Tamaño][Tamaño], int posicionInicial, int destino)
{
	int distancia[Tamaño];
	bool sptSet[Tamaño];

	for (int i = 0; i < Tamaño; i++) {

		distancia[i] = INT_MAX;
		sptSet[i] = false;
	}

	distancia[posicionInicial] = 0;

	for (int count = 0; count < Tamaño - 1; count++) {

		int u = distanciaMinima(distancia, sptSet);

		sptSet[u] = true;

		if (u == destino) {
			break;
		}
        
		for (int v = 0; v < Tamaño; v++) {

			if (!sptSet[v] && grafo[u][v] && distancia[u] != INT_MAX && distancia[u] + grafo[u][v] < distancia[v]) {

				distancia[v] = distancia[u] + grafo[u][v];
			}
		}
	}

	saveSolution(distancia);
}

```

**Dijsktra Algoritmo Paralelo**

Para la paralelizacion del algorimo planteado con anterioridad utilizaremos OpenMP y las metologias presentadas en el MOOC de sistemas paralelos de intel, para realizar estas mejoras es necesario hacer algunos cambios en la estructura basica del codigo y utilizar las facilidades que ofrece la libreria OpenMP por medio de pragmas y mutex, ademas es necesario estructura algunas secciones de codigo para facilitar las operaciones de vectores por medio de vectorizacion.
```cpp
método Dijkstra(Grafo,origen):
    creamos una cola de prioridad Q
    agregamos origen a la cola de prioridad Q
    **Paralelizar el for (omp for)**
    para cada Q no este vacío:
        **Paralelizacion de vector (simd)**
        **Aplicar Race Mutex (atomic)**  
        sacamos un elemento de la cola Q llamado u
        si u ya fue visitado continuo sacando elementos de Q 
        marcamos como visitado u
        **paralelizar el for*
        para cada vértice v adyacente a u en el Grafo:
            **Aplicar Race Mutex (critical)**  
            sea w el peso entre vértices ( u , v )  
            si v no ah sido visitado:
                Verificacion( u , v , w )
**Paralelizacion de vector (simd)**
método Verificacion( actual , adyacente , peso ):
    si distancia[ actual ] + peso < distancia[ adyacente ]
        distancia[ adyacente ] = distancia[ actual ] + peso
        agregamos adyacente a la cola de prioridad Q
```

# Diseño PCAM 

**Particionado:**

![](https://i.imgur.com/SDTCPi0.png)


**T0:** Se encarga de la inicializacion de la variable distancia y sptSet, ademas de esto tambien toma que la distancia hacia la posicion inicial es 0.
**T1:** Iterador principal encargado de realizar todas las operaciones de busqueda de rutas.
**T2:** Llamado a funcion encargada de calcular un vetice U que no este en el sptSet y busca el siguiente valor a menor distancia.
**T3:** Se encarga de la actualizacion de la distancia entre los valores adyacentes a U que cumplan con siertas caracteristicas.
**T4:** Almacena la solucion optenida en un archivo de texto.

**Comunicacion:**

![](https://i.imgur.com/SaxzCjN.jpg)

**Aglomeracion:**

T0:

**Mapeado:**