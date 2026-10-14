#include <iostream>
#include <fstream>
#include "Cache.h"
#include <cmath>

using namespace std;

int stringToInt(char *);
int runCacheSim(Cache*, int, int, char *);

int main(int argc, char *argv[])
{
	int totalNumEntries = 0;
	int associativity = 0;

	if (argc != 4)
	{
		cout << "Incorrect number of arguments provided! Execution should follow the format: ./exe <number_of_entries> <associativity> <mem_ref_file>";
		return 1;
	}
	
	totalNumEntries = stringToInt(argv[1]);
	if (totalNumEntries < 1)
	{
		cout << "Expected a numeric value greater than zero for <num_entries> but the value passed is not a number!";
		return 1;
	}

	associativity = stringToInt(argv[2]);
	if (associativity < 1)
	{
		cout << "Expected a numeric value greater than 0 for <associativity> but the value passed is not a number!";
		return 1;
	}

	int entriesPerWay = totalNumEntries / associativity; // calculate the actual number of indicies
	Cache *cache = new Cache(entriesPerWay, associativity); // create cache
	int result = runCacheSim(cache, entriesPerWay, associativity, argv[3]); // runs cache
	
	if(result == -1)
		cout << "An error ocurred reading the input file. Exiting...";
	else if (result == -2)
		cout << "An error ocurred writing to output file. Exiting...";
	else if (result == -3)
		cout << "A negative memory value was encountered in the input file. Exiting...";

	delete cache;

	return result;
}

int runCacheSim(Cache* cache, int entriesPerWay, int associativity, char *filename)
{
	ifstream input (filename);//input file name passed from main
	ofstream output ("cache_sim_output"); //file name required by assignment guidelines

	if(!input)// did something with the input file go wrong?
		return -1;

	if(!output) //did something with the output file go wrong?
		return -2;

	int val = 0;
	bool didRun = false;//checks if we actually run any data through the cache (basically checks for blank input file)
	while(input >> val)
	{
		if(val < 0)
		{
			input.close();
			output.close();
			return -3;
		}
		didRun = true;
		bool hit = cache->getValueFromCache(val);//cache takes a memory value to retrieve and returns if it was cached or not, if not it is cached for next time
		output << val << " : " << (hit ? "HIT" : "MISS") << endl;

		cout << val << " : " << (hit ? "HIT" : "MISS") << endl;
	}

	if(!didRun)
		cout << "Input file is empty! Exiting...";

	input.close();
	output.close();

	return 0;
}

int stringToInt(char *str) //function to convert string input args to ints. there's probably a much better way to do this...
{
	int finalNum = 0;
	string a = str;
	for(int i = 0; i < a.length(); i++)
	{
		int num = (int) str[i];
		if(num < 48 || num > 57)
			return -1;
		else
			finalNum += (num - 48) * pow(10, a.length() - 1 - i);
	}
	return finalNum;
}
