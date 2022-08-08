/**
 * @file ScenarioLoader.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief The source file for the ScenarioLoader class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "ScenarioLoader.h"

#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>

/**
 * @brief Construct a new Scenario Loader:: Scenario Loader object
 * 
 * @param filepath - The path to the file containing the level (maze).
 */
ScenarioLoader::ScenarioLoader(const std::string& filepath)
{
	std::ifstream levelFile(filepath);
	levelFile >> horizontalSize; levelFile.ignore(); levelFile >> verticalSize;

	int temp = 0;
	for (int i = 0; i < verticalSize; i++)
	{
		for (int j = 0; j < horizontalSize; j++)
		{
			levelFile >> temp;			//reading a new value
			mazeMap.push_back(temp);	//storing it in the mazeMap vector
		}
	}

}

/**
 * @brief Prints the currently stored "map", used for debugging only. 
 * 
 */
void ScenarioLoader::printMazeMap()
{
	for (int i = 0; i < (horizontalSize * verticalSize); i++)
	{
		if (i % horizontalSize == 0) std::cout << std::endl;
		std::cout << mazeMap[i] << ' ';
	}
}