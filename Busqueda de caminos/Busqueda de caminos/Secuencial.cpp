
#include <limits.h> 
#include <stdio.h> 
#include <fstream>
#include <string>
#include <iostream>
#include <omp.h>

#define Tamaño 9 


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



int main()
{
	int destino = 2;
	int grafo[Tamaño][Tamaño] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
						{ 4, 0, 8, 0, 0, 0, 0, 11, 0 },
						{ 0, 8, 0, 7, 0, 4, 0, 0, 2 },
						{ 0, 0, 7, 0, 9, 14, 0, 0, 0 },
						{ 0, 0, 0, 9, 0, 10, 0, 0, 0 },
						{ 0, 0, 4, 14, 10, 0, 2, 0, 0 },
						{ 0, 0, 0, 0, 0, 2, 0, 1, 6 },
						{ 8, 11, 0, 0, 0, 0, 1, 0, 7 },
						{ 0, 0, 2, 0, 0, 0, 6, 7, 0 } };

	dijkstra(grafo, 3, destino);

	return 0;
}
