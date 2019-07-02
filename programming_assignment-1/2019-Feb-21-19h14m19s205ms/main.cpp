// AlgorithmsAssignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.

/*
* @author: Himanshu Kohli
* @version: 1.0
* @date: 20 Feburary 2019
*/

#include <iostream>
#include<fstream>
#include<vector>
using namespace std;

int noOfVertices;	// no of Vertex
int leastSum = 0;	// Least sum

/*
* This method inserts the undirected edge in the graph
* @param adj : adjancy list
* @param source : vertex 1
* @param dest: vertex 2
* @param weight: weight of the edge
*/

void addEdge(vector <pair<int, int> > *adj, const int source, const int destination, const int weight)
{
	adj[source].push_back(make_pair(destination, weight));
	adj[destination].push_back(make_pair(source, weight));
}

/*
* This method finds if the adjancy list has the vertex
* @param adj : adjancy list
* @param source : list of that particular vertex
* @param find: The vertex you want to find
* @param weight: weight of the edge
* @return: true(if it has edge), false(if it doesn't)
*/
bool hasEdge(const vector<pair<int, int>> *adj, const int source, const int find) {
	int v;
	if (source <= noOfVertices) {
		for (auto it = adj[source].begin(); it != adj[source].end(); it++)
		{
			v = it->first;
			if (v == find) {
				return true;
			}
		}
	}
	return false;
}

/*
* This method returns the weight of the edge
* @param adj : adjancy list
* @param source : vertex 1
* @param dest: vertex 2
* @return: weight of the edge
*/
int getWeight(const vector<pair<int, int>> *adj, const int source, const int dest) {
	int vertex, weight;
	for (auto it = adj[source].begin(); it != adj[source].end(); it++)
	{
		vertex = it->first;
		weight = it->second;
		if (dest == vertex) {
			return weight;
		}
	}
	return 0;
}

/*
* This method assign the least weight to least sum
* @param adj : adjancy list
*/
void findLeastWeight(const int *path, const vector<pair<int, int>> *adj) {
	int sum = 0;

	for (int i = 0; i < noOfVertices; i++) {
		if (i != noOfVertices - 1)
			sum += getWeight(adj, path[i], path[i + 1]);
		else
			sum += getWeight(adj, path[0], path[i]);
	}

	if (sum != 0) {
		if (leastSum == 0) {
			leastSum = sum;
		}
		else if (leastSum > sum) {
			leastSum = sum;
		}
	}

}

/*
* This method finds the cycle
* @param adj : adjancy list
* @param vertex : current vertex
* @param visited: the visited array
* @param path: the path array
* @param position: the current posiion on path array
*/
void findCycles(const vector<pair<int, int>> *adj, const int vertex, bool *visited, int *path, const int position) {

	// For last postion to adding
	if (position == noOfVertices) {
		if (hasEdge(adj, path[position - 1], path[0]))
			findLeastWeight(path, adj);
		return;
	}

	// For all the rest of the positons
	for (auto w : adj[vertex])
	{
		int value = w.first;
		if (!visited[value])
		{
			visited[value] = true;
			path[position] = value;

			findCycles(adj, value, visited, path, position + 1);

			visited[value] = false;
			path[position] = -1;
		}
	}
}

/*
* This initializes the values
* @param adj : adjancy list
*/
void hamiltonianCycle(vector<pair<int, int>> *adj) {

	int *path = new int[noOfVertices];
	bool *visited = new bool[noOfVertices];

	// Mark all paths negative and all visited to false
	for (int i = 0; i < noOfVertices; i++) {
		path[i] = -1;
		visited[i] = false;
	}

	// add starting node to path and set visited to true
	int startNode = 0;
	path[0] = startNode;
	visited[0] = true;

	findCycles(adj, startNode, visited, path, 1);
}

/*
* This method finds the cycle
* @param input_file : path of input file
* @param output_file: path of output file
*/
void parseFile(const string input_file,const string output_file) {
	int no_of_graphs, edges, vertex;

	ifstream in(input_file);

	if (!in.is_open()) {
		cout << "Not opened" << endl;
		exit(0);
	}

	ofstream out;
	out.open(output_file);

	vector<pair<int, int> > *adj = NULL;
	in >> no_of_graphs;
	for (int graph = 0; graph < no_of_graphs; graph++) {
		in >> vertex >> edges;
		noOfVertices = vertex;
		adj = new vector<pair<int, int> >[noOfVertices];

		int source, dest, weight;
		for (int i = 0; i < edges; i++) {
			in >> source >> dest >> weight;
			addEdge(adj, source, dest, weight);
		}

		// Writing file
		hamiltonianCycle(adj);
		out << "Min Cycle weights for " << graph + 1 << " : " << leastSum << endl;
		leastSum = 0;
	}
	in.close();
	out.close();
	cout << "output is in the output file" << endl;


	//deallocating memory ===> garbage collection
	adj = NULL;
	delete adj;


}
/*
* Main cycle of the graph
* @param input_file : path of input file
* @param output_file: path of output file
*/
int main(int argc, char* argv[])
{
	parseFile(argv[1], argv[2]);
	return 0;
}
