#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "Graph.hpp"

using namespace std;

const int MAX = 4000;

int store[MAX], n;

int d[MAX];

int numOfIters = 0;

// funckcija koja proverava da li je trenutni podskup klik velicine cliqueSize
bool is_clique(Graph g, int cliqueSize)
{
	numOfIters++;
	std::vector<std::vector<int>> adjMatrix = g.getAdjacencyMatrix();

	for (int i = 1; i < cliqueSize; i++) {
		for (int j = i + 1; j < cliqueSize; j++)

			// ukoliko bilo koja grana nedostaje mozemo da vratmo
			// false odmah
			if (adjMatrix[store[i]][store[j]] == 0)
				return false;
	}

	// ako nigde nismo vratili false, vracamo true
	return true;
}

int maxCliquesBF(Graph g, int startNode, int newSize)
{
	int maxClique = 0;
	int size = g.getNodeCount();

	// proveravamo da li mozemo da dodamo bilo koji čvor tako da
	// prosimo podskup i da on i dalje bude klik
	// krecemo od cvora koji smo prosledili kao pocetni i proveramo sve
	// dalje
	for (int newNode = startNode + 1; newNode <= size; newNode++) {
		store[newSize] = newNode;

		if (is_clique(g, newSize)) {
			maxClique = max(maxClique, newSize);
			maxClique = max(maxClique,
					maxCliquesBF(g, newNode, newSize + 1));
		}
	}

	return maxClique;
}

int main()
{
	std::filesystem::path dataPath("./data");
	for (const auto& dirEntry :
	     std::filesystem::directory_iterator(dataPath)) {
		if (dirEntry.path().extension() == ".mtx") {
			cout << "Loading graph " << dirEntry.path().stem()
			     << " from file  ...... " << flush;
			Graph g(dirEntry.path());
			// std::cout << g << std::endl;
			cout << "Done" << endl;

			auto start = std::chrono::high_resolution_clock::now();

			int maxCliqueSize = maxCliquesBF(g, 0, 1);

			auto end = std::chrono::high_resolution_clock::now();
			chrono::duration<double> diff = end - start;

			cout << "Found clique of size " << maxCliqueSize
			     << " in " << diff.count() << " seconds" << endl
			     << endl;
		}
	}

	return 0;
}