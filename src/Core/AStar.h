/**
 * @file AStar.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Astar class.
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "../Maze3D/Maze3D.h"

/**
 * @brief A structure holding data regarding any given node in the map. Used for pathfinding.
*/
struct Node {
	int		y,
			x,
			parentY,
			parentX;
	float	gCost,
			hCost,
			fCost;
};

/**
 * @brief A struct containing the function for comparing two nodes, used in combination
 * 		  with the priority_queue we are using for keeping track of nodes. 
 * 
 */
struct CompareFCost {
	bool operator()(Node const& n1, Node const& n2) {
		return n1.fCost > n2.fCost; //compares the combined value of travelled distance and heuristics in any given node
	}
};

/**
 * @class Astar -	Handling pathfinding from one node to another in a 2 dimensional
					grid, in this case the level file.
 * @brief	- Will calculate the best (shortest) path from point A to point B. 
*/
class AStar
{
public:
	AStar(Maze3D* maze);
	std::vector<Node> Pathfind(Node start, Node destination);

	std::vector<std::vector<Node>> nodeMap;
	std::vector<std::vector<bool>> closedList;
private:
	Maze3D* m_maze;


	int height, width;
	int MAX_VALUE = 999;

	float calculateHeuristic(int y, int x, Node destination);
	bool  isValid(int y, int x);
	bool  isDestination(int y, int x, Node destination);
	vector<Node> makePath(Node destination);
	void resetMap();
};