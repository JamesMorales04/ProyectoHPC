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
#include <ctime> 
#include <vector>
#include <sstream>
#include <algorithm>

int Tamano = 0;

std::vector<std::vector<int>> readFile() {
	
	std::string line;
	std::vector < int > v;
	std::ifstream fin;
	
	fin.open(("example.txt"));
	int size = 0;
	if (fin.is_open()) {
		while (getline(fin, line)) {
			std::stringstream ss(line);
			while (getline(ss, line, ',')) {
				v.push_back(std::stoi(line));
			}
		}
	}
	fin.close();
	std::vector<std::vector<int>> rows(v.at(0), std::vector<int>(v.at(0)));
	std::vector<int> columns;
	int k = 0, l = 0;
	for (int i = 1; i < v.size(); i++) {
		if(k >= v.at(0)){
			std::vector<int> temp;	
			for(int m = 0; m<columns.size();m++){
				int value = columns.at(m);
				rows[l][m] = value;
			}
			columns.clear();
			k = 0;
			l++;
		}
		int value = v.at(i);
		columns.push_back(value);
		k++;
	}
	Tamano = v.at(0);
	return rows;
}

void saveSolution(std::vector<int> distancia, std::string name, double time)
{
	std::ofstream out(name + ".txt");
	out << "Vertex \t\t Distance from Source\n";

	for (int i = 0; i < Tamano; i++) {

		out << i;
		out << "\t\t";
		out << distancia[i];
		out << "\n";
	}
	out <<"Execution Time: " << time << std::endl;
	out.close();
}

int distanciaMinima(std::vector<int> distancia, std::vector<bool> sptSet)
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < Tamano; v++) {

		if (sptSet[v] == false && distancia[v] <= min) {

			min = distancia[v];
			min_index = v;

		}

	}

	return min_index;
}

void dijkstra(std::vector<std::vector<int>> grafo, int posicionInicial)
{
	unsigned t0, t1;
	t0 = clock();
	std::vector<int> distancia(Tamano);
	std::vector<bool> sptSet(Tamano);


	for (int i = 0; i < Tamano; i++) {
		distancia[i] = INT_MAX;
		sptSet[i] = false;
	}

	distancia[posicionInicial] = 0;
	bool done = false;
	for (int count = 0; count < Tamano - 1; count++) {

		int u = distanciaMinima(distancia, sptSet);

		sptSet[u] = true;

		for (int v = 0; v < Tamano; v++) {
			int value = grafo[u][v];
			if (!sptSet[v] && value && distancia[u] != INT_MAX && distancia[u] + value < distancia[v]) {
				distancia[v] = distancia[u] + value;
			}

		}
		
	}
	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	saveSolution(distancia, "procesador",time);
}

int main()
{

	std::vector<std::vector<int>> grafo = readFile();

	dijkstra(grafo, 0);

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
**T2:** Llamado a funcion encargada de calcular un vertice U que no este en el sptSet y busca el siguiente valor a menor distancia.
**T3:** Se encarga de la actualizacion de la distancia entre los valores adyacentes a U que cumplan con siertas caracteristicas.
**T4:** Almacena la solucion optenida en un archivo de texto.

**Comunicacion:**

![](https://i.imgur.com/SaxzCjN.jpg)

**Aglomeracion:**

**Iniciacion de datos:** T0

**Guardado de datos:** T4

**Procesamiento de datos:** T1,T2,T3

**Cluster de tareas:** [T0 - T1] - [T2] - [T3] - [T4]

**Tiempos de ejecución (sólo tomamos en cuenta el tiempo que se demoró en el algoritmo como tal en nuestro caso solo el método Dijkstra):**
El código secuencial tiene un tiempo de ejecución de entre 0.000300 y 0.00400 segundos, al implementar openMP y MPI logramos reducir este tiempo a un rango mejor de entre 0.000210 y 0.000160 segundos utilizando 2 procesadores y dos hilos, debido a que el algoritmo lo diseñamos para que funcionara con estas características.
**Videos**
Juan Andres Lopez: https://youtu.be/XS69Pw3Q5O4
Nicolas Raigosa: https://youtu.be/fWnfq2FPUfg
James Morales:https://youtu.be/-cZhSxehJrU
