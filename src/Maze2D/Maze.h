/**
 * @file Maze.h
 * @author Sebastian Lindtvedt (lindtvedt.sebastian@gmail.com)
 * @brief Header file for the Maze class
 * @version 0.1
 * @date 2020-10-12
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "../Core/ScenarioLoader.h"
#include "../Core/VertexArray.h"
#include "../Core/VertexBuffer.h"
#include "../Core/VertexBufferLayout.h"
#include "../Core/IndexBuffer.h"
#include "../Core/Renderer.h"
#include "../Core/shader.h"

/**
 * @class Maze
 * @brief Handles the creation and drawing of the maze. 
 */
class Maze
{
private:
	int width,
		height,
		pelletCount;

	ScenarioLoader* m_LoadedLevel;
	std::vector <unsigned int> mazeIndices;
	std::vector <glm::vec3> mazePositions;

	Renderer* m_Renderer;
	Shader* m_Shader;

	VertexArray*		mazeVAO;
	VertexBuffer*		mazeVBO;
	VertexBufferLayout* mazeVBLayout;
	IndexBuffer*		mazeIBO;
public:

	std::vector<std::vector<int>> map2d;

	Maze(ScenarioLoader* loadedLevel, Shader* shader, Renderer* renderer);
	~Maze();

	void draw();
	inline std::vector <glm::vec3> getMazePositions() { return mazePositions; }
	inline int getHeight()	{ return height; }
	inline int getWidth()	{ return width; }
	inline int getPelletCount() { return pelletCount; }

private:
	void countPellets();
	void make2dArray();
	void makeIndices();
	void makePositions();
	void generateMaze();
	void camera(int hSize, int vSize);
};
