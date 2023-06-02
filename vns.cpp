#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
#include <vector>
#include "Graph.hpp"

bool is_clique(Graph& g, std::set<int>& clique)
{
	std::vector<std::vector<int>> adjMatrix = g.getAdjacencyMatrix();

	for (int u : clique)
		for (int v : clique) {
			if (!adjMatrix[u][v] && u != v) {
				return false;
			}
		}
	return true;
}

float fit(Graph& g, std::set<int>& potential_clique)
{
	std::vector<std::vector<int>> adjMatrix = g.getAdjacencyMatrix();
	int n = potential_clique.size();

	int ukupno = n * (n - 1) / 2;
	int br = 0;

	for (auto it1 = potential_clique.begin(); it1 != potential_clique.end();
	     it1++) {
		auto it2 = it1;
		++it2;
		for (; it2 != potential_clique.end(); it2++) {
			if (adjMatrix[*it1][*it2])
				br++;
		}
	}

	if (ukupno - br != 0)
		return (float)ukupno - br;
	else
		return (float)1 / n;
}
void LS(Graph& g, std::set<int>& s)
{
	bool impr = true;
	float best_fit = fit(g, s);
	std::set<int> s_best = s;
	std::set<int> free_nodes;
	std::set<int> s_new;
	while (impr) {
		impr = false;
		std::set_difference(
		    g.nodeSet.begin(), g.nodeSet.end(), s.begin(), s.end(),
		    std::inserter(free_nodes, free_nodes.begin()));
		for (auto x : s) {
			for (auto y : free_nodes) {
				s_new = s;
				s_new.erase(x);
				s_new.insert(y);
				float new_fit = fit(g, s_new);
				if (new_fit < best_fit) {
					best_fit = new_fit;
					s_best = s_new;
					impr = true;

					break;
				}
			}
		}
		s = s_best;
	}
}

void shaking(Graph& g, std::set<int>& cur_solution, int k, std::mt19937& gen)
{
	std::set<int> free_nodes;
	std::set_difference(g.nodeSet.begin(), g.nodeSet.end(),
			    cur_solution.begin(), cur_solution.end(),
			    std::inserter(free_nodes, free_nodes.begin()));
	while (k > 0) {
		int n = cur_solution.size();
		int n2 = free_nodes.size();

		std::uniform_int_distribution<> distrib(0, n - 1);
		std::uniform_int_distribution<> distrib2(0, n2 - 1);
		auto it = cur_solution.begin();
		auto it2 = free_nodes.begin();

		std::advance(it, distrib(gen));
		int v = *it;
		cur_solution.erase(it);

		std::advance(it2, distrib2(gen));
		cur_solution.emplace(*it2);
		free_nodes.erase(it2);
		free_nodes.emplace(v);

		--k;
	}

	int n2 = free_nodes.size();
	std::uniform_int_distribution<> distrib2(0, n2 - 1);
	auto it2 = free_nodes.begin();
	std::advance(it2, distrib2(gen));
	cur_solution.emplace(*it2);
}

std::set<int> vns(Graph& g, std::uint32_t seed)
{
	std::set<int> best_solution;

	std::mt19937 gen;
	gen.seed(seed);

	int n = g.getNodeCount();
	std::uniform_int_distribution<> distrib(0, n - 1);
	auto it = g.getNodeSet().begin();
	std::advance(it, distrib(gen));
	best_solution.emplace(*it);

	int num = 200;
	// int num=5000;
	int k = 1;

	while (num > 0) {
		n = best_solution.size();
		std::set<int> new_solution = best_solution;
		shaking(g, new_solution, k, gen);
		--num;

		LS(g, new_solution);

		if (fit(g, new_solution) <= fit(g, best_solution)) {
			best_solution = new_solution;
			k = 1;

		} else {
			k++;
			if (k > n / 2) {
				k = 1;
			}
		}
	}
	return best_solution;
}

int main()
{
	std::vector<std::uint32_t> seeds = {
	    997637,   424653,  1856049002, 1129615051, 6460811,
	    10751511, 4678358, 3904109078, 1534123438, 1495905678};

	std::filesystem::path dataPath("./data");

	for (const auto& dirEntry :
	     std::filesystem::directory_iterator(dataPath)) {
		if (dirEntry.path().extension() == ".mtx") {
			std::cout << "Loading graph " << dirEntry.path().stem()
				  << " from file  -> " << std::flush;
			Graph g(dirEntry.path());

			std::cout << "Done" << std::endl;

			float sum = 0;
			double time = 0;
			int numOfIters = 10;
			for (int i = 0; i < numOfIters; i++) {
				auto start =
				    std::chrono::high_resolution_clock::now();
				std::set<int> best_solution =
				    vns(g, seeds.at(i));
				int maxCliqueSize = best_solution.size();

				auto end =
				    std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> diff =
				    end - start;

				std::cout << "Found clique of size "
					  << maxCliqueSize << " in "
					  << diff.count() << " seconds"
					  << std::endl;
				/*  if(is_clique(g,best_solution))
				      std::cout<<"Jeste klika"<<std::endl;
				  else
				      std::cout<<"Nije klika"<<std::endl; */
				sum += maxCliqueSize;
				time += diff.count();
			}
			std::cout << "Average: " << sum / numOfIters
				  << "  Time: " << time / numOfIters
				  << std::endl;
			std::cout << "------------------------------------\n";
		}
	}

	return 0;
}