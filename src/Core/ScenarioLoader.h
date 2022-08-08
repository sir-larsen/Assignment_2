/**
 * @file ScenarioLoader.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The header file for the ScenarioLoader class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

/**
 * @class ScenarioLoader
 * @brief  Handles the loading and saving of the content in the level files. 
 */
class ScenarioLoader
{
private:
	int horizontalSize, verticalSize;
public:
	std::vector <int> mazeMap;	//will hold data regarding the map of the maze
	ScenarioLoader(const std::string& filepath);
	void printMazeMap();
	int getHorizontalSize() { return horizontalSize; }
	int getVerticalSize() { return verticalSize; }

	int getValue(const int i) { return mazeMap[i]; }
	int getVecSize() { return mazeMap.size(); }
};