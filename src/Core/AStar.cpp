/**
 * @file AStar.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Class for calculating a path from any given node to another given node. 
 * @version 0.1
 * @date 2020-11-17
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "AStar.h"
#include <vector>
#include <queue>
#include <stack>

/**
 * @brief Construct a new AStar::AStar object.
 * 
 * @param maze - The maze in which the Astar algorithm will do its pathfinding. 
 */
AStar::AStar(Maze3D* maze)
	:	m_maze(maze),
		height(maze->getHeight()),
		width(maze->getWidth())
{
	nodeMap.resize(height, std::vector<Node>(width));
	closedList.resize(height, std::vector<bool>(width));
}

/**
 * @brief 	Calculates the heuristics (remaining distance to the goal node) 
 * 			in any given node. Heuristics are calculated in manhatten distance. 
 * 
 * @param y - The y coordinate for the node we are calculating heuristics for.
 * @param x - The x coordinate for the node we are calculating heuristics for.
 * @param destination - The node of which we are calculating remaining distance too
 * @return 	returns the calculated heuristic distance.
 */
float AStar::calculateHeuristic(int y, int x, Node destination)
{
	return (abs(y - destination.y) + abs(x - destination.x));
}

/**
 * @brief Checks wheter or not any given node is traversable (Can be walked on/through).
 * 
 * @param y - The y coordinate for the node we are checking
 * @param x - The x coordinate for the node we are checking
 * @return true - The tile can be traversed
 * @return false - The tile is not traversable.
 */
bool AStar::isValid(int y, int x)
{
	return (m_maze->map2d[y][x] != 1);
}

/**
 * @brief Checks wheter or not any given node is the destination we are looking for. 
 * 
 * @param y The y coordinate for the node we are going to check wheter or not is the destination
 * @param x The x coordinate for the node we are going to check wheter or not is the destination
 * @param destination - The node we are checking against as the destination 
 * @return true 
 * @return false 
 */
bool AStar::isDestination(int y, int x, Node destination)
{
	return (y == destination.y && x == destination.x);
}

/**
 * @brief 	Will generate a path by backtracking its way back from the destination 
 * 			to the start node
 * 
 * @param destination 	The node from which we are going to start our backtracking.
 * @return vector<Node>  A std::vector containing the calculated path.
 */
vector<Node> AStar::makePath(Node destination)
{
	int y = destination.y;
	int x = destination.x;

	std::stack <Node> path;
	std::vector <Node> pathFromStart;
	while (!(nodeMap[y][x].parentY == y && nodeMap[y][x].parentX == x)
		&& (nodeMap[y][x].y != 1 && nodeMap[y][x].x != -1))
	{
		path.push(nodeMap[y][x]);
		int fooY = nodeMap[y][x].parentY; //Backtrack our way to the start node.
		int fooX = nodeMap[y][x].parentX;
		y = fooY; x = fooX;
	}
	path.push(nodeMap[y][x]); //Finally, push the start node onto the stack.

	int counter = 0;
	while (!path.empty()) //While still content on the stack
	{
		Node top = path.top();
		path.pop();
		//std::cout << counter++ << ": " << top.x << ' ' << top.y << std::endl;
		pathFromStart.emplace_back(top);
	}
	return pathFromStart; //return the now reversed path going from start -> destination
}

/**
 * @brief Clear the entire 2d array, preparing it for a fresh calculation of Astar. 
 * 
 */
void AStar::resetMap()
{
	//initialise our whole nodeMap to "undiscovered"
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++) {
			nodeMap[y][x].fCost = MAX_VALUE;
			nodeMap[y][x].gCost = MAX_VALUE;
			nodeMap[y][x].hCost = MAX_VALUE;
			nodeMap[y][x].parentX = -1;
			nodeMap[y][x].parentY = -1;
			nodeMap[y][x].x = x;
			nodeMap[y][x].y = y;

			closedList[y][x] = false; //setting every node to visited == false
		}
}

/**
 * @brief 	The actual algorithm for calculating the shortest path from any given node start
 * 			to any given node destination. 
 * 
 * @param start - The start node.
 * @param destination - The destination node. 
 * @return std::vector<Node> Will return the result std::vector of nodes from start to the destination.
 * @return std::vector<Node> Can also return an empty std::vector if there was no path available. 
 */
std::vector<Node> AStar::Pathfind(Node start, Node destination)
{
	resetMap();
	std::vector<Node> empty;
	if (isDestination(start.y, start.x, destination))
		return empty;
	//If the start node and the destination is the same, we have already found
	//our path and therefore we simply return an empty vector

	//initialise our starting list
	int x = start.x;
	int y = start.y;
	nodeMap[y][x].fCost = 0.0f;
	nodeMap[y][x].gCost = 0.0f;
	nodeMap[y][x].hCost = 0.0f;
	nodeMap[y][x].parentX = x;
	nodeMap[y][x].parentY = y;

	//We sort the priority queue based on the fCost of a given Node, making sure
	//that the node with the currently shortest total cost is popped first.
	std::priority_queue <Node, vector<Node>, CompareFCost> openList;
	openList.push(nodeMap[y][x]); //Push the start node onto the queue

	bool destinationFound = false; //Has destination been found?

	while (!openList.empty()) {
		Node node = openList.top(); //Picks the lowest fCost element in the queue.
		openList.pop();				//Pops it off the queue
		
		x = node.x;
		y = node.y;
		closedList[y][x] = true;	//Marks the current location as visited.

		for (int i = 0; i < 4; i++) { //Loop through all directions
			int newY = 0, newX = 0; //Reset for each iteration
			switch (i) {
			case 0: newY--; break; //Check "North"
			case 1: newY++; break; //Check "South"
			case 2: newX++; break; //Check "East"
			case 3: newX--; break; //Check "West"
			default: break;
			}
			float gNew, hNew, fNew;
			if (isValid(y + newY, x + newX)) //Node is not a wall
				if (isDestination(y + newY, x + newX, destination)) { //Node is not the destination
					//Found a path
					nodeMap[y + newY][x + newX].parentY = y; //sets the parent node
					nodeMap[y + newY][x + newX].parentX = x;
					destinationFound = true;
					return makePath(destination);
				} else if (closedList[y + newY][x + newX] == false) { //Node has not been visited
					gNew = node.gCost + 1.f;
					hNew = calculateHeuristic(y + newY, x + newX, destination);
					fNew = gNew + hNew;
					//Checks if this path is better than the current one
					if (nodeMap[y + newY][x + newX].fCost == MAX_VALUE ||
						nodeMap[y + newY][x + newX].fCost > fNew) 
					{
						//Update the neighbour nodes data
						nodeMap[y + newY][x + newX].fCost = fNew;
						nodeMap[y + newY][x + newX].gCost = gNew;
						nodeMap[y + newY][x + newX].hCost = hNew;
						nodeMap[y + newY][x + newX].parentY = y;
						nodeMap[y + newY][x + newX].parentX = x;
						openList.push(nodeMap[y + newY][x + newX]); //Add the neighbor to the queue
					}
				}
		}
	}
	if (!destinationFound) {
		std::cout << "Destination not found!\n";
		return empty;
	}

}
