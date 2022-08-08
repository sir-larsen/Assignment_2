/**
 * @file Maze.cpp
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Source code for the Maze class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "Maze.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
 * @brief Construct a new Maze:: Maze object
 * 
 * @param loadedLevel - A ScenarioLoader containing the level file
 * @param shader 	  - The maze's shader
 * @param renderer 	  - The maze's renderer
 * 
 * @see	make2dArray();
 * @see	generateMaze();
 */
Maze::Maze(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer)
	: m_LoadedLevel(loadedLevel),
	  m_Renderer(renderer),
	  m_Shader(shader),
	  pelletCount(0)
{
	width = m_LoadedLevel->getHorizontalSize();
	height = m_LoadedLevel->getVerticalSize();

	map2d.resize(height, std::vector<int>(width));
	make2dArray();
	generateMaze();
	countPellets();
}

/**
 * @brief Destroy the Maze:: Maze object
 * 
 */
Maze::~Maze(){
	free(mazeVAO);
	free(mazeVBO);
	free(mazeVBLayout);
	free(mazeIBO);
}

/**
 * @brief Converts the 1D vector of the map from the ScenarioLoader into a 2D vector. 
 * 
 */
void Maze::make2dArray()
{
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			map2d[i][j] = m_LoadedLevel->mazeMap[index++];
		}
	}
}

/**
 * @brief Generates the indices for each square in the maze.
 * 
 */
void Maze::makeIndices()
{
	//Since we require +1 more indices than the amount of squares it is incremented.
	int indicesHeigth = height + 1; int indicesWidth = width + 1;

	for (int i = 0; i < indicesHeigth; i++) {
		for (int j = 0; j < indicesWidth; j++) {
			if (i < map2d.size() && j < map2d[i].size() && map2d[i][j] == 1) {
						mazeIndices.push_back((i * indicesWidth) + j);
						mazeIndices.push_back((i * indicesWidth) + j + 1);
						mazeIndices.push_back(((i + 1) * indicesWidth) + j);

						mazeIndices.push_back((i * indicesWidth) + j + 1);
						mazeIndices.push_back(((i + 1) * indicesWidth) + j);
						mazeIndices.push_back(((i + 1) * indicesWidth) + (j + 1));
			}
		}
	}
}

/**
 * @brief Generates positions to be used by the squares.
 * 
 */
void Maze::makePositions()
{
	int index = 0;
	for (int y = 0; y < height + 1; y++) {
		for (int x = 0; x < width + 1; x++) {
			mazePositions.push_back(glm::vec3(x, y, 0));
		}
	}
}

/**
 * @brief Generates the maze, from the generation of the positions/vertices, to the OpenGL stuff. 
 * @see makePositions();
 * @see makeIndices();
 */
void Maze::generateMaze()
{
	makePositions();
	makeIndices();

	mazeVAO = new VertexArray;
	mazeVAO->Bind();
	mazeVBO = new VertexBuffer(&mazePositions[0], mazePositions.size() * sizeof(glm::vec3));
	mazeVBO->Bind();
	mazeVBLayout = new VertexBufferLayout;
	mazeVBLayout->Push<float>(3);
	mazeVAO->AddBuffer(*mazeVBO, *mazeVBLayout);

	mazeIBO = new IndexBuffer(&mazeIndices[0], mazeIndices.size());

	m_Shader->use();
	m_Shader->setVec4("u_Color", 0.f, 0.125f, 0.76f, 1.f);
}

void Maze::camera(int hSize, int vSize)
{
	glm::mat4 projection = glm::ortho(0.f, (float)hSize, (float)vSize, 0.f);

	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_Shader->setMat4("u_ProjectionMat", projection);
	m_Shader->setMat4("u_ViewMat", view);


}

/**
 * @brief Counts how many !1 occour in the maze, counting how many "pellets" exists
*/
void Maze::countPellets()
{
	for (int i = 0; i < m_LoadedLevel->mazeMap.size(); i++)
		if (m_LoadedLevel->mazeMap[i] != 1)
			pelletCount++;
}

/**
 * @brief Draws the maze. 
 * 
 */
void Maze::draw()
{
	m_Shader->use();
	camera(width, height);
	m_Renderer->Draw(mazeVAO, mazeIBO, m_Shader);
}
