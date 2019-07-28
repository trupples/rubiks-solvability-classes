#include "cube.hpp"
#include <map>
#include <queue>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>

std::map<int, int> visited_positions;	// visited_positions[cube.hash] = cube solvability class
int solvability_class_count = 0;

void logNewPosition(const Cube &pos)
{
	static int count = 0;
	const int total = 4*3*2 * 4*3*2 * 16 * 81 / 4;

	count++;
	std::cout << "\r[ " << std::fixed << std::setprecision(2) << count * 100.0 / total << "% ] " << count << "/" << total << " positions visited";
}

void exploreStartingAt(Cube &starting_position)
{
	if(visited_positions.count(starting_position.hash())) return;

	std::queue<Cube> bfs;

	const int current_class = solvability_class_count++;
	bfs.push(starting_position);

	while(!bfs.empty())
	{
		Cube vis = bfs.front(); bfs.pop();
		
		if(visited_positions.count(vis.hash()) == 1) continue;

		visited_positions[vis.hash()] = current_class;
		logNewPosition(vis);

		#define X(move) \
		{ \
			Cube vm(vis); \
			vm.move(); \
			if(visited_positions.count(vm.hash()) == 0) bfs.push(vm); \
		}
		X_SUFFICIENT_MOVES
		#undef X
	}
}

int main()
{
	int edges[4] = {0, 1, 2, 3};
	int corners[4] = {0, 1, 2, 3};

	for(int edgeIdx = 0; edgeIdx < 2*3*4; edgeIdx++)
	{
		for(int cornerIdx = 0; cornerIdx < 2*3*4; cornerIdx++)
		{
			for(int edgeOrientIdx = 0; edgeOrientIdx < (1<<4); edgeOrientIdx++)
				for(int cornerOrientIdx = 0; cornerOrientIdx < 81; cornerOrientIdx++)
				{
					Cube pos;
					pos.cornerStates[0] = {corners[0], cornerOrientIdx % 3};
					pos.cornerStates[1] = {corners[1], cornerOrientIdx / 3 % 3};
					pos.cornerStates[2] = {corners[2], cornerOrientIdx / 9 % 3};
					pos.cornerStates[3] = {corners[3], cornerOrientIdx / 27};
					pos.edgeStates[0] = {edges[0], edgeOrientIdx & 1};
					pos.edgeStates[1] = {edges[1], (edgeOrientIdx>>1) & 1};
					pos.edgeStates[2] = {edges[2], (edgeOrientIdx>>2) & 1};
					pos.edgeStates[3] = {edges[3], edgeOrientIdx>>3};

					exploreStartingAt(pos);
				}
			std::next_permutation(corners,corners+4);
		}
		std::next_permutation(edges,edges+4);
	}

	std::cout << std::endl;
	
	std::cout << solvability_class_count << " classes" << std::endl;

	// Count number of positions in each class. Should be the same for all classes
	std::vector<int> class_count(solvability_class_count);
	for(auto it : visited_positions)
	{
		class_count[it.second]++;
	}

	for(int i=0; i<solvability_class_count; i++)
	{
		std::cout << "Class " << i << ": " << class_count[i] << " positions" << std::endl;
	}

	std::cout << std::endl;

	std::cout << "Solvability class group operation:" << std::endl;

	std::cout << "F = flip edge" << std::endl;
	std::cout << "S = swap 2 adjacent edges" << std::endl;
	std::cout << "Rx = rotate corner counter clockwise x third-turns" << std::endl;

	std::string futere_mnemonics[] = {
		"0", "R1", "R2",
		"F", "FR1", "FR2",
		"S", "SR1", "SR2",
		"FS", "FSR1", "FSR2"
	};

	std::cout << "\t";
	for(int i=0; i<12; i++)
	{
		std::cout << futere_mnemonics[i] << "\t";
	}

	for(int first=0; first<12; first++)
	{
		std::cout << std::endl << futere_mnemonics[first] << "\t";
		for(int second=0; second<12; second++)
		{
			Cube c;
			c.applyFutere(first);
			c.applyFutere(second);

			std::cout << futere_mnemonics[visited_positions[c.hash()]] << "\t";
		}
	}

	return 0; 
}
