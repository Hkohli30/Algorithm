#include <iostream>
#include<vector>
#include<iostream>
#include<bits/stdc++.h>
#include<map>
#include <string>
#include<fstream>
using namespace std;

const int cost_elements = 10000 + 1;

int cost[cost_elements];

void initalizeInputs(ifstream &cost_input) {
	int temp;
	for (int i = 0; i <= 10000; i++) {
		cost_input >> temp;
		cost_input >> cost[i];
	}
}

void preformDP(int iterations, int credit, int balance, ofstream &out_file, bool flag) {

	if (balance == 0) {
		int sum = 0;
		for (int i = 0; i < iterations; i++) {
			sum += cost[credit];
		}
		out_file << sum;
		for (int i = 0; i < iterations; i++)
			cout << credit << " ";
	}
	else {
		map<int, int> myMap;
		map<int, int> backtrack;

		for (int x = 0; x <= credit*iterations; x++) {
			myMap[x] = INT_MAX;
			backtrack[x] = -1;
		}

		int upper = credit + balance;
		for (int x = 0; x <= credit; x++) {
			myMap[x]= cost[x];
			//backtrack.insert(pair<int, int>(x, x));
		}


		for (int i = 1; i <= iterations; i++) {
			int lowerBound = (credit * i) - balance, upperBound = (credit * i) + balance;
			int midBound = credit * i;

			if (lowerBound < 0)		// lower bound must not be negative
				lowerBound = 0;
			if (upperBound > (credit * iterations))	// upperbound cannot exceed the range
				upperBound = credit * iterations;

			if (i == iterations) {		// for last iteration the loop will run untill the  middle value
				upperBound = midBound;
			}

			for (int j = lowerBound; j <= upperBound; j++) {

				// last iteration
				//if (i == iterations - 1) {
				//	int iterationHigh = (credit * i) + balance , iterationMid = (credit * i);
				//	int sum = INT_MAX;
				//	int holder = 0;

				//	for (int k = iterationMid; k <= iterationHigh; k++) {
				//		int kCost = INT_MAX;
				//		int valCost1 = 0;
				//		auto mapIterator = myMap.find(k);
				//		if (mapIterator != myMap.end()) {
				//			kCost = mapIterator->second;
				//			valCost1 = mapIterator->first;
				//		}
				//		int alreadyPaid = j - k;
				//		auto mapIterator2 = myMap.find(alreadyPaid);
				//		if (mapIterator2 != myMap.end()) {
				//			kCost = kCost + mapIterator2->second;
				//			holder = mapIterator2->first;
				//		}
				//		else
				//			kCost = INT_MAX;

				//		int sumVal = valCost1 + holder;
				//		if (sum > kCost)	//&& sumVal >= credit * (i + 1)) 
				//		{								// update to least cost
				//			sum = kCost;
				//		}

				//	}
				//	if (holder == 0)
				//		holder = j;

				//	backtrack.insert(pair<int, int>(j, holder));
				//	myMap.insert(pair<int, int>(j, sum));
				//}
				// middle iterations
				//else {
					// calculate previous values
					int sum = INT_MAX;
					int holder = 0;
					int difference = 0;
					if (j > midBound)
						difference = j - midBound;

					if (difference < 0 || i==2)
						difference = 0;

					int iterationHigh = (credit * (i - 1)) + balance - difference;
					int iterationMid = credit * (i - 1);
					iterationMid = (iterationMid - difference) < 0 ? 0 : (iterationMid - difference);

					for (int k = iterationMid; k <= iterationHigh; k++) {
						int kCost = INT_MAX;
						int value1 = 0;
						
						int amountToBePaid = j - k;				// 94

						if (amountToBePaid >= 0 && amountToBePaid <= (credit+balance)) {
							int temp = myMap[k] + cost[amountToBePaid];
							if (temp < myMap[j]) {
								myMap[j] = temp;
								backtrack[j] = amountToBePaid;
							}
						}

						int sumVal = value1 + holder;

						if (sum > kCost && sumVal >= credit * (i + 1)) {
							sum = kCost;
						}
					}

					if (holder == 0)
						holder = j;
					//backtrack.insert(pair<int, int>(j, holder));
					//myMap.insert(pair<int, int>(j, sum));
				//}
			}
		}

			/*for (auto it = myMap.begin(); it != myMap.end(); it++) {
				cout << it->first << "  :  " << it->second << endl;
			}*/


		/*for (auto it = backtrack.begin(); it != backtrack.end(); it++) {
			cout << it->first << "  :  " << it->second << endl;
		}*/

		vector<int> bTrack;
		int prevbalanace = balance;
		int loop = 0;
		int value = iterations * credit;

		while (loop < iterations) {
			auto it = backtrack.find(value);
			if (it != backtrack.end()) {
				int payment = it->second;
				if (payment == -1)
					break;
				bTrack.push_back(payment);
				value = it->first - it->second;
			}
			loop++;
		};



		for (int x : bTrack) {
			cout << x << " ";
		}

		auto iter = myMap.find(credit*iterations);
		if (iter != myMap.end())
			out_file << iter->second;

	}
	if (flag == false) {
		out_file << endl;
		cout << endl;
	}
}

// main function
int main(int argv,char **argc)
{

	//argv = 4;
	//char a[] = "cost.txt";
	//char b[] = "testin.txt";
	//char c[] = "outtest2.txt";

	//argc[1] = b, argc[2] = c, argc[3] = a;
	ifstream in_cost(argc[3]);
	ifstream in_test(argc[1]);
	ofstream out_file(argc[2]);

	if (argv != 4) {
		cout << "There must be three input values" << endl;
		return -1;
	}

	if (!in_cost || !in_test) {
		cout << "Error Opening file(s)" << endl;
		return -1;
	}

	int no_of_inputs;
	in_test >> no_of_inputs;

	initalizeInputs(in_cost);
	bool flag = false;
	for (int i = 0; i < no_of_inputs; i++) {
		int iterations, credit, balance;
		in_test >> iterations >> credit >> balance;
		if (i == no_of_inputs - 1)
			flag = true;
		preformDP(iterations, credit, balance, out_file, flag);
	}

	in_cost.close();
	in_cost.close();
	out_file.close();
	return 0;
}