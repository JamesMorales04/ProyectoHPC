#include <limits.h> 
#include <stdio.h> 
#include <fstream>
#include <string>
#include <iostream>
#include <omp.h>
#include <ctime> 
#include <vector>
#include <sstream>
#include <algorithm>
#include <mpi.h>

int Tamaño = 0;

#pragma omp declare simd
std::vector<std::vector<int>> readFile() {
	
	std::string line;
	std::vector < int > v;
	std::ifstream fin;
	
	/*std::vector<vector<int>> rows;
	std::vector<int> columns;*/
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
#pragma omp simd
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

	Tamaño = v.at(0);
	return rows;
}

#pragma omp declare simd
void saveSolution(std::vector<int> distancia, std::string name, double time)
{
	std::ofstream out(name + ".txt");
	out << "Vertex \t\t Distance from Source\n";

	for (int i = 0; i < Tamaño; i++) {

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

#pragma omp parallel for
	for (int v = 0; v < Tamaño; v++) {

		if (sptSet[v] == false && distancia[v] <= min) {

			min = distancia[v];
			min_index = v;

		}

	}

	return min_index;
}

void dijkstra(std::vector<std::vector<int>> grafo, int posicionInicial, int destino, int procesador)
{
	unsigned t0, t1;
	t0 = clock();
	std::vector<int> distancia(Tamaño);
	std::vector<bool> sptSet(Tamaño);


#pragma omp parallel for
	for (int i = 0; i < Tamaño; i++) {

		distancia[i] = INT_MAX;
		sptSet[i] = false;
	}

	distancia[posicionInicial] = 0;
	bool done = false;
#pragma omp parallel for
	for (int count = 0; count < Tamaño - 1; count++) {

		int u = distanciaMinima(distancia, sptSet);

		sptSet[u] = true;

		if (u == destino) {
			done = true;
			count = Tamaño;
		}
		if (!done){		
#pragma omp parallel for
			for (int v = 0; v < Tamaño; v++) {
				int value = grafo[u][v];
				if (!sptSet[v] && value && distancia[u] != INT_MAX && distancia[u] + value < distancia[v]) {
#pragma omp simd
					distancia[v] = distancia[u] + value;

				}

			}
		}

	}
	t1 = clock();
	double time = (double(t1 - t0) / CLOCKS_PER_SEC);
	saveSolution(distancia, "procesador"+procesador);
}

int main(int argc, char *argv[])
{
	
	int pid, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	std::vector<std::vector<int>> grafo = readFile();
	if (pid <= size/2){
		dijkstra(grafo, 0,3,pid);
	}else{
		dijkstra(grafo, 3,0,pid);
	}
	MPI_Finalize();
	
	return 0;
}


